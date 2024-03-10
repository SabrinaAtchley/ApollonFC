#include "receiver.h"

#include "../macros.h"
#include "../error.h"
#include "../../configuration.h"


uint16_t Receiver::getChannel(const uint8_t ch) {
  if (ch < 1 || ch > INPUT_CHANNELS) {
    ERROR(2);
  }

  return channels[ch - 1];
}

void Receiver::update() {
  for (uint8_t ch = 0; ch < INPUT_CHANNELS; ch++) {
    getSignal(ch);
    applyDeadzones(channels[ch]);
  }
}

void Receiver::applyDeadzones(uint16_t &signal) {
  #define APOLLON_FC_SRC_RECEIVERS_CLAMP_DEADZONE(D) if (D - INPUT_DEADZONE_WIDTH < signal && signal < D + INPUT_DEADZONE_WIDTH) {signal = D; return;}

  MAP(APOLLON_FC_SRC_RECEIVERS_CLAMP_DEADZONE, INPUT_DEADZONE_STOPS);
}
