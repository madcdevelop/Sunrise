// Fill out your copyright notice in the Description page of Project Settings.

#include "PursuitBTTaskNode.h"
#include "../SunriseAIController.h"
#include "../../Character/SunrisePlayerCharacter.h"

UPursuitBTTaskNode::UPursuitBTTaskNode()
{
    BBKeyTargetPlayer.SelectedKeyName = "TargetPlayer";
    AcceptenceRadius = 100.0f;
    AllowStrafe = false;
    StopOnOverlap = true;
}

EBTNodeResult::Type UPursuitBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ASunriseAIController* AICon = Cast<ASunriseAIController>(OwnerComp.GetAIOwner());
    UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComponent();

    if(AICon)
    {
        UObject* TargetObject = BlackboardComp->GetValueAsObject(BBKeyTargetPlayer.SelectedKeyName);
        ASunrisePlayerCharacter* TargetPlayer = Cast<ASunrisePlayerCharacter>(TargetObject);
        EPathFollowingRequestResult::Type Result = AICon->MoveToActor(TargetPlayer, AcceptenceRadius, StopOnOverlap, true);
        return EBTNodeResult::Succeeded;
    }
    else
    {
        return EBTNodeResult::Failed;
    }
}