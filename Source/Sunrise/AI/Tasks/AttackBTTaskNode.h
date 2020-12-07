// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackBTTaskNode.generated.h"

/**
 *  Attack behavior task for AI.
 */
UCLASS()
class SUNRISE_API UAttackBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

private:

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector BBKeyTargetPlayer;

public:
    UAttackBTTaskNode();

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
