#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "SkaterCharacterInterface.h"
#include "SkaterAnimInstance.generated.h"

UCLASS()
class SKATEBOARDSIMULATOR_API USkaterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	/** Initialize Animation */
	virtual void NativeInitializeAnimation() override;

	/** Update Animation */
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

protected:
	/** Character reference */
	UPROPERTY(BlueprintReadOnly, Category = "References")
	ACharacter* Character;

	/** Movement Component */
	UPROPERTY(BlueprintReadOnly, Category = "References")
	UCharacterMovementComponent* MovementComponent;

	/** Character speed */
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float GroundSpeed;

	/** Whether the character is falling */
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool IsFalling;

	/** Whether the character should be moving */
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool ShouldMove;

	/** Leg positions for the skateboard */
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	FVector FrontLegEffectorLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	FVector BackLegEffectorLocation;
};
