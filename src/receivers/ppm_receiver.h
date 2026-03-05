#ifndef APOLLON_FC_SRC_RECEIVERS_PPM_RECEIVER_H
#define APOLLON_FC_SRC_RECEIVERS_PPM_RECEIVER_H

#include <PPMReader.h>
#include "receiver.h"
#include "../../configuration.h"

#ifndef RECEIVER_T
#define RECEIVER_T PPM_Receiver
#endif


class PPM_Receiver : public Receiver {
private:
  PPMReader ppmReader;

public:
  PPM_Receiver() : ppmReader(SERIAL_RECEIVER_PIN, INPUT_CHANNELS) {}

private:
  unsigned long microsAtLastVariance = 0;
  uint16_t lastRawValues[INPUT_CHANNELS];
  bool areRawValuesInitialized = false;

  void getSignal(const uint8_t ch) {
    if (!areRawValuesInitialized) {
      for (uint8_t ch = 0; ch < INPUT_CHANNELS; ch++) {
        lastRawValues[ch] == ppmReader.rawChannelValue(ch + 1);
      }
      areRawValuesInitialized = true;
    }

    // If the channel is unchanged for > x micros, we've lost signal (no noise)
    uint16_t currentRawValue = ppmReader.rawChannelValue(ch + 1);
    if (lastRawValues[ch] == currentRawValue) {
      if (micros() - microsAtLastVariance > 500000L) {
        channels[ch] = INPUT_MOTOR_MIN;
      }
    } else { // Signal has varied
      microsAtLastVariance = micros();
      channels[ch] = ppmReader.latestValidChannelValue(ch + 1, channels[ch]);
    }

    lastRawValues[ch] = currentRawValue;
  }
};

#endif /* APOLLON_FC_SRC_RECEIVERS_PPM_RECEIVER_H */
