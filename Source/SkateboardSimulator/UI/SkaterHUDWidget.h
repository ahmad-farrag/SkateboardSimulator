#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkaterHUDWidget.generated.h"

UCLASS()
class SKATEBOARDSIMULATOR_API USkaterHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** Sets the player's score */
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void SetScore(int32 Score);

};
