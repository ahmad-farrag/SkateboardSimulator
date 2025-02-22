// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS(Blueprintable)
class SKATEBOARDSIMULATOR_API AObstacle : public AActor
{
    GENERATED_BODY()

public:
    AObstacle();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* GameOverTrigger;

    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* ScoreTrigger;

    UPROPERTY()
    AActor* SkaterRef;

    UFUNCTION()
    void OnGameOverTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnScoreTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnScoreTriggerExit(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);
};
