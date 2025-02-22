#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ScoreInterface.generated.h"

UINTERFACE(MinimalAPI)
class UScoreInterface : public UInterface
{
    GENERATED_BODY()
};

class IScoreInterface
{
    GENERATED_BODY()

public:
    virtual void OnGameOver() = 0;
    virtual void OnPlayerScored(int32 Points) = 0;
};
