#include "selfdrive/pandad/pandad.h"
#include "openpilot/cereal/messaging/messaging.h"
#include "common/swaglog.h"

// How many 10Hz configureSafetyMode() ticks (i.e. ~100ms apart) to keep retrying quietly before
// escalating to a loud, rate-limited error log. Retries never stop entirely -- see the comment
// in configureSafetyMode() for why giving up outright would be worse than logging loudly.
constexpr int SAFETY_MODE_LOUD_RETRY_ATTEMPTS = 50;  // ~5s

void PandaSafety::configureSafetyMode(bool is_onroad) {
  if (is_onroad && !safety_configured_) {
    updateMultiplexingMode();

    auto car_params = fetchCarParams();
    if (!car_params.empty()) {
      if (safety_verify_attempts_ == 0) {
        LOGW("got %lu bytes CarParams", car_params[0].size());
        LOGW("got %lu bytes CarParamsSP", car_params[1].size());
      }

      setSafetyMode(car_params);
      safety_verify_attempts_ += 1;

      // set_alternative_experience() and set_safety_model() (both called from setSafetyMode(),
      // above) are one-shot USB control writes with no built-in acknowledgement or retry. A
      // single dropped or corrupted transfer -- more likely on a less reliable link, e.g. an
      // externally-connected USB panda -- would otherwise leave alternativeExperience (which
      // gates MADS/lateral-only control, among other things) silently stuck at its old value for
      // the entire drive, on any car, with no way to recover mid-drive.
      //
      // verifySafetyMode() reads panda's live health state back and confirms it actually matches
      // what we just sent. If it doesn't, safety_configured_ stays false and this function will
      // simply be called again on the next 10Hz tick (see the pandad main loop), which resends
      // setSafetyMode() -- a cheap, safe retry for what's almost certainly a transient comms
      // issue. We deliberately never fall back to setting safety_configured_ = true without a
      // verified match: doing so would silently mask the exact class of problem this exists to
      // catch, and downstream mismatch detection (e.g. MADS's lateral_mismatch_counter) depends
      // on panda's real state, not on pandad's optimistic assumption of it.
      if (verifySafetyMode()) {
        safety_configured_ = true;
        safety_verify_attempts_ = 0;
      } else if (safety_verify_attempts_ >= SAFETY_MODE_LOUD_RETRY_ATTEMPTS) {
        LOGE_100("panda safety mode / alternative experience still unverified after %d attempts -- "
                 "still retrying, but MADS/lateral-only control may be unavailable this drive",
                 safety_verify_attempts_);
      }
    }
  } else if (!is_onroad) {
    initialized_ = false;
    safety_configured_ = false;
    safety_verify_attempts_ = 0;
    log_once_ = false;
  }
}

void PandaSafety::updateMultiplexingMode() {
  if (!initialized_) {
    prev_obd_multiplexing_ = false;
    for (size_t i = 0; i < pandas_.size(); ++i) {
      pandas_[i]->set_safety_model(cereal::CarParams::SafetyModel::ELM327, 1U);
    }
    initialized_ = true;
  }

  bool obd_multiplexing_requested = params_.getBool("ObdMultiplexingEnabled");
  if (obd_multiplexing_requested != prev_obd_multiplexing_) {
    for (size_t i = 0; i < pandas_.size(); ++i) {
      const uint16_t safety_param = (i > 0 || !obd_multiplexing_requested) ? 1U : 0U;
      pandas_[i]->set_safety_model(cereal::CarParams::SafetyModel::ELM327, safety_param);
    }
    prev_obd_multiplexing_ = obd_multiplexing_requested;
    params_.putBool("ObdMultiplexingChanged", true);
  }
}

std::vector<std::string> PandaSafety::fetchCarParams() {
  if (!params_.getBool("FirmwareQueryDone")) {
    return {};
  }

  if (!log_once_) {
    LOGW("Finished FW query, Waiting for params to set safety model");
    log_once_ = true;
  }

  if (!params_.getBool("ControlsReady")) {
    return {};
  }
  return {params_.get("CarParams"), params_.get("CarParamsSP")};
}

void PandaSafety::setSafetyMode(const std::vector<std::string> &params_string) {
  AlignedBuffer aligned_buf;
  AlignedBuffer aligned_buf_sp;

  capnp::FlatArrayMessageReader cmsg(aligned_buf.align(params_string[0].data(), params_string[0].size()));
  cereal::CarParams::Reader car_params = cmsg.getRoot<cereal::CarParams>();

  capnp::FlatArrayMessageReader cmsg_sp(aligned_buf_sp.align(params_string[1].data(), params_string[1].size()));
  cereal::CarParamsSP::Reader car_params_sp = cmsg_sp.getRoot<cereal::CarParamsSP>();

  auto safety_configs = car_params.getSafetyConfigs();
  uint16_t alternative_experience = car_params.getAlternativeExperience();
  uint16_t safety_param_sp = car_params_sp.getSafetyParam();
  expected_alternative_experience_ = alternative_experience;

  for (size_t i = 0; i < pandas_.size(); ++i) {
    cereal::CarParams::SafetyModel safety_model = cereal::CarParams::SafetyModel::SILENT;
    uint16_t safety_param = 0U;
    if (i < safety_configs.size()) {
      safety_model = safety_configs[i].getSafetyModel();
      safety_param = safety_configs[i].getSafetyParam();
    }

    LOGW_100("Panda %zu: setting safety model: %d, param: %d, alternative experience: %d, param_sp: %d",
             i, (int)safety_model, safety_param, alternative_experience, safety_param_sp);
    pandas_[i]->set_alternative_experience(alternative_experience, safety_param_sp);
    pandas_[i]->set_safety_model(safety_model, safety_param);
  }
}

bool PandaSafety::verifySafetyMode() {
  for (Panda *panda : pandas_) {
    std::optional<health_t> health = panda->get_state();
    if (!health || (health->alternative_experience_pkt != expected_alternative_experience_)) {
      return false;
    }
  }
  return true;
}

bool PandaSafety::getOffroadMode() {
  return params_.getBool("OffroadMode");
}
