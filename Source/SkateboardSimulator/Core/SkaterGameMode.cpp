#include "SkaterGameMode.h"
#include "SkateboardSimulator/Characters/SkaterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"
#include "SkateboardSimulator/UI/SkaterHUD.h"
#include "UObject/ConstructorHelpers.h"

ASkaterGameMode::ASkaterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SkateboardSimulator/Characters/Blueprint/BP_SkaterCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	HUDClass = ASkaterHUD::StaticClass();

	PlayerScore = 0;
}

void ASkaterGameMode::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!"));
	PlayerScore = 0;

	if (ISkaterHUDInterface* HUDInterface = GetHUDInterface())
	{
		HUDInterface->UpdateScore(PlayerScore);
	}

	// Implement game-over logic (e.g., UI, restart)
}


void ASkaterGameMode::OnPlayerScored(int32 Points)
{
	PlayerScore += Points;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), PlayerScore);

	if (ISkaterHUDInterface* HUDInterface = GetHUDInterface())
	{
		HUDInterface->UpdateScore(PlayerScore);
	}
}


ISkaterHUDInterface* ASkaterGameMode::GetHUDInterface() const
{
	if (AHUD* HUD = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD())
	{
		if (ISkaterHUDInterface* HUDInterface = Cast<ISkaterHUDInterface>(HUD))
		{
			return HUDInterface;
		}
	}
	return nullptr;
}
