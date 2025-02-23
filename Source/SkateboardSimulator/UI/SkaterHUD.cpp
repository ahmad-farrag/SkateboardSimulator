#include "SkaterHUD.h"
#include "SkaterHUDWidget.h"
#include "Blueprint/UserWidget.h"

ASkaterHUD::ASkaterHUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetObj(TEXT("/Game/SkateboardSimulator/UI/WBP_SkaterHUD"));
    if (HUDWidgetObj.Succeeded())
    {
        HUDWidgetClass = HUDWidgetObj.Class;
    }
}

void ASkaterHUD::BeginPlay()
{
    Super::BeginPlay();

    if (HUDWidgetClass)
    {
        HUDWidget = CreateWidget<USkaterHUDWidget>(GetWorld(), HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
        }
    }
}

void ASkaterHUD::UpdateScore(int32 Score)
{
    if (HUDWidget)
    {
        HUDWidget->SetScore(Score);
    }
}


