#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    ShowPlayerHUD();
}

void AShooterPlayerController::GameHasEnded(class AActor *EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    ShowEndGameScreen(bIsWinner);
    GetWorldTimerManager().SetTimer(RestartTime, this, &APlayerController::RestartLevel, RestartDelay);
}

void AShooterPlayerController::ShowEndGameScreen(bool bIsWinner)
{
    TSubclassOf<UUserWidget> EndGameScreenClass = bIsWinner ? WinScreenClass : LoseScreenClass;
    UUserWidget *EndGameScreen = CreateWidget(this, EndGameScreenClass);
    if(PlayerHUD != nullptr) {
        PlayerHUD->RemoveFromParent();
    }
    if (EndGameScreen != nullptr)
    {
        EndGameScreen->AddToViewport();
    }
}

void AShooterPlayerController::ShowPlayerHUD()
{
    PlayerHUD = CreateWidget(this, PlayerHUDClass);
    if (PlayerHUD != nullptr)
    {
        PlayerHUD->AddToViewport();
    }
}
