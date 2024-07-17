// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Subsystems/SubFeeder.h"

#include <frc/smartdashboard/SmartDashboard.h>

SubFeeder::SubFeeder() = default;

// This method will be called once per scheduler run
void SubFeeder::Periodic() {
    frc::SmartDashboard::PutNumber("Feeder Power", _feedermotor.Get());
}

frc2::CommandPtr SubFeeder::Feed(){
    return StartEnd(
        [this]
        {
            _feedermotor.Set(1);
        },
        [this]
        {
            _feedermotor.Set(0);
        });
}
