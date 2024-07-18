#include "Commands/ShootingCommands.h"

#include "Subsystems/SubFeeder.h"
#include "Subsystems/SubShooter.h"
#include "Subsystems/SubTurret.h"

namespace cmd {
    frc2::CommandPtr ShootSequence() {
        return frc2::cmd::Parallel(
            SubShooter::GetInstance().SpinFlyWheel(),
            SubTurret::GetInstance().TurnTo(30_deg),
            frc2::cmd::Sequence(
                frc2::cmd::WaitUntill([] {return SubShooter::GetInstance().AtTarget();}),
                frc2::cmd::WaitUntill([] {return SubTurret::GetInstance().AtTarget();}),
                SubFeeder::GetInstance().Feed()
            )
        );
    }

}