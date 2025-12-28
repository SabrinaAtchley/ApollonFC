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
  void getSignal(const uint8_t ch) {
    // Use last value as default
    channels[ch] = ppmReader.latestValidChannelValue(ch + 1, channels[ch]);
  }
};

#endif /* APOLLON_FC_SRC_RECEIVERS_PPM_RECEIVER_H */
