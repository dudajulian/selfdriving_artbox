from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, TimerAction
from launch.substitutions import Command, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare


def generate_launch_description() -> LaunchDescription:
	use_sim_time = LaunchConfiguration('use_sim_time')

	robot_description_content = Command(
		[
			'xacro ',
			PathJoinSubstitution(
				[FindPackageShare('artbox_robot'), 'urdf', 'artbox.urdf.xacro']
			),
		]
	)
	robot_description = {
		'robot_description': ParameterValue(robot_description_content, value_type=str)
	}

	controllers_file = PathJoinSubstitution(
		[FindPackageShare('artbox_robot'), 'config', 'ros2_controllers.yaml']
	)

	robot_state_publisher = Node(
		package='robot_state_publisher',
		executable='robot_state_publisher',
		output='screen',
		parameters=[robot_description, {'use_sim_time': use_sim_time}],
	)

	control_node = Node(
		package='controller_manager',
		executable='ros2_control_node',
		output='screen',
		parameters=[robot_description, controllers_file],
	)

	joint_state_broadcaster_spawner = Node(
		package='controller_manager',
		executable='spawner',
		arguments=['joint_state_broadcaster', '--controller-manager', '/controller_manager'],
		output='screen',
	)

	platform_velocity_controller_spawner = Node(
		package='controller_manager',
		executable='spawner',
		arguments=['platform_velocity_controller', '--controller-manager', '/controller_manager'],
		output='screen',
	)

	timed_joint_state_broadcaster_spawner = TimerAction(
		period=2.0,
		actions=[joint_state_broadcaster_spawner],
	)

	timed_platform_velocity_controller_spawner = TimerAction(
		period=4.0,
		actions=[platform_velocity_controller_spawner],
	)

	return LaunchDescription(
		[
			DeclareLaunchArgument(
				'use_sim_time',
				default_value='false',
				description='Use simulation clock if true.',
			),
			robot_state_publisher,
			control_node,
			timed_joint_state_broadcaster_spawner,
			timed_platform_velocity_controller_spawner,
		]
	)
