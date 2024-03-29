#ifndef APOLLON_FC_SRC_PERIPHERALS_H
#define APOLLON_FC_SRC_PERIPHERALS_H

/* Defines several small hardware interfaces for taking direct user input
 * and output, e.g. piezo buzzers, LEDs, buttons, POV displays, etc
 */

#include <Arduino.h>
#include "../configuration.h"
#include "macros.h"

#define PLAY_NOTE(T) Buzzer::toneAndDelay T
#define PLAY_MELODY(M) MAP(PLAY_NOTE, PERIPHERAL_BUZZER_STARTUP)

#define BUZZER_ERROR_CODE(C) while(true){for(byte i = PERIPHERAL_BUZZER_MAX_BITS; i > 0; i--){Buzzer::toneAndDelay(bitRead(C, i - 1) ? PERIPHERAL_BUZZER_HIGH : PERIPHERAL_BUZZER_LOW, PERIPHERAL_BUZZER_TONE_LENGTH, PERIPHERAL_BUZZER_INTERTONE_DELAY);}delay(PERIPHERAL_BUZZER_REPEAT_DELAY);}


namespace Buzzer {

  void toneAndDelay(
    const unsigned int freq,
    const unsigned int length,
    const unsigned int restDelay = 0
  );

  // Startup jingle when ready to fly
  void ready();
}

#endif
