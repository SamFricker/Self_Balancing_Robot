// Include necessary libraries
#include <Wire.h>
#include <MPU6050.h>
#include <Stepper.h>

#define PI_OVER_180 0.0174533  // Precompute pi/180

float alpha = 0.95; // Complementary filter weight variable

// set intial pitch and time to zero
float pitch = 0; // zero is a placeholder value whilst we dont have IMU data
unsigned long prev_time = 0;

// Set limits for stepper motors
const int maxSpeed = 100;  // rps
const int stepsPerRevolution = 200;

// Initialise stepper motor pin layout and imu
Stepper left(stepsPerRevolution, 8, 9, 10, 11);
Stepper right(stepsPerRevolution, 2, 3, 4, 5);
MPU6050 imu;

// begin serial monitor and start timer
void setup() {
  Serial.begin(9600);
  Wire.begin();
  imu.initialize();
  prev_time = millis();
}

// start main operating loop
void loop() {

  // on every loop, get the current time and calculate the time difference of the last loop dt.
  unsigned long curr_time = millis();
  float dt = (curr_time - prev_time) * 0.001; // in ms
  prev_time = curr_time;

  // Get IMU data
  int16_t ax, ay, az, gx, gy, gz;
  imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // --- COMPLEMENTARY FILTER ---

  // calculate the accelerometer pitch value and the anguular velocity in the pitch direction
  float accPitch = -atan2(ax, az);
  float omegaY = gy * 0.007633; // use the given conversion factor from IMU manufacturers datasheet

  // Calculate the pitch angle using complementary filter
  pitch = (alpha * (pitch + omegaY * dt * PI_OVER_180) + (1 - alpha) * accPitch);

  //int pitchDEG = abs((pitch * 57.2958) - 3);

  // --- PITCH PID CONTROLLER ---

  // Set constants
  float Kp = 17, Ki = 3, Kd = 10; // chosen randomly, to be determined experimentally
  float maxPID = 1500, targetPitch = 0;
  float intTerm = 0, previousPitch = 0;

  // calculate error in pitch angle
  float error = targetPitch - pitch;

  // calculate integral and derivative terms iteratively
  intTerm += error * dt;
  float drvTerm = (previousPitch - pitch) / dt;
  previousPitch = pitch;

  // Calculate PID value
  float PID = (Kp * error) + (Ki * intTerm) + (Kd * drvTerm);

  // Limit PID to its maximum value
  if (PID > maxPID){
    PID = maxPID;
  } else if(PID < -maxPID){
    PID = -maxPID;
  }

  // Serial.print("PID = "); // Sanity check
  // Serial.println(PID);

  // Set threshold PID value for motors to prevent vibrations and jittering
  if (PID > 50 || PID < -50){
    right.setSpeed(abs(PID));
    left.setSpeed(abs(PID));
  }

  // set the step count to 5 (steps per loop)
  int stepCount = 5;

  // set direction of the motors
  if(PID != 0){
    if (PID > 0) {
      right.step(-stepCount);
      left.step(-stepCount);
    } else if (PID < 0) {
      right.step(stepCount);
      left.step(stepCount);
    }
  }
}
