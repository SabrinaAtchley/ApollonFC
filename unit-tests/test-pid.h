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

  void setDutyCycle(const float duty) {
    dutyCycle = CLAMP(duty, 0, 100) / 100;;
  }

  void update(const unsigned long deltaT) {
    double energy = thermalMass * temperature;
    energy += (dutyCycle * heaterPower - coolingRate) * deltaT;
    temperature = energy / thermalMass;
  }
};

void unitPID() {
  float u = 0.0, setPoint = 30.0;
  float kp = 8.0, ki = 0.5, kd = 16.0;
  static constexpr float iMin = -4, iMax = 4;
  static constexpr float pScale = 10, iScale = 1, dScale = 10;

  PID pid(kp, ki, kd, pScale, iScale, dScale, iMin, iMax);
  HeaterSimulation heater;
  unsigned long previousTime = millis(), currentTime, deltaT;

  delay(10);
  while (true) {
    currentTime = millis();
    deltaT = currentTime - previousTime;
    previousTime = currentTime;

    // Print setpoint and control variable
    u = pid.update(setPoint, heater.temperature, deltaT);
    heater.setDutyCycle(u);
    heater.update(deltaT);

    Serial.print("setPoint:");
    Serial.print(setPoint);
    Serial.print(",control:");
    Serial.print(u);
    Serial.print(",process:");
    Serial.print(heater.temperature);
    Serial.print(",error:");
    Serial.print(setPoint - heater.temperature);
    Serial.print(",p:");
    Serial.print(pid.getLastPTerm());
    Serial.print(",i:");
    Serial.print(pid.getLastITerm());
    Serial.print(",d:");
    Serial.println(pid.getLastDTerm());

    // Check for user commands
    if (Serial.available()) {
      String input = Serial.readString();
      input.trim();

      if (input.startsWith("kp ")) {
        kp = input.substring(3).toFloat();
        pid.setCoefficients(kp, ki, kd);

      } else if (input.startsWith("ki ")) {
        ki = input.substring(3).toFloat();
        pid.setCoefficients(kp, ki, kd);

      } else if (input.startsWith("kd ")) {
        kd = input.substring(3).toFloat();
        pid.setCoefficients(kp, ki, kd);

      } else if (input.startsWith("set ")) {
        setPoint = input.substring(4).toFloat();

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
