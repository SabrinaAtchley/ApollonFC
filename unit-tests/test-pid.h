#ifndef APOLLON_FC_UNIT_TESTS_TEST_PID_H
#define APOLLON_FC_UNIT_TESTS_TEST_PID_H

#include "../src/pid.h"

// Tests the PID controller responds to coefficient changes appropriately
// The heater simulation leaves some to be desired

class HeaterSimulation {
private:
  // Completely random arbitrary test values
  float thermalMass = 4.179;
  float heaterPower = 0.005;
  float coolingRate = 0.0001;
  float dutyCycle = 0.0;

public:
  float temperature = 20.0;

  void reset() { // reset temperature to room temp
    temperature = 20.0;
  }

  void setDutyCycle(const Q16x16 duty) {
    dutyCycle = CLAMP(q16x16tof(duty), 0, 100) / 100;;
  }

  void update(const unsigned long deltaT) {
    double energy = thermalMass * temperature;
    energy += (dutyCycle * heaterPower - coolingRate) * deltaT;
    temperature = energy / thermalMass;
  }
};

void unitPID() {
  Q16x16 u = 0, setPoint = ftoq16x16(30.0);
  const Q16x16 kp = ftoq16x16(8.0), ki = ftoq16x16(2.0), kd = ftoq16x16(16.0),
              alpha = ftoq16x16(0.5);
  static constexpr Q16x16 iMin = -itoq16x16(4), iMax = itoq16x16(4);
  static constexpr Q16x16 pScale = itoq16x16(10), iScale = itoq16x16(1), dScale = itoq16x16(10);

  PID pid(kp, ki, kd, pScale, iScale, dScale, iMin, iMax, alpha);
  HeaterSimulation heater;
  unsigned long previousTime = millis(), currentTime, deltaT;

  delay(10);
  while (true) {
    currentTime = millis();
    deltaT = currentTime - previousTime;
    previousTime = currentTime;

    // Print setpoint and control variable
    u = pid.update(setPoint, ftoq16x16(heater.temperature), deltaT);
    heater.setDutyCycle(u);
    heater.update(deltaT);

    Serial.print("setPoint:");
    Serial.print(q16x16tof(setPoint));
    Serial.print(",control:");
    Serial.print(q16x16tof(u));
    Serial.print(",process:");
    Serial.print(heater.temperature);
    Serial.print(",error:");
    Serial.print(q16x16tof(setPoint) - heater.temperature);
    Serial.print(",p:");
    Serial.print(q16x16tof(pid.getLastPTerm()));
    Serial.print(",i:");
    Serial.print(q16x16tof(pid.getLastITerm()));
    Serial.print(",d:");
    Serial.println(q16x16tof(pid.getLastDTerm()));

    // Check for user commands
    if (Serial.available()) {
      String input = Serial.readString();
      input.trim();

      /*
      if (input.startsWith("kp ")) {
        kp = input.substring(3).toFloat();
        pid.setCoefficients(kp, ki, kd);

      } else if (input.startsWith("ki ")) {
        ki = input.substring(3).toFloat();
        pid.setCoefficients(kp, ki, kd);

      } else if (input.startsWith("kd ")) {
        kd = input.substring(3).toFloat();
        pid.setCoefficients(kp, ki, kd);
      */

      if (input.startsWith("set ")) {
        setPoint = ftoq16x16(input.substring(4).toFloat());

      } else if (input.startsWith("reset")) {
        u = 0.0;
        heater.reset();

      } else if (input.startsWith("stop")) {
        break;
      }
    }
  }

  Serial.print("(kp, ki, kd): (");
  Serial.print(kp);
  Serial.print(", ");
  Serial.print(ki);
  Serial.print(", ");
  Serial.print(kd);
  Serial.println(")");
}

#endif /* APOLLON_FC_UNIT_TESTS_TEST_PID_H */
