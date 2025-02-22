#include "Obstacle.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "SkateboardSimulator/Core/ScoreInterface.h"
#include "SkaterInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

AObstacle::AObstacle()
{
    PrimaryActorTick.bCanEverTick = false;

    GameOverTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("GameOverTrigger"));
    GameOverTrigger->SetupAttachment(RootComponent);
    GameOverTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GameOverTrigger->SetCollisionResponseToAllChannels(ECR_Overlap);
    GameOverTrigger->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OnGameOverTriggerEnter);

    ScoreTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ScoreTrigger"));
    ScoreTrigger->SetupAttachment(RootComponent);
    ScoreTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ScoreTrigger->SetCollisionResponseToAllChannels(ECR_Overlap);
    ScoreTrigger->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OnScoreTriggerEnter);
    ScoreTrigger->OnComponentEndOverlap.AddDynamic(this, &AObstacle::OnScoreTriggerExit);
}

void AObstacle::BeginPlay()
{
    Super::BeginPlay();
}

void AObstacle::OnGameOverTriggerEnter(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
    if (GameMode && UKismetSystemLibrary::DoesImplementInterface(Cast<UObject>(GameMode), UScoreInterface::StaticClass()))
    {
        IScoreInterface* ScoreSystem = Cast<IScoreInterface>(GameMode);
        if (ScoreSystem)
        {
            ScoreSystem->OnGameOver();
        }
    }
}

void AObstacle::OnScoreTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (ISkaterInterface* Skater = Cast<ISkaterInterface>(OtherActor))
    {
        if (Skater->CanBeCountedForScore())
        {
            SkaterRef = OtherActor;
        }
    }
}

void AObstacle::OnScoreTriggerExit(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (OtherActor == SkaterRef)
    {
        AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
        if (GameMode && UKismetSystemLibrary::DoesImplementInterface(Cast<UObject>(GameMode), UScoreInterface::StaticClass()))
        {
            IScoreInterface* ScoreSystem = Cast<IScoreInterface>(GameMode);
            if (ScoreSystem)
            {
                ScoreSystem->OnPlayerScored(10);
            }
        }

        SkaterRef = nullptr;
    }
}




