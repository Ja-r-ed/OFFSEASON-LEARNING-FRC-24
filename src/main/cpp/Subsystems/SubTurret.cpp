// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Subsystems/SubTurret.h"
#include <frc/smartdashboard/SmartDashboard.h>

SubTurret::SubTurret() {
    _turretmotor.SetConversionFactor(1.0/Gearing);
    _turretmotor.SetPIDFF(0.1, 0.0, 0.1, 0.0);
    frc::SmartDashboard::PutData("Turret Motor", (wpi::Sendable*)&_turretmotor);
}

// This method will be called once per scheduler run
void SubTurret::Periodic() {}

frc2::CommandPtr SubTurret::TurnTo(units:degree_t angle) {
    return RunOnce(
        [this, angle] {
            _turretmotor.SetPositionTarget(angle);
        }
    );
}

void SimulationPeriodic () {
    _turretSim.SetInputVoltage(_turretmotor.GetSimVoltage());

    _turretSim.Update(20_ms);

    _turretmotor.UpdateSimEncoder(_turretSim.GetAngularPosition(), _turretSim.GetAngularVelocity());
}