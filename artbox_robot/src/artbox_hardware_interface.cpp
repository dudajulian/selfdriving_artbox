#include "artbox_robot/artbox_hardware_interface.hpp"

#include <algorithm>

#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "pluginlib/class_list_macros.hpp"
#include "rclcpp/rclcpp.hpp"

ArtboxHardwareInterface::CallbackReturn ArtboxHardwareInterface::on_init(
	const hardware_interface::HardwareInfo & info)
{
	if (hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS) {
		return CallbackReturn::ERROR;
	}

	const std::size_t joint_count = info_.joints.size();
	hw_positions_.assign(joint_count, 0.0);
	hw_velocities_.assign(joint_count, 0.0);
	hw_commands_.assign(joint_count, 0.0);

	for (const auto & joint : info_.joints) {
		const bool has_velocity_cmd = std::any_of(
			joint.command_interfaces.begin(), joint.command_interfaces.end(),
			[](const auto & iface) { return iface.name == hardware_interface::HW_IF_VELOCITY; });
		if (!has_velocity_cmd) {
			RCLCPP_ERROR(rclcpp::get_logger("ArtboxHardwareInterface"),
				"Joint '%s' is missing required velocity command interface.", joint.name.c_str());
			return CallbackReturn::ERROR;
		}
	}

	return CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface> ArtboxHardwareInterface::export_state_interfaces()
{
	std::vector<hardware_interface::StateInterface> state_interfaces;
	state_interfaces.reserve(info_.joints.size() * 2);

	for (std::size_t i = 0; i < info_.joints.size(); ++i) {
		state_interfaces.emplace_back(
			info_.joints[i].name, hardware_interface::HW_IF_POSITION, &hw_positions_[i]);
		state_interfaces.emplace_back(
			info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &hw_velocities_[i]);
	}

	return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> ArtboxHardwareInterface::export_command_interfaces()
{
	std::vector<hardware_interface::CommandInterface> command_interfaces;
	command_interfaces.reserve(info_.joints.size());

	for (std::size_t i = 0; i < info_.joints.size(); ++i) {
		command_interfaces.emplace_back(
			info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &hw_commands_[i]);
	}

	return command_interfaces;
}

ArtboxHardwareInterface::CallbackReturn ArtboxHardwareInterface::on_configure(
	const rclcpp_lifecycle::State &)
{
	std::fill(hw_positions_.begin(), hw_positions_.end(), 0.0);
	std::fill(hw_velocities_.begin(), hw_velocities_.end(), 0.0);
	std::fill(hw_commands_.begin(), hw_commands_.end(), 0.0);
	return CallbackReturn::SUCCESS;
}

ArtboxHardwareInterface::CallbackReturn ArtboxHardwareInterface::on_activate(
	const rclcpp_lifecycle::State &)
{
	is_active_ = true;
	return CallbackReturn::SUCCESS;
}

ArtboxHardwareInterface::CallbackReturn ArtboxHardwareInterface::on_deactivate(
	const rclcpp_lifecycle::State &)
{
	is_active_ = false;
	return CallbackReturn::SUCCESS;
}

hardware_interface::return_type ArtboxHardwareInterface::read(
	const rclcpp::Time &, const rclcpp::Duration & period)
{
	if (!is_active_) {
		return hardware_interface::return_type::OK;
	}

	const double dt = period.seconds();
	for (std::size_t i = 0; i < hw_velocities_.size(); ++i) {
		// Fake behavior: mirror commanded wheel velocity into measured state.
		hw_velocities_[i] = hw_commands_[i];
		hw_positions_[i] += hw_velocities_[i] * dt;
	}

	return hardware_interface::return_type::OK;
}

hardware_interface::return_type ArtboxHardwareInterface::write(
	const rclcpp::Time &, const rclcpp::Duration &)
{
	if (!is_active_) {
		return hardware_interface::return_type::OK;
	}

	// Placeholder write: in a real implementation this would send hw_commands_ to the motor driver.
	return hardware_interface::return_type::OK;
}

PLUGINLIB_EXPORT_CLASS(ArtboxHardwareInterface, hardware_interface::SystemInterface)
