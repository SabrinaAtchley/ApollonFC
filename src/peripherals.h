#ifndef FLIGHT_CONTROLLER_SRC_PERIPHERALS_H
#define FLIGHT_CONTROLLER_SRC_PERIPHERALS_H

/* Defines several small hardware interfaces for taking direct user input
 * and output, e.g. piezo buzzers, LEDs, buttons, POV displays, etc
 */

#include <Arduino.h>
#include "../configuration.h"

#define PLAY_NOTE(T) toneAndDelay T

#define _PLAY_MELODY_1(N, M) PLAY_NOTE(ARG_N(0, M))
#define _PLAY_MELODY_2(N, M) _PLAY_MELODY_1(N, M);PLAY_NOTE(ARG_N(1, M))
#define _PLAY_MELODY_3(N, M) _PLAY_MELODY_2(N, M);PLAY_NOTE(ARG_N(2, M))
#define _PLAY_MELODY_4(N, M) _PLAY_MELODY_3(N, M);PLAY_NOTE(ARG_N(3, M))
#define _PLAY_MELODY_5(N, M) _PLAY_MELODY_4(N, M);PLAY_NOTE(ARG_N(4, M))
#define _PLAY_MELODY_6(N, M) _PLAY_MELODY_5(N, M);PLAY_NOTE(ARG_N(5, M))
#define _PLAY_MELODY_7(N, M) _PLAY_MELODY_6(N, M);PLAY_NOTE(ARG_N(6, M))
#define _PLAY_MELODY_8(N, M) _PLAY_MELODY_7(N, M);PLAY_NOTE(ARG_N(7, M))
#define _PLAY_MELODY_9(N, M) _PLAY_MELODY_8(N, M);PLAY_NOTE(ARG_N(8, M))
#define _PLAY_MELODY_10(N, M) _PLAY_MELODY_9(N, M);PLAY_NOTE(ARG_N(9, M))
#define _PLAY_MELODY_11(N, M) _PLAY_MELODY_10(N, M);PLAY_NOTE(ARG_N(10, M))
#define _PLAY_MELODY_12(N, M) _PLAY_MELODY_11(N, M);PLAY_NOTE(ARG_N(11, M))
#define _PLAY_MELODY_13(N, M) _PLAY_MELODY_12(N, M);PLAY_NOTE(ARG_N(12, M))
#define _PLAY_MELODY_14(N, M) _PLAY_MELODY_13(N, M);PLAY_NOTE(ARG_N(13, M))
#define _PLAY_MELODY_15(N, M) _PLAY_MELODY_14(N, M);PLAY_NOTE(ARG_N(14, M))
#define _PLAY_MELODY_16(N, M) _PLAY_MELODY_15(N, M);PLAY_NOTE(ARG_N(15, M))
#define __PLAY_MELODY_N(N, M) _PLAY_MELODY_##N(N, M)
#define _PLAY_MELODY_N(N, M) __PLAY_MELODY_N(N, M)

#define PLAY_MELODY(M) _PLAY_MELODY_N(SIZE_LIST(PERIPHERAL_BUZZER_STARTUP), M)

#define BUZZER_ERROR_CODE(C) while(true){for(byte i = PERIPHERAL_BUZZER_MAX_BITS - 1; i>= 0; i--){Buzzer::toneAndDelay(bitRead(C, i) ? PERIPHERAL_BUZZER_HIGH : PERIPHERAL_BUZZER_LOW, PERIPHERAL_BUZZER_TONE_LENGTH, PERIPHERAL_BUZZER_INTERTONE_DELAY);}delay(PERIPHERAL_BUZZER_REPEAT_DELAY);}


namespace Buzzer {

  void toneAndDelay(
    const unsigned int freq,
    const unsigned int length,
    const unsigned int restDelay = 0
  ) {
    tone(PERIPHERAL_BUZZER_PIN, freq, length);
    delay(length + restDelay);
  }

  // Startup jingle when ready to fly
  void ready() {
    PLAY_MELODY(PERIPHERAL_BUZZER_STARTUP);
  }
}

#endif
