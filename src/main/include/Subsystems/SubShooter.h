// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include "utilities/ICSparkMax.h"
#include <frc2/command/Commands.h>
#include "Constants.h"

#include <frc/simulation/FlywheelSim.h>
#include <frc/system/plant/DCMotor.h>
#include <units/mass.h>

class SubShooter : public frc2::SubsystemBase {
 public:
 static SubShooter& GetInstance() {static SubShooter inst; return inst;}
  SubShooter();

  /**
   * Will be called periodically whenever the CommandScheduler runs.
   */
  void Periodic() override;
  void SimulationPeriodic() override; 
  bool AtTarget();

  frc2::CommandPtr SpinFlyWheel();

 private:
  // Components (e.g. motor controllers and sensors) should generally be
  // declared private and exposed only through public methods.
  ICSparkMax _shootermotor{canid::shootermotor};

  // Simulation
  static constexpr double Gearing = 1.0;
  static constexpr units::kilogram_square_meter_t FLYWHEEL_MASS = 0.05_kg_sq_m;
  frc::sim::FlywheelSim _flywheelSim{frc::DCMotor::NEO(), Gearing, FLYWHEEL_MASS};
};

