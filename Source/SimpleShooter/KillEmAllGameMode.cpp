#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "ShooterAIController.h"
#include "GameFramework/Controller.h"

void AKillEmAllGameMode::PawnKilled(APawn *PawnKilled)
{
    Super::PawnKilled(PawnKilled);
    APlayerController *PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController != nullptr)
    {
       return EndGame(false);
    }

    bool IsAIAlive = false;
    for (AShooterAIController *AIController : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!AIController->IsDead())
        {
            return;
        }
    }
 
    EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
    for (AController *Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWin = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWin);
    }
}
