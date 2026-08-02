# SELFDRIVING ARTBOX ROBOT
## Setup
1. Pull the devcontainer workspace. If you open the folder with vscode and have `ms-vscode-remote.remote-containers
` installed this should automatically ask you to start the folder as remote container.
```bash
git pull https://github.com/dudajulian/vscode_ros2_workspace.git
```
2. Once you have started the container create the `src/` folder and pull this repository into it. (All other ros packages should also be located in `src/`.)
```bash
cd /workspaces/vscode_ros2_workspace
mkdir src
cd src
git pull https://github.com/dudajulian/selfdriving_artbox.git
cd ..
```

4. All that is left is to install dependencies and build the workspace. The `vscode_ros2_workspace` does this with tasks. Open the vscode commands (`Ctrl+Shift+P`) and search for "Tasks: Run tasks". Then run the `install dependencies` task and after that the `build` task. 

5. To run the artbox robot open a terminal inside the devcontainer run:
```bash
cd /workspaces/vscode_ros2_workspace
source install/setup.bash
./src/selfdriving_artbox/artbox_robot/startup.bash
```
This starts a tmux session that runs the robot interface, rviz2 and keyboard controls for steering. You can switch between the windows of tmux by using `Ctrl+B` and then hitting the number of the window, or close it by hitting `D`.
Once tmux is closed, it still runs in the background. Make sure to kill it with `tmux kill-session -t artbox` or reenter the session with `tmux attach -t artbox`.


## Motor Interface

### Components

| Component | Model | Quantity |
|-----------|-------|:--------:|
| Microcontroller | NodeMCU (ESP8266) | 2 |
| Motor Driver | L298N | 1 |
| DC Motor | JGA25-370, 12 V | 2 |

The robot is driven by two independent motor controllers. Each NodeMCU controls one DC motor and its corresponding quadrature encoder via a shared L298N motor driver. Two microcontrollers are required because a single NodeMCU does not provide enough GPIOs for controlling both motors and reading both encoders simultaneously.

Connect the hardware according to the L298N and motor datasheets.

### Flashing the Firmware

The PlatformIO project located in `self_driving_artbox_differential_drive` must be flashed to **both** NodeMCUs.

During compilation, select the appropriate PlatformIO environment:

- `left` for the left motor controller
- `right` for the right motor controller

Each controller is assigned a static IP address.

| Controller | IP Address |
|-----------|------------|
| Left | `192.168.8.50` |
| Right | `192.168.8.51` |

### HTTP Interface

The motor controllers expose a simple HTTP interface.

Example:

**Left motor**

```text
http://192.168.8.50/motor?speed=700
```

**Right motor**

```text
http://192.168.8.51/motor?speed=700
```

The `speed` parameter accepts values between `-1023` and `1023`, where negative values drive the motor in reverse.

### Wi-Fi Configuration

The Wi-Fi credentials, gateway, and network configuration are defined in

```text
config.h
```

Update these settings before flashing the firmware.

---

## Low-Level Motor Controller

For development and debugging without ROS 2, the browser-based controller contained in `HTML_Controller` can be used.

After powering both NodeMCUs:

1. Open `HTML_Controller/index.html` in a web browser.
2. Control the robot using the **W**, **A**, **S**, and **D** keys.
3. Observe the current motor commands and encoder counts.

This interface is intended for testing the motor controllers independently of the ROS 2 stack.
