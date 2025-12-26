#include "main.h"

/* Class instance declarations */
// Reads input from the radio controller
RECEIVER_T receiver;
// Converts receiver input into target drone state values
PilotControl pilot; // This could be a namespace instead, or we could use it as non-instanced class
// Handles PIDs, motor mixing, and writing to ESCs
MotorController motors;
DroneState state;

const Q16x16 k_pitchRate = ftoq16x16(P_PITCHSPEED);
const Q16x16 k_rollRate = ftoq16x16(P_ROLLSPEED);

Q16x16 gx, gy, gz, ax, ay, az;

unsigned long nextSlowUpdate = 0; // Updates every 200Hz (5000μs)
unsigned long lastSlowUpdate = 0;
const unsigned long microsPerSlow = (int32_t) 1000000 / 200;
unsigned long nextFastUpdate = 0; // Updates every 500Hz (2000μs)
unsigned long lastFastUpdate = 0;
const unsigned long microsPerFast = (int32_t) 1000000 / 500;


// Useful constants
const Q16x16 qPi = ftoq16x16(3.1415);
const Q16x16 q180 = itoq16x16(180);
const Q16x16 qHalf = 0x00008000;
const Q16x16 thousand = itoq16x16(1000);

const Q16x16 rollSpeedMin = ftoq16x16(PILOT_ROLLSPEED_MIN);
const Q16x16 rollSpeedMax = ftoq16x16(PILOT_ROLLSPEED_MAX);
const Q16x16 pitchSpeedMin = ftoq16x16(PILOT_PITCHSPEED_MIN);
const Q16x16 pitchSpeedMax = ftoq16x16(PILOT_PITCHSPEED_MAX);

// Sensor initialization
// TODO: Revisit sensor configuration for the case of multiple sensors (e.g. 2 barometers)
#ifdef SENSOR_BMP180
Sensor_BMP180 *baro;
// Sensor_BMP180 bmp180(BMP180_MODE_ULTRA_HIGH_RES, 102070); // oss, pressure at sea level
#endif
#ifdef SENSOR_HMC5883L
Sensor_HMC5883L *mag;
// Sensor_HMC5883L hmc5883l(HMC5883L_MODE_SINGLE, HMC5883L_GAIN_1, HMC5883L_OSS_3);
#endif
#ifdef SENSOR_MPU6050
Sensor_MPU6050 *imu;
// Sensor_MPU6050 imu(false, false, false, MPU6050_GYRO_SCALE_500, MPU6050_ACCEL_SCALE_4G);
#endif



void Main::setup() {
  // Initialize sensors
  #ifdef SENSOR_BMP180
  Sensor_BMP180 baro_obj(BMP180_MODE_ULTRA_HIGH_RES, 102070); // oss, pressure at sea level
  baro = &baro_obj;
  #endif
  #ifdef SENSOR_HMC5883L
  Sensor_HMC5883L mag_obj(HMC5883L_MODE_SINGLE, HMC5883L_GAIN_1, HMC5883L_OSS_3);
  mag = &mag_obj;
  #endif
  #ifdef SENSOR_MPU6050
  Sensor_MPU6050 imu_obj(false, false, false, MPU6050_GYRO_SCALE_500, MPU6050_ACCEL_SCALE_4G);
  imu = &imu_obj;
  #endif


  // Check IMU
  if (imu->selfTest() ^ 0x3F) { // Self test failed
    ERROR(1);
  }

  // Play start up jingle when finished initializing
  Buzzer::ready();
}

// Main process loop
void Main::loop() {
  if (micros() >= nextFastUpdate) {
    Q16x16 fastDeltaT = micros() - lastFastUpdate;
    lastFastUpdate = micros();
    nextFastUpdate += microsPerFast;
    // DeltaT should be small and calling q16x16_div_s would require
    // two calls since 1000000 can't fit into 16 bits.
    fastDeltaT = itoq16x16(fastDeltaT) / 1000000;
    fastUpdate(fastDeltaT);
  }

  if (micros() >= nextSlowUpdate) {
    Q16x16 slowDeltaT = micros() - lastSlowUpdate;
    lastSlowUpdate = micros();
    nextSlowUpdate += microsPerSlow;
    // DeltaT should be small and calling q16x16_div_s would require
    // two calls since 1000000 can't fit into 16 bits.
    slowDeltaT = itoq16x16(slowDeltaT) / 1000000;
    slowUpdate(slowDeltaT);
  }
}

// Read pilot commands, Estimate current attitude, angle PIDs
void slowUpdate(const Q16x16 deltaT) {
  // Read pilot commands and update drone state
  receiver.update();
  pilot.update(state, receiver);

  // Convert pilot commands into radians
  // TODO: Consider folding this into the Pilot Control module
  if (state.flightMode == FLIGHT_MODE_ACRO) {
    state.rollSpeed.target = q16x16_div_s(q16x16_mul_s(state.rollSpeed.target, qPi), q180);
    state.pitchSpeed.target = q16x16_div_s(q16x16_mul_s(state.pitchSpeed.target, qPi), q180);
  } else if (state.flightMode == FLIGHT_MODE_ANGLE){
    state.rollAngle.target = q16x16_div_s(q16x16_mul_s(state.rollAngle.target, qPi), q180);
    state.pitchAngle.target = q16x16_div_s(q16x16_mul_s(state.pitchAngle.target, qPi), q180);
  }
  // Convert yawSpeed to radians
  state.yawSpeed.target = q16x16_div_s(q16x16_mul_s(state.yawSpeed.target, qPi), q180);

  state.attitude = madgwickUpdate(state.attitude,
    gx, gy, gz,
    ax, ay, az,
    deltaT
  );

  // If angle mode, compute error terms and send to proportional controllers
  if (state.flightMode == FLIGHT_MODE_ANGLE) {
    // Use the small angle approximation <1, θ/2>
    state.targetAttitude.w = 0x00010000; // 1
    state.targetAttitude.x = q16x16_mul_s(qHalf, state.rollAngle.target);
    state.targetAttitude.y = q16x16_mul_s(qHalf, state.pitchAngle.target);
    state.targetAttitude.z = 0;

    // Compute error quaternion
    Quaternion tiltError = state.targetAttitude;
    Quaternion estimatedAttitudeConj = state.attitude;
    estimatedAttitudeConj.conjugate();
    tiltError *= estimatedAttitudeConj;
    if (tiltError.w < 0) {
      tiltError.flipSign();
    }

    // Estimate Euler angles from tilt quaternion
    // Theoretically we could skip this, but it maintains a physically meaningful
    // k_roll/pitchRate
    tiltError.x = q16x16_double_s(tiltError.x);
    tiltError.y = q16x16_double_s(tiltError.y);

    // Update pitch and roll target rate
    // NOTE: If additional smoothing is needed in angle mode, consider upgrading
    //       to a PI controller

    state.rollSpeed.target = CLAMP(
      q16x16_mul_s(k_rollRate, tiltError.x),
      rollSpeedMin, rollSpeedMax
    );
    state.pitchSpeed.target = CLAMP(
      q16x16_mul_s(k_pitchRate, tiltError.y),
      pitchSpeedMin, pitchSpeedMax
    );
  }
}


// Read IMU data, PIDs, motor mixing and output
void fastUpdate(const Q16x16 deltaT) {
  // Read IMU
  if (imu->update()) {
    imu->getGyroRad(gx, gy, gz);
    imu->getAccelG(ax, ay, az);
  }

  // TODO: Consider adding sensor bias correction/filtering

  state.rollSpeed.estimate = gx;
  state.pitchSpeed.estimate = gy;
  state.yawSpeed.estimate = gz;

  if (state.isArmed) {
    // PID update and create mixed motor signals
    motors.update(state, deltaT);
    // Write to motors
    // motors.loop();
  }
}
