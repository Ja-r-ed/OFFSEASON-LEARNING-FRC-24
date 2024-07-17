// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include <rev/CANSparkMax.h>
#include <frc2/command/Commands.h>
#include "Constants.h"

class SubFeeder : public frc2::SubsystemBase {
 public:
  SubFeeder();
  static SubFeeder& GetInstance() {static SubFeeder inst; return inst;} 

  /**
   * Will be called periodically whenever the CommandScheduler runs.
   */
  void Periodic() override;

 private:
  // Components (e.g. motor controllers and sensors) should generally be
  // declared private and exposed only through public methods.
  rev::CANSparkMax _feedermotor{canid::feedermotor, rev::CANSparkMax::MotorType::kBrushless};
};
