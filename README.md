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
