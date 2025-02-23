#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkaterHUDInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USkaterHUDInterface : public UInterface
{
    GENERATED_BODY()
};

class SKATEBOARDSIMULATOR_API ISkaterHUDInterface
{
    GENERATED_BODY()

public:
    /** Updates the player's score */
    virtual void UpdateScore(int32 Score) = 0;
};
