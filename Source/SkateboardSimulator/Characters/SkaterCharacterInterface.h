#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkaterCharacterInterface.generated.h"

UINTERFACE(BlueprintType)
class USkaterCharacterInterface : public UInterface
{
    GENERATED_BODY()
};

class SKATEBOARDSIMULATOR_API ISkaterCharacterInterface
{
    GENERATED_BODY()

public:
    /** This ensures the function can be called in both C++ and Blueprint */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SkaterCharacterInterface")
    void GetLegLocationsForSkateboard(FVector& OutFrontLegLocation, FVector& OutBackLegLocation) const;
};
