// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"
#include <pathplanner/lib/commands/PathPlannerAuto.h>
#include <frc2/command/Commands.h>
#include "subsystems/SubShooter.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <subsystems/SubDrivebase.h>
#include "subsystems/SubIntake.h"
#include "subsystems/SubLED.h"
#include <pathplanner/lib/auto/NamedCommands.h>
#include "RobotContainer.h"
#include "subsystems/SubClimber.h"
#include <frc2/command/button/CommandXboxController.h>
#include "commands/UniversalCommands.h"
#include "utilities/BotVars.h"
#include "subsystems/SubVision.h"
#include "commands/VisionCommands.h"
#include "subsystems/SubArm.h"
#include "utilities/POVHelper.h"

RobotContainer::RobotContainer() {
  frc::SmartDashboard::PutData("Command Scheduler", &frc2::CommandScheduler::GetInstance());

  pathplanner::NamedCommands::registerCommand("Intake", SubIntake::GetInstance().Intake());
  pathplanner::NamedCommands::registerCommand(
      "VisionAlign", cmd::VisionAlignToSpeaker(_driverController)
                         .Until([] {
                           auto Cameraresult =
                               SubVision::GetInstance().GetSpecificTagYaw(SubVision::SPEAKER);
                           if (Cameraresult.has_value()) {
                             if (units::math::abs(Cameraresult.value_or(0_deg)) < 2_deg) {
                               return true;
                             } else {
                               return false;
                             }
                           }
                           return false;
                         })
                         .WithTimeout(2_s));
  pathplanner::NamedCommands::registerCommand("StopIntakeSpinning",
                                              SubIntake::GetInstance().StopSpinningIntake());
  pathplanner::NamedCommands::registerCommand("StartShooter",
                                              SubShooter::GetInstance().StartShooter());
  pathplanner::NamedCommands::registerCommand("RetractInt ake",
                                              SubIntake::GetInstance().CommandRetractIntake());
  pathplanner::NamedCommands::registerCommand("ShootNote",  
    SubShooter::GetInstance().ShootSequence());
  pathplanner::NamedCommands::registerCommand("StopShooter",
                                              SubShooter::GetInstance().StopShooterCommand());
  pathplanner::NamedCommands::registerCommand("FeedNote", SubArm::GetInstance().FeedNote());
  // pathplanner::NamedCommands::registerCommand("ShootFullSequence",
  // cmd::ShootFullSequence().WithTimeout(0.5_s));
  pathplanner::NamedCommands::registerCommand("AutoShootFullSequence",
                                              cmd::AutoShootFullSequence().WithTimeout(0.5_s));
  pathplanner::NamedCommands::registerCommand("StoreNote", SubArm::GetInstance().StoreNote());
  pathplanner::NamedCommands::registerCommand("ShooterChangePosFar",
                                              SubShooter::GetInstance().ShooterChangePosFar());
  pathplanner::NamedCommands::registerCommand("ShooterChangePosClose",
                                              SubShooter::GetInstance().ShooterChangePosClose());
  pathplanner::NamedCommands::registerCommand("StopFeeder", SubShooter::GetInstance().StopFeeder());
  pathplanner::NamedCommands::registerCommand("OuttakeEndEffector", SubArm::GetInstance().Outtake());
  // pathplanner::NamedCommands::registerCommand("Shoot3_s",
  // cmd::ShootFullSequenceWithVision(controller).WithTimeout(3_s));

  SubArm::GetInstance();
  SubDrivebase::GetInstance();
  SubIntake::GetInstance();
  SubVision::GetInstance();

  SubDrivebase::GetInstance().SetDefaultCommand(
      SubDrivebase::GetInstance().JoystickDrive(_driverController, false));
  ConfigureBindings();
  _delayChooser.AddOption("0 Seconds", 0);
  _delayChooser.AddOption("1 Seconds", 1);
  _delayChooser.AddOption("2 Seconds", 2);
  _delayChooser.AddOption("3 Seconds", 3);
  _delayChooser.AddOption("10 Seconds", 10);
  frc::SmartDashboard::PutData("Delay By", &_delayChooser);

  _autoChooser.AddOption("A10", "A10");
  _autoChooser.AddOption("Test Path", "Test Path");
  _autoChooser.AddOption("M4", "M4");
  _autoChooser.AddOption("M4 No Far Note", "M4 No Far Note");
  _autoChooser.AddOption("S1 (C5 first)", "S1 (C5 first)");
  _autoChooser.AddOption("S1 (C4 first)", "S1 (C4 first)");
  _autoChooser.AddOption("A2", "A2");
  _autoChooser.AddOption("Alliance collect path", "Alliance collect path");
  _autoChooser.AddOption("Nothing", "Nothing");
  _autoChooser.AddOption("A2", "A2");
  _autoChooser.AddOption("SUPRISE", "SUPRISE");
  _autoChooser.SetDefaultOption("Nothing", "Nothing");
  _autoChooser.SetDefaultOption("Move Back", "Move Back");
  _autoChooser.SetDefaultOption("AmpSide Preload Back up", "AmpSide Preload Back up");
  _autoChooser.SetDefaultOption("Source side c3 rush", "S C3 rush");
  frc::SmartDashboard::PutData("Chosen Path", &_autoChooser);

  _compressor.EnableAnalog(80_psi, 120_psi);
}

void RobotContainer::ConfigureBindings() {
  // Single controller controls


// not both bumper combination

  !_driverController.LeftBumper() && !_driverController.RightBumper() && _driverController.LeftTrigger().WhileTrue(cmd::IntakefullSequence());
  !_driverController.LeftBumper() && !_driverController.RightBumper() && _driverController.X().OnTrue(SubClimber::GetInstance().ClimberManualDrive(0.5));
  !_driverController.LeftBumper() && !_driverController.RightBumper() && _driverController.X().OnFalse(SubClimber::GetInstance().ClimberManualDrive(0));
  !_driverController.RightBumper() && _driverController.LeftBumper().WhileTrue(cmd::VisionAlignToSpeaker(_driverController));
  !_driverController.LeftBumper() && !_driverController.RightBumper() && _driverController.B().WhileTrue(cmd::PassNote());
  !_driverController.LeftBumper() && !_driverController.RightBumper() && _driverController.RightTrigger().WhileTrue(cmd::ShootSpeakerOrArm());
  !_driverController.LeftBumper() && !_driverController.RightBumper() && _driverController.Y().OnTrue(cmd::ArmToAmpPos());
  !_driverController.LeftBumper() && !_driverController.RightBumper() && _driverController.Y().OnFalse(cmd::ArmToStow());
  !_driverController.LeftBumper() && !_driverController.RightBumper() && _driverController.A().OnTrue(cmd::PrepareToShoot());
  !_driverController.LeftBumper() && !_driverController.RightBumper() && _driverController.Start().WhileTrue(cmd::OuttakeNote());
  
  !_driverController.LeftBumper() && !_driverController.RightBumper() && POVHelper::Left(&_driverController) && (POVHelper::Down(&_driverController)).OnTrue(SubDrivebase::GetInstance().ResetGyroCmd());
  !_driverController.LeftBumper() && !_driverController.RightBumper() && POVHelper::Up(&_driverController).OnTrue(SubClimber::GetInstance().ClimberPosition(0.467_m));
  !_driverController.LeftBumper() && !_driverController.RightBumper() && POVHelper::Down(&_driverController).OnTrue(SubClimber::GetInstance().ClimberPosition(0.001_m));
  !_driverController.LeftBumper() && !_driverController.RightBumper() && POVHelper::Left(&_driverController).ToggleOnTrue(SubIntake::GetInstance().ToggleExtendIntake());
  !_driverController.LeftBumper() && !_driverController.RightBumper() && POVHelper::Right(&_driverController).OnTrue(SubClimber::GetInstance().ClimberPosition(SubClimber::_ClimberPosStow));
\
// both bumper combination

  _driverController.LeftBumper() && _driverController.RightBumper() && _operatorController.RightTrigger().OnTrue(SubShooter::GetInstance().ShooterChangePosFar());
  _driverController.LeftBumper() && _driverController.RightBumper() && _operatorController.LeftTrigger().OnTrue(SubShooter::GetInstance().ShooterChangePosClose());



  //joystick related
  // frc2::Trigger(frc2::CommandScheduler::GetInstance().GetDefaultButtonLoop(), [=, this] {
  //   return (_driverController.GetLeftY() < -0.2 || _driverController.GetLeftY() > 0.2) &&
  //   !(_driverController.GetRightY() < -0.2 || _driverController.GetRightY() > 0.2); 
  // }).WhileTrue(SubClimber::GetInstance().ClimberJoystickDriveLeft(_driverController));

  // frc2::Trigger(frc2::CommandScheduler::GetInstance().GetDefaultButtonLoop(), [=, this] {
  //   return (_driverController.GetRightY() < -0.2 || _driverController.GetRightY() > 0.2) 
  //   !(_driverController.GetLeftY() < -0.2 || _driverController.GetLeftY() > 0.2);
  // }).WhileTrue(SubClimber::GetInstance().ClimberJoystickDriveRight(_driverController));

  // frc2::Trigger(frc2::CommandScheduler::GetInstance().GetDefaultButtonLoop(), [=, this] {
  //   return (_driverController.GetRightY() < -0.2 || _driverController.GetRightY() > 0.2) &&
  //          (_driverController.GetLeftY() < -0.2 || _driverController.GetLeftY() > 0.2 ); 
  // }).WhileTrue(SubClimber::GetInstance().ClimberJoystickDrive(_driverController));


  // SOFTWARE CONTROLS

  //   _driverController.Start().OnTrue(SubDrivebase::GetInstance().ResetGyroCmd()); //working

  //   _driverController.LeftBumper().WhileTrue(cmd::ArmToAmpPos()); //working
  //   _driverController.LeftBumper().OnFalse(cmd::ArmToStow()); //working
  //   _driverController.LeftTrigger().WhileTrue(cmd::IntakefullSequence());
  // //  _driverController.B().OnTrue(SubIntake::GetInstance().ExtendIntake());
  //   _driverController.X().WhileTrue(cmd::OuttakeNote());

  //   _driverController.RightBumper().WhileTrue(cmd::ShootFullSequenceWithoutVision());
  //   _driverController.RightTrigger().WhileTrue(cmd::ShootFullSequenceWithVision());

  //   _driverController.A().OnTrue(cmd::VisionRotateToZero());
  //   _driverController.B().OnTrue(SubIntake::GetInstance().ExtendIntake());
  //   _driverController.Y().OnTrue(cmd::PrepareToShoot());
  //   //POVHelper::Up(&_driverController).WhileTrue(SubShooter::GetInstance().StartFeeder());

  //   _operatorController.RightTrigger().WhileTrue(cmd::ShootFullSequenceWithVision());
  //   _operatorController.RightBumper().OnFalse(SubShooter::GetInstance().ShooterChangePosFar());

  //   _operatorController.LeftBumper().OnFalse(SubShooter::GetInstance().ShooterChangePosClose());
  // _driverController.LeftTrigger().WhileTrue(cmd::IntakefullSequence());

  //   _operatorController.X().OnTrue(SubClimber::GetInstance().ClimberPosition(0.625_m));
  //   _operatorController.Y().OnTrue(SubClimber::GetInstance().ClimberPosition(0.02_m));
  //   _operatorController.A().WhileTrue(SubShooter::GetInstance().StartShooter());

  //   _operatorController.Start().WhileTrue(SubClimber::GetInstance().ClimberAutoReset());

  //   POVHelper::Up(&_operatorController).OnTrue(SubClimber::GetInstance().ClimberManualDrive(0.5));
  //   POVHelper::Up(&_operatorController).OnFalse(SubClimber::GetInstance().ClimberManualDrive(0));
  // _driverController.X.OnTrue(SubClimber::GetInstance().ClimberManualDrive(0.5));
  // _driverController.X.OnFalse(SubClimber::GetInstance().ClimberManualDrive(0));

  // DRIVER CONTROLS

  // _driverController.Y().OnTrue(SubDrivebase::GetInstance().ResetGyroCmd());
  // // _driverController.LeftTrigger().WhileTrue(/*Align2Stage*/);
  // // _driverController.LeftBumper().WhileTrue(/*IntakeFromSource*/);
  
  // _driverController.LeftBumper().WhileTrue(cmd::VisionAlignToSpeaker(_driverController));
  // _driverController.LeftBumper().WhileTrue(cmd::IntakefullSequence().AndThen(Rumble(1, 0.3_s)));
  // _driverController.LeftTrigger().WhileTrue(cmd::IntakefullSequence().AndThen(Rumble(1, 0.3_s)));
  // _driverController.X().WhileTrue(SubArm::GetInstance().Outtake());
  // _driverController.A().WhileTrue(cmd::ShootSpeakerOrArm());

  // _driverController.RightBumper().WhileTrue(/*Align2Amp*/);

  // // _operatorController.Start().WhileTrue(cmd::OuttakeNote());

  // POVHelper::Up(&_driverController).WhileTrue(cmd::PassNote());
  // _operatorController.LeftBumper().OnTrue(SubShooter::GetInstance().ShooterChangePosClose());
  // _operatorController.RightBumper().OnTrue(SubShooter::GetInstance().ShooterChangePosFar());
  // _driverController.RightTrigger().WhileTrue(cmd::ShootSpeakerOrArm());

  // _operatorController.X().WhileTrue(cmd::ShootIntoAmp());
  // _operatorController.Back().WhileTrue(SubClimber::GetInstance().ClimberAutoReset());
  // // amp
  // _driverController.B().OnTrue(cmd::ArmToAmpPos());
  // _driverController.B().OnFalse(cmd::ArmToStow());
  // // amp
  // _driverController.Y().OnTrue(cmd::ArmToAmpPos());
  // _driverController.Y().OnFalse(cmd::ArmToStow());

  // _driverController.A().OnTrue(cmd::PrepareToShoot());






  // POVHelper::Up(&_operatorController).OnTrue(SubClimber::GetInstance().ClimberPosition(0.467_m));
  // POVHelper::Down(&_operatorController).OnTrue(SubClimber::GetInstance().ClimberPosition(0.001_m));
  // POVHelper::Left(&_operatorController).ToggleOnTrue(SubIntake::GetInstance().ToggleExtendIntake());
  // POVHelper::Right(&_operatorController).OnTrue(SubClimber::GetInstance().ClimberPosition(SubClimber::_ClimberPosStow));

  // frc2::Trigger(frc2::CommandScheduler::GetInstance().GetDefaultButtonLoop(), [=, this] {
  //   return (_operatorController.GetLeftY() < -0.2 || _operatorController.GetLeftY() > 0.2) &&
  //   !(_operatorController.GetRightY() < -0.2 || _operatorController.GetRightY() > 0.2);
  // }).WhileTrue(SubClimber::GetInstance().ClimberJoystickDriveLeft(_operatorController));

  // frc2::Trigger(frc2::CommandScheduler::GetInstance().GetDefaultButtonLoop(), [=, this] {
  //   return (_operatorController.GetRightY() < -0.2 || _operatorController.GetRightY() > 0.2) &&
  //   !(_operatorController.GetLeftY() < -0.2 || _operatorController.GetLeftY() > 0.2);
  // }).WhileTrue(SubClimber::GetInstance().ClimberJoystickDriveRight(_operatorController));

  // frc2::Trigger(frc2::CommandScheduler::GetInstance().GetDefaultButtonLoop(), [=, this] {
  //   return (_operatorController.GetRightY() < -0.2 || _operatorController.GetRightY() > 0.2) &&
  //          (_operatorController.GetLeftY() < -0.2 || _operatorController.GetLeftY() > 0.2);
  // }).WhileTrue(SubClimber::GetInstance().ClimberJoystickDrive(_operatorController));

  // Operator controls sysID
  // _operatorController.A().WhileTrue(SubArm::GetInstance().SysIdDynamic(frc2::sysid::Direction::kForward));
  // _operatorController.B().WhileTrue(SubArm::GetInstance().SysIdDynamic(frc2::sysid::Direction::kReverse));
  // _operatorController.X().WhileTrue(SubArm::GetInstance().SysIdQuasistatic(frc2::sysid::Direction::kForward));
  // _operatorController.Y().WhileTrue(SubArm::GetInstance().SysIdQuasistatic(frc2::sysid::Direction::kReverse));
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
  _autoSelected = _autoChooser.GetSelected();
  units::second_t delay = _delayChooser.GetSelected() * 1_s;
  return frc2::cmd::Wait(delay)
      .AndThen(pathplanner::PathPlannerAuto(_autoSelected).ToPtr())
      .AlongWith(SubClimber::GetInstance().ClimberAutoReset().AndThen(
          SubClimber::GetInstance().ClimberPosition(SubClimber::_ClimberPosStow)));
}

frc2::CommandPtr RobotContainer::Rumble(double force, units::second_t duration) {
return frc2::cmd::Run([this, force, duration]{  
    //_driverController.SetRumble(frc::GenericHID::RumbleType::kBothRumble, force);
    _operatorController.SetRumble(frc::GenericHID::RumbleType::kBothRumble, force);}).WithTimeout(duration)
    .FinallyDo([this]{
    //_driverController.SetRumble(frc::GenericHID::RumbleType::kBothRumble, 0);
    _operatorController.SetRumble(frc::GenericHID::RumbleType::kBothRumble, 0);});
}

