#!/usr/bin/env bash
set -euo pipefail

PKG_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORKSPACE_ROOT="$(cd "$PKG_DIR/../../.." && pwd)"
SESSION="artbox"

# Load workspace environment once so package-share paths can be resolved.
set +u
source ~/.bashrc
source "$WORKSPACE_ROOT/install/setup.bash"
set -u

declare -A cmds=(
  [artbox_bringup]="ros2 launch artbox_robot artbox_bringup.launch.py"
#   [static_tf_cam]="ros2 run tf2_ros static_transform_publisher \
#     0.0 0.0 0.71 0 0.05 0 base_link camera_link"
  [rviz]="ros2 run rviz2 rviz2"
  [teleop_key]="ros2 run teleop_twist_keyboard teleop_twist_keyboard \
    --ros-args -p stamped:=true -p frame_id:=base_link \
    --remap cmd_vel:=/platform_velocity_controller/cmd_vel" 
  [teleop_joy]="ros2 launch teleop_twist_joy teleop-launch.py \
    joy_config:='xbox' publish_stamped_twist:=true\
    joy_vel:=/platform_velocity_controller/cmd_vel"
)

# Keep startup order aligned with all.launch.py.
startup_order=(
  artbox_bringup
  rviz
  teleop_key
  # teleop_joy
)

if tmux has-session -t "$SESSION" 2>/dev/null; then
  tmux kill-session -t "$SESSION"
fi

source_cmd="source ~/.bashrc; source '$WORKSPACE_ROOT/install/setup.bash'; cd '$WORKSPACE_ROOT'"

tmux new-session -d -s "$SESSION" -n "bash"
for name in "${startup_order[@]}"; do
  echo "Starting $name ..."
  tmux new-window -a -t "$SESSION" -n "$name"
  tmux send-keys -t "$SESSION:$name" \
    "bash -lc 'set +u; $source_cmd; set -u; ${cmds[$name]}'" C-m
  sleep 2
done

echo "All windows ready. Attach with: tmux attach-session -t $SESSION"
tmux attach-session -t "$SESSION"