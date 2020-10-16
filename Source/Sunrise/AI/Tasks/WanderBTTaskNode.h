// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "WanderBTTaskNode.generated.h"

/**
 * Wander behavior task for AI movement.
 */
UCLASS()
class SUNRISE_API UWanderBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

private:

    UPROPERTY(EditAnywhere, Category = "Wander")
    FVector WanderTarget;

    UPROPERTY(EditAnywhere, Category = "Wander")
    float WanderRadius;

    UPROPERTY(EditAnywhere, Category = "Wander")
    float WanderDistance;

    UPROPERTY(EditAnywhere, Category = "Wander")
	float WanderJitter;


public:
    UWanderBTTaskNode();

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
