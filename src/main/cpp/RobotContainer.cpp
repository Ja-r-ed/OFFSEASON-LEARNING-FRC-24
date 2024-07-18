// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"
#include "Subsystems/SubFeeder.h"
#include <frc2/command/Commands.h>
#include "Subsystems/SubShooter.h"
#include "Subsystems/SubTurret.h"

RobotContainer::RobotContainer() {
  ConfigureBindings();
}

void RobotContainer::ConfigureBindings() {
  _controller.A().WhileTrue(
    SubShooter::GetInstance().SpinFlyWheel()
    .AlongWith(frc2::cmd::Wait(1_s))
    .AndThen(SubFeeder::GetInstance().Feed())
    );

  _controller.X().OnTrue(SubTurret::GetInstance().TurnTo(30_deg));
  _controller.Y().OnTrue(SubTurret::GetInstance().TurnTo(0_deg));
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
  return frc2::cmd::Print("No autonomous command configured");
}
