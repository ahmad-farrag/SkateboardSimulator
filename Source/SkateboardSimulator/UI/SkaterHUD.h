#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SkaterHUDInterface.h"
#include "SkaterHUD.generated.h"

class USkaterHUDWidget;

UCLASS()
class SKATEBOARDSIMULATOR_API ASkaterHUD : public AHUD, public ISkaterHUDInterface
{
    GENERATED_BODY()

public:
    ASkaterHUD();

protected:
    virtual void BeginPlay() override;

public:
    virtual void UpdateScore(int32 Score) override;

private:
    /** Reference to the main UI widget */
    UPROPERTY()
    class USkaterHUDWidget* HUDWidget;

    /** UI class reference */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> HUDWidgetClass;
};
