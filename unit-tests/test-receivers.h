#ifndef APOLLON_FC_UNIT_TESTS_TEST_RECEIVERS_H
#define APOLLON_FC_UNIT_TESTS_TEST_RECEIVERS_H

#include "test-framework.h"
#include "../src/receivers.h"
#include "../configuration.h"
#include "../src/macros.h"


void unitReceivers() {
  RECEIVER_T receiver;

  TEST_UNIT(Receivers, {
    TEST_SECTION(Constructor, {
      #if ENABLED(INPUT_RECEIVER_PPM)
      REQUIRE(1.1, receiver.receiverType == "PPM_Receiver");
      #elif ENABLED(INPUT_RECEIVER_PWM)
      REQUIRE(1.1, receiver.receiverType == "PWM_Receiver");
      #elif ENABLED(INPUT_RECEIVER_SBUS)
      REQUIRE(1.1, receiver.receiverType == "SBUS_Receiver");
      #elif ENABLED(INPUT_RECEIVER_IBUS)
      REQUIRE(1.1, receiver.receiverType == "IBUS_Receiver");
      #endif
    }); /* Constructor */

    TEST_SECTION(Deadzones, {
      uint16_t signal;
      #define UNIT_TESTS_TEST_RECEIVERS_TEST_DEADZONE(NAME, SIG, EXP) signal = SIG; receiver.applyDeadzones(signal); TEST_CASE(NAME, signal == EXP);
      #define UNIT_TESTS_TEST_RECEIVERS_DEADZONES(D) UNIT_TESTS_TEST_RECEIVERS_TEST_DEADZONE(CAT(2.1., D), D - INPUT_DEADZONE_WIDTH - 1, D - INPUT_DEADZONE_WIDTH - 1);UNIT_TESTS_TEST_RECEIVERS_TEST_DEADZONE(CAT(2.2., D), D - INPUT_DEADZONE_WIDTH, D - INPUT_DEADZONE_WIDTH);UNIT_TESTS_TEST_RECEIVERS_TEST_DEADZONE(CAT(2.3., D), D - INPUT_DEADZONE_WIDTH + 1, D);UNIT_TESTS_TEST_RECEIVERS_TEST_DEADZONE(CAT(2.4., D), D, D);UNIT_TESTS_TEST_RECEIVERS_TEST_DEADZONE(CAT(2.5., D), D + INPUT_DEADZONE_WIDTH - 1, D);UNIT_TESTS_TEST_RECEIVERS_TEST_DEADZONE(CAT(2.6., D), D + INPUT_DEADZONE_WIDTH, D + INPUT_DEADZONE_WIDTH);UNIT_TESTS_TEST_RECEIVERS_TEST_DEADZONE(CAT(2.7., D), D + INPUT_DEADZONE_WIDTH + 1, D + INPUT_DEADZONE_WIDTH + 1);

      MAP(UNIT_TESTS_TEST_RECEIVERS_DEADZONES, INPUT_DEADZONE_STOPS);
    }); /* Deadzones */

    delay(3000);
    TEST_SECTION(Controls, {

      // Plot controls for receiver testing
      while(true) {
        receiver.update();
        for (uint8_t ch = 1; ch <= INPUT_CHANNELS; ch++) {
          Serial.print("Ch ");
          Serial.print(ch);
          Serial.print(":");
          Serial.print(receiver.getChannel(ch));
          Serial.print(",");
        }
        Serial.println();
      }
    }); /* Controls */

  }); /* Receivers */
}

#endif /* FLIGHT_CONTROLLER_UNIT_TESTS_TEST_RECEIVER_H */
