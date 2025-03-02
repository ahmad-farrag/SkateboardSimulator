// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SkateboardSimulator/Obstacles/SkaterInterface.h"
#include "SkaterCharacterInterface.h"
#include "SkaterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(config=Game)
class ASkaterCharacter : public ACharacter, public ISkaterInterface, public ISkaterCharacterInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Speed Up Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SpeedUpAction;
	
	/** Slow Down Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SlowDownAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SkateboardMesh;

public:
	ASkaterCharacter();
	
	/** For acceleration  */


	/** Default Speed when not accelerating or decelerating*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DefaultSpeed;

	/** The rate at which the character accelerates to reach the target speed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AccelerationRate;

	/** The rate at which the character decelerates when slowing down. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DecelerationRate;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/**
	 * Increases the character's movement speed, simulating a speed boost.
	 * Clamps the max speed to prevent excessive acceleration.
	 */
	void SpeedUp();

	/**
	 * Reduces the character's movement speed when moving backward.
	 * Simulates slowing down when pressing the opposite direction.
	 */
	void SlowDown();

	/** Resets the character's movement speed to the default value. */
	void ResetSpeed();

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	FTimerHandle AccelerationTimerHandle;
	FTimerHandle DecelerationTimerHandle;

	void UpdateAcceleration();
	void UpdateDeceleration();
	bool bIsSlowingDown = false; // Tracks if slowdown is active
	bool bIsSpeedingUp = false; // Tracks if SpeedUp is active
	float TargetSpeed; //The target speed the character will accelerate or decelerate towards.

	//Skateboard Rotation
	FVector GetGroundLocation(const FVector& SocketLocation) const;
	void UpdateSkateboardRotation();
public:
	virtual bool CanBeCountedForScore() const override { return true; }

	virtual void GetLegLocationsForSkateboard_Implementation(FVector& OutFrontLegLocation, FVector& OutBackLegLocation) const override;

	virtual UCharacterMovementComponent* GetSkaterMovementComponent_Implementation() const override;
};

