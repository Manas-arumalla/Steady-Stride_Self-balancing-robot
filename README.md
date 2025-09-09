# Steady-Stride — Self-Balancing Robot (Single Inverted Pendulum + Robotic Arm)

**Steady-Stride** is a two-wheeled, inverted-pendulum self-balancing robot prototype with an integrated pan-tilt robotic arm and four-claw gripper. The platform was designed for robustness on *uneven field-like terrain* . This repository contains Arduino control code, MATLAB modelling & LQR tuning scripts, and documentation of mechanical/electrical design and experiments.

---

## Highlights
- Prototype integrates a **single inverted pendulum** balancing body + a **pan-tilt robotic arm** with a 4-claw gripper.   
- Control work includes **LQR** and **pole-placement** simulations, plus on-hardware PID/LQR style controllers implemented on Arduino (MPU/MPU6050 and BNO055 IMU examples included).   
- Fabrication variants: Mk3 (3D-print) and Mk7 (wood/laser cut sheet) — Mk7 chosen for cost/weight tradeoff. 

---

## Quick demo (what to expect in this repo)
- `arduino/` — Arduino sketches (MPU6050-based PID and BNO055/LQR-based sketches you provided)
- `matlab/` — Modelling & simulation scripts (e.g. `angle_only_lqr_ga.m`) used to compute LQR gains and to run GA/Q-weight optimization.
- `docs/` — Report (D2.5) and slides (PPT) used as project documentation. 
- `assets/` — Suggested locations for photos, plots and block diagrams (see list below)

---

## System overview

### Mechanical
- 5-level compact chassis (300 × 100 mm footprint) that stacks electronics, battery and actuators to place the CG in an optimal location for balancing. Two large torque wheel servos at the bottom drive the wheels; pan/tilt servos for the arm at the top. Mk7 wood-sheet variant reduces weight & cost compared to 3D-printed Mk3. 

### Electronics & Sensors
- **Controller:** Arduino Mega 2560 (primary) + Arduino Nano in console.  
- **IMU(s):** MPU6050 (for body & arm) and BNO055 examples are included in sketches.  
- **Wheel encoders:** OE37 Hall-effect encoders for wheel odometry.  
- **Motor driver:** TA6586 (two-motor driver) used for wheel motors.  
- **Comms:** HC-05 Bluetooth for console ↔ robot. 

### Control
- Linearized state-space model derived via Euler–Lagrange → 4-state model for the single inverted pendulum. LQR and pole-placement controllers were simulated and compared. LQR provided superior angle stabilization (faster settling) in simulation and motivated the on-robot controller design. 

---

## Key design & simulation results (use these exact numbers in your paper/README)
- Example LQR weighting used in simulation: `Q = diag([150, 50, 200, 10]), R = 0.01`. Resulting LQR gain (MATLAB):  
  `K_LQR ≈ [-12.2474, -15.0435, 112.5687, 10.5238]`. 
- In simulation the LQR controller produced significantly faster settling of the pendulum angle vs pole-placement; outer pendulum settling time was ~3× faster with LQR in the studied cases. 
- Prototype testing notes: single inverted pendulum prototype was tested on flat surfaces; several RPM/gear variants were tested showing instability at high wheel RPMs and better behavior at lower RPMs with stand-deployment heuristics. Tests including the robotic arm showed vibrations at higher motor speeds. (See test videos/notes in presentation.) 

---

## Files & code (what's in this repo)
- `arduino/lqr_bno055.ino` — LQR style controller that reads BNO055 orientation and uses wheel encoder feedback to compute wheel rate; published control law `u = -K x`. (Adapted from your BNO055 + encoder sketch.)
- `arduino/pid_mpu6050.ino` — MPU6050 + PID loop implementation used in initial experiments (your PID sketch).  
- `matlab/angle_only_lqr_ga.m` — plant parameters, GA optimization of Q weight (q_phi) and LQR simulation for angle-only control. (Generates φ(t) response plot.) 
- `docs/D2.5_Report.pdf` and `docs/Presentation.pdf` — full report and slides. 

---

## How to reproduce the plots (quick)
1. Open `matlab/angle_only_lqr_ga.m` in MATLAB (this is your GA + LQR script).
2. Run the script — it computes `K` via `lqr(...)` and simulates a small initial tilt (e.g. `x0 = [0;0;0.1;0]`), then plots `φ(t)` (pendulum angle vs time). The script also prints the optimized `q_phi`. 
3. Save the produced figure as `assets/plots/lqr_phi_response.png` and commit it.

*(Tip: also export `phi` vs `time`, `control effort u` vs `time`, and a comparative plot of PID vs LQR vs Pole-placement for README.)*

---

## Suggested images / assets to include (filenames)

- ![Prototype front view — Steady-Stride](assets/photos/prototype_front.jpg "front view of the prototype")
 
- `assets/photos/prototype_side.jpg` — side view with arm and electronics visible.   
- `assets/diagrams/block_diagram.png` — IMU → controller → motor driver → wheels loop. (Generate from draw.io)   
- `assets/diagrams/electrical_schematic.png` — integrated circuit diagram from the PPT/report.   
- `assets/plots/lqr_phi_response.png` — MATLAB LQR pendulum angle vs time (from `angle_only_lqr_ga.m`).   
- `assets/plots/step_response_compare.png` — comparison: LQR vs Pole-placement step responses (from report figures).   
- `assets/photos/gripper_closeup.jpg` — 4-claw gripper picture. 

Use these image paths in the README so GitHub renders them.

---

## How to run the Arduino code (short)
1. Open the relevant Arduino sketch in the `arduino/` folder.
2. Install libraries:
   - `MPU6050_light` (for MPU sketch)
   - `Adafruit_BNO055`, `Adafruit_Sensor` (for BNO055 sketch)
   - `PWMServo` / `Servo` as required
3. Check wiring: follow the integrated electrical diagram in `docs/Presentation.pdf`. 
4. Upload at 115200 baud. Power the platform with the recommended battery (14.8 V Li-Ion + buck converter to 7.4 V/5 V as used in prototype). 

---

## Tests performed / validation
- MATLAB simulations: Pole-placement vs LQR comparison, LQR convergence & settling time estimation (documented in the report).   
- Hardware tests: prototypes tested at several wheel RPMs with/without the arm; instability observed at high RPMs (videos recorded in the PPT). 

---

## Future work & extensions
- Extend to **double inverted pendulum** (DIP) and design robust controllers for uneven/swampy terrains.   
- Add sensor fusion (Kalman filter) between accelerometer & gyro for better angle estimation.  
- Add autonomous computer vision for target detection & harvesting at the arm end-effector. 

---

## References & credits
- [1] J. Velagic, I. Kovac, A. Panjevic, and A. Osmanovic, “Design and Control of Two-Wheeled and Self Balancing Mobile Robot,” in Proceedings Elmar - International Symposium Electronics in Marine, Croatian Society Electronics in Marine - ELMAR, Sep. 2021, pp. 77–82. doi: 10.1109/ELMAR52657.2021.9550938.
- [2] Institute of Electrical and Electronics Engineers Singapore Section Control Systems Chapter, Annual IEEE Computer Conference, IEEE Conference on Decision and Control, IEEE International Conference on Control and Automation 10 2013.06.12-14 Hangzhou, and IEEE ICCA 10 2013.06.12-14 Hangzhou, 10th IEEE International Conference on Control and Automation (ICCA), 2013 12-14 June 2013, Hangzhou, China.
- [3] Strategic Technology (IFOST), 2010 International Forum on : date, 13-15 Oct. 2010. IEEE, 2010.
- [4] A. T. Azar, H. H. Ammar, M. H. Barakat, M. A. Saleh, and M. A. Abdelwahed, “Self-balancing Robot Modeling and Control Using Two Degree of Freedom PID Controller,” in Advances in Intelligent Systems and Computing, Springer Verlag, 2019, pp. 64–76. doi: 10.1007/978-3-319-99010-1_6.
-  [5] IEEE Robotics and Automation Society, Annual IEEE Computer Conference, IEEE International Conference on Robotics and Biomimetics 2013.12.12-14 Shenzhen, and ROBIO 2013.12.12-14 Shenzhen, IEEE International Conference on Robotics and Biomimetics (ROBIO), 2013 Shenzhen, China, 12-14 Dec. 2013.
-  [6] E. Ahmad, A. Ur Rehman, O. Khan, M. Haseeb, and N. Ali, “Backstepping control design for twowheeled self balancing robot,” in Proceedings - 2018, IEEE 1st International Conference on Power, Energy and Smart Grid, ICPESG 2018, Institute of Electrical and Electronics Engineers Inc., Jun. 2018, pp. 1–6. doi: 10.1109/ICPESG.2018.8384494. 

---
