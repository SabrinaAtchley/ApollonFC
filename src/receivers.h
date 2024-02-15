#ifndef APOLLON_FC_SRC_RECEIVERS_H
#define APOLLON_FC_SRC_RECEIVERS_H

/* Classes which handle RF receivers
 * Receiver configuration is handled through flags in configuration.h
 * All receivers must extend the Receiver abstract class
 * A receiver must:
 *  Define RECEIVER_T to the class name of the receiver
 *  Implement a RECEIVER_T constructor which passes a receiver type string to the
 *    Receiver constructor, usually the class name
 *  Implement the virtual function getSignal
 */

#include "../configuration.h"
#include "macros.h"

// PPM specific includes
#if ENABLED(INPUT_RECEIVER_PPM)
#include <PPMReader.h>

// PWM specific includes
#elif ENABLED(INPUT_RECEIVER_PWM)

// SBUS specific includes
#elif ENABLED(INPUT_RECEIVER_SBUS)

// IBUS specific includes
#elif ENABLED(INPUT_RECEIVER_IBUS)

#endif

// Abstract
class Receiver {

public:
  /* Derived classes must have a constructor which sets receiverType to
   * a class-specific value, usually the class name
   *
   * e.g. PPM_Receiver() : Receiver("PPM_Receiver") {}
   *
   * Must also include a type definition which *must* be the class name
   * e.g. #define RECEIVER_T PPM_Receiver
   */
  const String receiverType;
  uint16_t channels[INPUT_CHANNELS];

  Receiver(const String type) : receiverType(type) {};

  uint16_t getChannel(const uint8_t ch) {
    if (ch < 1 || ch > INPUT_CHANNELS)
      ERROR(2);
    return channels[ch - 1];
  }

  // Fetch raw channel signals, then convert into final pilot input controls
  void update() {
    for (uint8_t ch = 0; ch < INPUT_CHANNELS; ch++) {
      getSignal(ch);
      applyDeadzones(channels[ch]);
    }
  }

#ifndef TEST_MODE
private:
#endif
  // Snap input signal to deadzone if it's within INPUT_DEADZONE_WIDTH of it
  void applyDeadzones(uint16_t &signal) {
    #define SRC_RECEIVERS_CLAMP_DEADZONE(D) if(D - INPUT_DEADZONE_WIDTH < signal && signal < D + INPUT_DEADZONE_WIDTH) {signal = D; return;}

    MAP(SRC_RECEIVERS_CLAMP_DEADZONE, INPUT_DEADZONE_STOPS);
  }

protected:
  /* Fetch signal from receiver for a given channel
   * (input range [INPUT_MOTOR_MIN, INPUT_MOTOR_MAX])
   */
  virtual void getSignal(const uint8_t ch) = 0;
};


// Receiver types

#if ENABLED(INPUT_RECEIVER_PPM)
class PPM_Receiver : public Receiver {
  #define RECEIVER_T PPM_Receiver

private:
  PPMReader ppmReader;

public:
  PPM_Receiver() : Receiver("PPM_Receiver"), ppmReader(SERIAL_RECEIVER_PIN, INPUT_CHANNELS) {}

private:
  void getSignal(const uint8_t ch) {
    channels[ch] = ppmReader.latestValidChannelValue(ch + 1, INPUT_MOTOR_MIN);
  }
};

// Implement other receiver types here

#elif ENABLED(INPUT_RECEIVER_PWM)
class PWM_Receiver : public Receiver {};

#elif ENABLED(INPUT_RECEIVER_SBUS)
class SBUS_Receiver : public Receiver {};

#elif ENABLED(INPUT_RECEIVER_IBUS)
class IBUS_Receiver : public Receiver {};

#endif

#endif /* FLIGHT_CONTROLLER_SRC_RECEIVERS_H */
