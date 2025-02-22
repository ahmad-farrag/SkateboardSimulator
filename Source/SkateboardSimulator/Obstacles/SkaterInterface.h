#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkaterInterface.generated.h"

UINTERFACE(MinimalAPI)
class USkaterInterface : public UInterface
{
    GENERATED_BODY()
};

class SKATEBOARDSIMULATOR_API ISkaterInterface
{
    GENERATED_BODY()

public:
    virtual bool CanBeCountedForScore() const = 0;
};
