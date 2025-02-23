#include "SkaterAnimInstance.h"
#include "SkaterCharacterInterface.h"
#include "GameFramework/CharacterMovementComponent.h"

void USkaterAnimInstance::NativeInitializeAnimation()
{
    // Get owning actor
    Character = Cast<ACharacter>(TryGetPawnOwner());

    // Check if the Character implements the SkaterCharacterInterface
    if (Character && Character->Implements<USkaterCharacterInterface>())
    {
        // Use the interface to get the movement component
        MovementComponent = ISkaterCharacterInterface::Execute_GetSkaterMovementComponent(Character);
    }
}

void USkaterAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
    // Ensure character and movement component are valid
    if (!Character || !MovementComponent)
    {
        return;
    }

    // Get velocity and calculate ground speed
    FVector Velocity = MovementComponent->Velocity;
    GroundSpeed = Velocity.Size2D(); // Only XY plane speed

    // Get acceleration and determine if the character should move
    FVector Acceleration = MovementComponent->GetCurrentAcceleration();
    ShouldMove = (GroundSpeed > 30.f && !Acceleration.IsNearlyZero());

    // Check if character is falling
    IsFalling = MovementComponent->IsFalling();

    // Fetch leg locations from the interface
    if (Character->Implements<USkaterCharacterInterface>())
    {
        ISkaterCharacterInterface::Execute_GetLegLocationsForSkateboard(Character, FrontLegEffectorLocation, BackLegEffectorLocation);
    }
}
