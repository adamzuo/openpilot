#pragma once

#include <string>
#include <vector>

#include "common/params.h"
#include "selfdrive/pandad/panda.h"

void pandad_main_thread(std::vector<std::string> serials);

// deprecated devices
static const std::vector<cereal::PandaState::PandaType> SUPPORTED_PANDA_TYPES = {
  cereal::PandaState::PandaType::RED_PANDA,
  cereal::PandaState::PandaType::TRES,
  cereal::PandaState::PandaType::CUATRO,
};


class PandaSafety {
public:
  PandaSafety(const std::vector<Panda *> &pandas) : pandas_(pandas) {}
  void configureSafetyMode(bool is_onroad);
  bool getOffroadMode();

private:
  void updateMultiplexingMode();
  std::vector<std::string> fetchCarParams();
  void setSafetyMode(const std::vector<std::string> &params_string);
  bool verifySafetyMode();

  bool initialized_ = false;
  bool log_once_ = false;
  bool safety_configured_ = false;
  bool prev_obd_multiplexing_ = false;
  std::vector<Panda *> pandas_;
  Params params_;

  // set_alternative_experience()/set_safety_model() are one-shot USB control writes with no
  // built-in ack -- verifySafetyMode() confirms panda actually applied what was sent (see
  // panda_safety.cc for why this matters), and these track that verification across retries.
  uint16_t expected_alternative_experience_ = 0;
  int safety_verify_attempts_ = 0;
};
