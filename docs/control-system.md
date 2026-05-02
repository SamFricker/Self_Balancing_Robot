# Control System

The robot uses a PID controller:

u(t) = Kp*e(t) + Ki∫e(t)dt + Kd*de/dt

## Strategy
1. Balance loop (angle → motor speed)
2. Velocity compensation (reduces drift)

## Notes
- Highly sensitive to tuning
- Noise affects derivative term

## To do
- the robot need properly tuning, establishing a P controler first then we can tune values accordingly to get smooth motion.
