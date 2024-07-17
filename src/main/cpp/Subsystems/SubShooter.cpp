// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Subsystems/SubShooter.h"
#include <frc/smartdashboard/SmartDashboard.h>


SubShooter::SubShooter() = default;

// This method will be called once per scheduler run
void SubShooter::Periodic() {
    frc::SmartDashboard::PutNumber("Shooter Speed", _shootermotor.Get());
}

frc2::CommandPtr SubShooter::SpinFlyWheel(){
    return StartEnd(
        [this]
        {
            _shootermotor.Set(0.8);
        },
        [this]
        {
            _shootermotor.Set(0);
        });
}
