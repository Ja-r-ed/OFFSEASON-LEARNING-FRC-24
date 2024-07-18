// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/simulation/DCMotorSim.h>
#include <frc/system/plant/DCMotor.h>
#include <units/angle.h>
#include <units/moment_of_inertia.h>

#include <frc2/command/SubsystemBase.h>
#include <frc2/command/Commands.h>
#include "utilities/ICSparkMax.h"
#include "Constants.h"

class SubTurret : public frc2::SubsystemBase {
 public:
  SubTurret();
  static SubTurret& GetInstance() {
    static SubTurret inst;
    return inst;
  }

  /**
   * Will be called periodically whenever the CommandScheduler runs.
   */
  void Periodic() override;
  void SimulationPeriodic() override;
  void AtTarget();

  frc2::CommandPtr TurnTo{units::degree_t, angle};

 private:
  // Components (e.g. motor controllers and sensors) should generally be
  // declared private and exposed only through public methods.
  ICSparkMax _turretmotor{canid::turretmotor};

  static constexpr double Gearing = 20;
  static constexpr inline auto MOI = 0.005_kg_sq_m;
};
