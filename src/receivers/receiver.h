#ifndef APOLLON_FC_SRC_RECEIVERS_RECEIVER_H
#define APOLLON_FC_SRC_RECEIVERS_RECEIVER_H

/* Abstract which handles RF receivers
 * Receiver configuration is handled through flags in configuration.h
 * All receivers must extend the Receiver abstract class
 * A receiver must:
 *  Define RECEIVER_T to be the class name of the receiver
 *  Implement a void-arg constructor e.g. PPM_Receiver() : ppmReader(...) {};
 *  Implement the virtual function getSignal
 *  Ensure src/receivers.h includes the header file when the proper flag is set
 */

#include <Arduino.h>
#include "../../configuration.h"

// Abstract
class Receiver {

public:
  /* Derived classes must include a type definition which *must* be the class name
   * e.g. #define RECEIVER_T PPM_Receiver
   * This allows initializing a receiver as RECEIVER_T receiver;
   */
  uint16_t channels[INPUT_CHANNELS];

  // Get latest value for a particular channel
  uint16_t getChannel(const uint8_t ch);

  // Fetch raw channel signals, then convert into final pilot input controls
  void update();

#ifndef TEST_MODE // Make method public for testing purposes only
private:
#endif
  // Snap input signal to deadzone if it's within INPUT_DEADZONE_WIDTH of it
  void applyDeadzones(uint16_t &signal);

protected:
  /* Fetch signal from receiver for a given channel
   * (input range [INPUT_MOTOR_MIN, INPUT_MOTOR_MAX])
   */
  virtual void getSignal(const uint8_t ch) = 0;
};

#endif /* FLIGHT_CONTROLLER_SRC_RECEIVERS_RECEIVER_H */
