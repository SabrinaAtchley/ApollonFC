#include "peripherals.h"

void Buzzer::toneAndDelay(
  const unsigned int freq,
  const unsigned int length,
  const unsigned int restDelay = 0
) {
  tone(PERIPHERAL_BUZZER_PIN, freq, length);
  delay(length + restDelay);
}

void Buzzer::ready() {
  PLAY_MELODY(PERIPHERAL_BUZZER_STARTUP);
}
