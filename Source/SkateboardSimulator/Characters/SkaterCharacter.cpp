
#include "SkaterCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"


//////////////////////////////////////////////////////////////////////////
// ASkaterCharacter

ASkaterCharacter::ASkaterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	//GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Set default movement parameters
	DefaultSpeed = 500.f; // Default movement speed
	TargetSpeed = DefaultSpeed;
	AccelerationRate = 4.f;  // Speed gain per second
	DecelerationRate = 10.f;  // Speed loss per second

	// Apply the initial speed to character movement
	GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;

	//SkateBoardRotation
	SkateboardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkateboardMesh"));
	SkateboardMesh->SetupAttachment(RootComponent);
	SkateboardMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASkaterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASkaterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateSkateboardRotation();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASkaterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkaterCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASkaterCharacter::Look);

		// Speed up
		EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Started, this, &ASkaterCharacter::SpeedUp);
		EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Completed, this, &ASkaterCharacter::ResetSpeed);

		//Slow Down
		EnhancedInputComponent->BindAction(SlowDownAction, ETriggerEvent::Started, this, &ASkaterCharacter::SlowDown);
		EnhancedInputComponent->BindAction(SlowDownAction, ETriggerEvent::Completed, this, &ASkaterCharacter::ResetSpeed);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void ASkaterCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Reset speed if SlowDown is NOT active and TargetSpeed is less than 500
		if (!bIsSlowingDown && TargetSpeed < DefaultSpeed)
		{
			TargetSpeed = DefaultSpeed;
			GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;
			GetWorldTimerManager().SetTimer(DecelerationTimerHandle, this, &ASkaterCharacter::UpdateDeceleration, 0.02f, true);
		}

		// Reset speed if neither SlowDown nor SpeedUp is active and TargetSpeed is different from 500
		if (!bIsSlowingDown && !bIsSpeedingUp && TargetSpeed != DefaultSpeed)
		{
			TargetSpeed = DefaultSpeed;
			GetWorldTimerManager().SetTimer(DecelerationTimerHandle, this, &ASkaterCharacter::UpdateDeceleration, 0.02f, true);
		}

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASkaterCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASkaterCharacter::SpeedUp()
{
	bIsSpeedingUp = true;  // Mark SpeedUp as active

	const float MaxAllowedSpeed = 1500.f;
	TargetSpeed = FMath::Clamp(TargetSpeed * 1.5f, DefaultSpeed, MaxAllowedSpeed);

	// Start acceleration timer
	GetWorldTimerManager().SetTimer(AccelerationTimerHandle, this, &ASkaterCharacter::UpdateAcceleration, 0.02f, true);
}

void ASkaterCharacter::SlowDown()
{
	bIsSlowingDown = true;  // Mark SlowDown as active

	const float MinAllowedSpeed = 200.f;
	TargetSpeed = FMath::Clamp(TargetSpeed * 0.6f, MinAllowedSpeed, DefaultSpeed);

	// Start deceleration timer
	GetWorldTimerManager().SetTimer(DecelerationTimerHandle, this, &ASkaterCharacter::UpdateDeceleration, 0.02f, true);
}

void ASkaterCharacter::ResetSpeed()
{
	if (!bIsSpeedingUp && !bIsSlowingDown)
	{
		TargetSpeed = DefaultSpeed;
		GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;

		if (GetCharacterMovement()->Velocity.Size() > 0)
		{
			GetWorldTimerManager().SetTimer(DecelerationTimerHandle, this, &ASkaterCharacter::UpdateDeceleration, 0.02f, true);
		}
		else
		{
			GetWorldTimerManager().ClearTimer(DecelerationTimerHandle);
		}
	}

	bIsSpeedingUp = false;
	bIsSlowingDown = false;
}

void ASkaterCharacter::UpdateAcceleration()
{
	float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// If speed is lower than the target speed, increase it
	if (CurrentSpeed < TargetSpeed)
	{
		float NewSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, GetWorld()->GetDeltaSeconds(), AccelerationRate);
		GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
	else
	{
		// Stop the acceleration timer if the target speed is reached
		GetWorldTimerManager().ClearTimer(AccelerationTimerHandle);
	}
}

void ASkaterCharacter::UpdateDeceleration()
{
	float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// If speed is higher than the target speed, decrease it
	if (CurrentSpeed > TargetSpeed)
	{
		float NewSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, GetWorld()->GetDeltaSeconds(), DecelerationRate);
		GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
	else
	{
		// Stop the deceleration timer if target speed is reached
		GetWorldTimerManager().ClearTimer(DecelerationTimerHandle);
	}
}


//SkateBoard Rotation
FVector ASkaterCharacter::GetGroundLocation(const FVector& SocketLocation) const
{
	FVector TraceStart = SocketLocation;
	FVector TraceEnd = SocketLocation - FVector(0, 0, 100); // Trace downward
	FHitResult HitResult;

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	// Debugging
	FColor LineColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, 1.0f, 0, 2.0f);
	DrawDebugPoint(GetWorld(), bHit ? HitResult.ImpactPoint : TraceEnd, 5.0f, LineColor, false, 1.0f);

	return bHit ? HitResult.ImpactPoint : SocketLocation;
}

void ASkaterCharacter::UpdateSkateboardRotation()
{
	if (!SkateboardMesh)
		return;

	// Get socket locations
	FVector FrontSocketLocation = SkateboardMesh->GetSocketLocation("Front");
	FVector BackSocketLocation = SkateboardMesh->GetSocketLocation("Back");

	// Get ground locations
	FVector FrontGroundLocation = GetGroundLocation(FrontSocketLocation);
	FVector BackGroundLocation = GetGroundLocation(BackSocketLocation);

	// Compute Look-At rotation but only modify pitch
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(BackGroundLocation, FrontGroundLocation);

	// Get current rotation
	FRotator CurrentRotation = SkateboardMesh->GetComponentRotation();

	// Only modify pitch (up and down rotation) properly
	float NewPitch = FMath::FInterpTo(CurrentRotation.Pitch, TargetRotation.Pitch, GetWorld()->GetDeltaSeconds(), 30.f);

	FRotator NewRotation = CurrentRotation;
	NewRotation.Pitch = NewPitch;

	// Apply the new rotation
	SkateboardMesh->SetWorldRotation(NewRotation);
}

void ASkaterCharacter::GetLegLocationsForSkateboard_Implementation(FVector& OutFrontLegLocation, FVector& OutBackLegLocation) const
{
	if (!SkateboardMesh)
	{
		OutFrontLegLocation = FVector::ZeroVector;
		OutBackLegLocation = FVector::ZeroVector;
		return;
	}

	OutFrontLegLocation = SkateboardMesh->GetSocketLocation("LegFront");
	OutBackLegLocation = SkateboardMesh->GetSocketLocation("LegBack");
}