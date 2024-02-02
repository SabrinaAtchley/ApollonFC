#ifndef FLIGHT_CONTROLLER_SRC_RECEIVERS_H
#define FLIGHT_CONTROLLER_SRC_RECEIVERS_H

#include <PPMReader.h>
#include "../configuration.h"

// Abstract
class Receiver {

private:
  // Snap input signal to deadzone if it's within INPUT_DEADZONE_WIDTH of it
  void applyDeadzone(uint16_t &signal) {
    for (const auto deadzone : INPUT_DEADZONE_STOPS) {
      if (abs(signal - deadzone) < INPUT_DEADZONE_WIDTH) {
        signal = deadzone;
        break;
      }
    }
  }

protected:
  /* Fetch signal from receiver for a given channel
   * (input range [INPUT_MOTOR_MIN, INPUT_MOTOR_MAX])
   */
  void getSignal(uint16_t &channels, const byte ch) = 0;

public:
  // Fetch raw channel signals, then convert into final pilot input controls
  void getControls(uint16_t &channels) {
    for (const byte ch = 0; ch < INPUT_CHANNELS; ch++) {
      getChannel(channels, ch);
      applyDeadzone(channels[ch]);
    }
  }
};

class PPM_Receiver : public Receiver {
  reader PPMReader(SERIAL_RECEIVER_PIN, INPUT_CHANNELS);

  void getSignal(uint16_t &channels, const byte ch) {
    channels[ch] = ppmReader.latestValidChannelValue(ch + 1, INPUT_MOTOR_MIN);
  }
};

// Implement other receiver types here

#endif
