# Control System

The robot uses a PID controller:
$$
u(t) = K_p e(t) + K_i \int e(t)\,dt + K_d \frac{de(t)}{dt}
$$
The core idea is that the proportional term grows with error (adjusting motors toward desired point), The integral term grows with accumulated error (adjusting motors such that error is not large for a long time), and the derivative term changes with the rate of error (making sure the motor doesnt overshoot)
## Strategy
1. Balance loop (angle → motor speed)
2. Velocity compensation (reduces drift)

## Notes
- Highly sensitive to tuning
- Noise affects derivative term

## To do
- the robot need properly tuning, establishing a P controler first then we can tune values accordingly to get smooth motion.
