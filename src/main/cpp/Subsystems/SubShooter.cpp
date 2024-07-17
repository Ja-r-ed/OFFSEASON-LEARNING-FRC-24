// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Subsystems/SubShooter.h"
#include <frc/smartdashboard/SmartDashboard.h>


SubShooter::SubShooter() {
    frc::SmartDashboard::PutData("Shooter Motor", (wpi::Sendable*)&_shootermotor);
};

// This method will be called once per scheduler run
void SubShooter::Periodic() {
    frc::SmartDashboard::PutNumber("Shooter Speed", _shootermotor.Get());
}

frc2::CommandPtr SubShooter::SpinFlyWheel(){
    return StartEnd(
        [this]
        {
            _shootermotor.SetVelocityTarget(1000_rpm);
        },
        [this]
        {
            _shootermotor.Set(0);
        });
}

void SubShooter::SimulationPeriodic() {
    auto volts = _shootermotor.GetSimVoltage();

    _flywheelSim.SetInputVoltage(volts);
    _flywheelSim.Update(20_ms);

    auto velocity = _flywheelSim.GetAngularVelocity();

    _shootermotor.UpdateSimEncoder(0_deg, velocity);
}