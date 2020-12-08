// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "../Classes/BehaviorTree/BehaviorTreeTypes.h"
#include "PursuitBTTaskNode.generated.h"

/**
 * Pursuit behavior task for AI movement.
 */
UCLASS()
class SUNRISE_API UPursuitBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

private:

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector BBKeyTargetPlayer;

    UPROPERTY(EditAnywhere, Category = "Node")
    float AcceptenceRadius;

    UPROPERTY(EditAnywhere, Category = "Node")
    bool AllowStrafe;

    UPROPERTY(EditAnywhere, Category = "Node")
    bool StopOnOverlap;

public:
    UPursuitBTTaskNode();

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
