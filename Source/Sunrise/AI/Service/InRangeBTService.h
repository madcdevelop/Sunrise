// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "InRangeBTService.generated.h"

/**
 * Checks if target is in range of AI.
 */
UCLASS()
class SUNRISE_API UInRangeBTService : public UBTService
{
	GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector BBKeyTargetPlayer;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector BBMeleeRange;

    UPROPERTY(EditAnywhere, Category = "Node")
    float MeleeRange;

public:
    UInRangeBTService();

private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
