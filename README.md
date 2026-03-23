# B.E.R. (Big Embedded Robot) - Autonomous GPU-Powered Vehicle

**B.E.R.** is a vision-based autonomous robot built on top of a modified off-the-shelf RC car (Maisto Rock Crawler Extreme). The project leverages the **NVIDIA Jetson TX2** to run a Deep Learning object detection algorithm (SSD + MobileNets v1) via **TensorFlow** to track individuals in real-time, while utilizing an **Arduino UNO R3** for deterministic, low-level hardware control and obstacle avoidance.

This repository contains the full **ROS (Robot Operating System)** software stack required to run the robot.

## System Architecture

The robot's software is built with a decoupled publisher-subscriber architecture in ROS across several core nodes:

*   **Vision & Perception (`jetson_csi_cam`):** Interfaces with the Jetson TX2's built-in CSI camera using OpenCV to capture high-resolution images.
*   **Deep Learning Object Detection (`learning_image_transport`):** Subscribes to the raw camera feed and runs a pre-trained **SSD + MobileNets** Convolutional Neural Network (CNN). This node identifies objects (such as people), calculates bounding boxes, and executes a tracking algorithm to keep the target centered in the frame.
*   **Locomotion Intelligence (`motor_control`):** Subscribes to the object detection data (`/objects`), calculates the required positional adjustment, and publishes motion commands (`/car_direction`) to follow or yield to the tracked target.
*   **Real-Time Hardware Bridge (`rosserial` & `arduino_sketches`):** Because Linux on the TX2 is non-deterministic, an Arduino UNO R3 is used for critical real-time operations. The Arduino receives ROS motion commands over USB serial and converts them to PWM signals for the motor controller. It also independently reads data from an array of three SHARP IR sensors (front, left, right) to execute emergency stops if an obstacle is within 80cm.

## Hardware Bill of Materials (BOM)

*   **Compute:** NVIDIA Jetson TX2 Developer Kit
*   **Microcontroller:** Arduino UNO R3
*   **Chassis:** Maisto Rock Crawler Extreme RC Car
*   **Motor Controller:** Pololu Dual MC33926 Motor Driver Carrier
*   **Sensors:** 3x SHARP GP2Y0A21YK0F IR Distance Sensors (10-80cm)
*   **Camera:** Jetson TX2 Built-in Monocular CSI Camera
*   **Power (Compute):** Turnigy 5000 mAh 3S 20C Lipo Pack
*   **Power (Motors):** E-flite 7.4V 800mAh 2-cell Lipo battery pack

## Installation & Setup

1.  **Clone the repository into your ROS workspace:**
    ```bash
    cd ~/catkin_ws/src
    git clone https://github.com/RoboEvangelist/BER_Robot.git gpu-robot
    ```
2.  **Build the workspace:**
    ```bash
    cd ~/catkin_ws
    catkin_make
    source devel/setup.bash
    ```
3.  **Flash the Arduino:**
    Upload the `src/arduino_sketches/IrRanger/IrRanger.ino` sketch to your connected Arduino UNO. Ensure the Arduino is connected to the TX2 via USB (`/dev/ttyACM0`).

## Running the Robot

The easiest way to start the entire robot stack (Camera, Object Detection, Motor Control, and Arduino Serial Bridge) is to use the master launch file:

```bash
roslaunch motor_control launch_everything.launch
```

### Running Components Individually (For Debugging)

If you need to test specific components or run them individually, use the following commands:

**1. Start the Camera:**
```bash
roslaunch jetson_csi_cam jetson_csi_cam.launch width:=960 height:=540 fps:=30
```

**2. Image Transport & Viewing:**
```bash
# Compress the raw image feed
rosrun image_transport republish raw in:=/csi_cam/image_raw compressed out:=/csi_cam/image_raw_compressed

# View the compressed feed
rosrun image_view image_view image:=/csi_cam/image_raw_compressed _image_transport:=compressed
```

**3. Start Object Detection (TensorFlow/GPU):**
```bash
rosrun learning_image_transport object_detection_node.py
```

**4. Start the Motor Controller Calculation Node:**
```bash
rosrun motor_control motor_control
```

**5. Start the Arduino Serial Bridge (IR Sensors & Motor Output):**
```bash
rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0
```
