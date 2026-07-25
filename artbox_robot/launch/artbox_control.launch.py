from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description() -> LaunchDescription:
    use_sim_time = LaunchConfiguration('use_sim_time')
    joy_config = LaunchConfiguration('joy_config')
    joy_dev = LaunchConfiguration('joy_dev')
    cmd_vel_topic = LaunchConfiguration('cmd_vel_topic')
    publish_stamped_twist = LaunchConfiguration('publish_stamped_twist')

    teleop_joy = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [
                    FindPackageShare('teleop_twist_joy'),
                    'launch',
                    'teleop-launch.py',
                ]
            )
        ),
        launch_arguments={
            'joy_config': joy_config,
            'joy_dev': joy_dev,
            'publish_stamped_twist': publish_stamped_twist,
            'use_sim_time': use_sim_time,
            'cmd_vel': cmd_vel_topic,
        }.items(),
    )

    teleop_keyboard = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [
                    FindPackageShare('teleop_twist_keyboard'),
                    'launch',
                    'teleop-launch.py',
                ]
            )
        ),
        launch_arguments={
            'joy_config': joy_config,
            'joy_dev': joy_dev,
            'publish_stamped_twist': publish_stamped_twist,
            'use_sim_time': use_sim_time,
            'cmd_vel': cmd_vel_topic,
        }.items(),
    )

    return LaunchDescription(
        [
            DeclareLaunchArgument(
                'use_sim_time',
                default_value='false',
                description='Use simulation clock if true.',
            ),
            DeclareLaunchArgument(
                'joy_config',
                default_value='xbox',
                description='Joystick config from teleop_twist_joy.',
            ),
            DeclareLaunchArgument(
                'joy_dev',
                default_value='/dev/input/js0',
                description='Joystick device path.',
            ),
            DeclareLaunchArgument(
                'publish_stamped_twist',
                default_value='true',
                description='Publish TwistStamped instead of Twist.',
            ),
            DeclareLaunchArgument(
                'cmd_vel_topic',
                default_value='platform_velocity_controller/cmd_vel',
                description='Output velocity command topic.',
            ),
            teleop_joy,
        ]
    )