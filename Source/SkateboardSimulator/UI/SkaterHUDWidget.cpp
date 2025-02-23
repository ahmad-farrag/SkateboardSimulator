#include "SkaterHUDWidget.h"

void USkaterHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Set default positive messages
    if (PositiveMessages.Num() == 0)
    {
        PositiveMessages = {
            FText::FromString("Awesome!"),
            FText::FromString("Fabulous!"),
            FText::FromString("Incredible!"),
            FText::FromString("Fantastic!"),
            FText::FromString("Superb!")
        };
    }

    // Set default reset messages
    if (ResetMessages.Num() == 0)
    {
        ResetMessages = {
            FText::FromString("Keep Going!"),
            FText::FromString("Try Again!"),
            FText::FromString("Reset!"),
            FText::FromString("You Got This!"),
            FText::FromString("Don't Give Up!")
        };
    }
}

void USkaterHUDWidget::SetScore_Implementation(int32 Score)
{
    if (Score > 0)
    {
        if (PositiveMessages.Num() > 0)
        {
            SetNotification(GetRandomMessage(PositiveMessages));
        }
    }
    else
    {
        if (ResetMessages.Num() > 0)
        {
            SetNotification(GetRandomMessage(ResetMessages));
        }
    }
}

FText USkaterHUDWidget::GetRandomMessage(const TArray<FText>& Messages) const
{
    if (Messages.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, Messages.Num() - 1);
        return Messages[RandomIndex];
    }
    return FText::FromString("Default Message");
}
