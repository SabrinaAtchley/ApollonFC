#include "esc.h"

Servo SpeedController::escs[MOTOR_AMOUNT];

void SpeedController::setup() {
  uint8_t i = 0;
  #define SRC_ESC_SPEEDCONTROLLER_ESC_INIT(P) SpeedController::escs[i++].attach(P, INPUT_MOTOR_MIN, INPUT_MOTOR_MAX)
  MAP(SRC_ESC_SPEEDCONTROLLER_ESC_INIT, ESC_PINS);
}

void SpeedController::write(const uint16_t *signals) {
  for (uint8_t i = 0; i < MOTOR_AMOUNT; i++) {
    SpeedController::escs[i].writeMicroseconds(signals[i]);
  }
}
