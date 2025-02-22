#include "SkaterGameMode.h"
#include "SkateboardSimulator/Characters/SkaterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASkaterGameMode::ASkaterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SkateboardSimulator/Characters/Blueprint/BP_SkaterCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerScore = 0;
}

void ASkaterGameMode::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!"));
	PlayerScore = 0;
	// Implement game-over logic (e.g., UI, restart)
}

void ASkaterGameMode::OnPlayerScored(int32 Points)
{
	PlayerScore += Points;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), PlayerScore);
	// Call UI update function here
}
