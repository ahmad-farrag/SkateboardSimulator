#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ScoreInterface.h"
#include "SkaterGameMode.generated.h"

UCLASS(minimalapi)
class ASkaterGameMode : public AGameModeBase, public IScoreInterface
{
	GENERATED_BODY()

public:
	ASkaterGameMode();

	virtual void OnGameOver() override;
	virtual void OnPlayerScored(int32 Points) override;

private:
	int32 PlayerScore;
};



