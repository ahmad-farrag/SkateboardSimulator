#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkaterHUDWidget.generated.h"

UCLASS()
class SKATEBOARDSIMULATOR_API USkaterHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void NativeConstruct();

    /** Sets the player's score and triggers notifications */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
    void SetScore(int32 Score);
    virtual void SetScore_Implementation(int32 Score);

protected:
    /** Triggers a notification in Blueprint */
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void SetNotification(const FText& Message);

    /** Array of positive messages when the player scores */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TArray<FText> PositiveMessages;

    /** Array of reset messages when the score is reset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TArray<FText> ResetMessages;

private:
    /** Picks a random message from the given array */
    FText GetRandomMessage(const TArray<FText>& Messages) const;
};
