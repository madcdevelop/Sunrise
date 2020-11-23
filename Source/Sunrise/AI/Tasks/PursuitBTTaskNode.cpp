// Fill out your copyright notice in the Description page of Project Settings.

#include "../SunriseAIController.h"
#include "../../Character/SunrisePlayerCharacter.h"
#include "PursuitBTTaskNode.h"

UPursuitBTTaskNode::UPursuitBTTaskNode()
{
    BBKeyTargetPlayer.SelectedKeyName = "TargetPlayer";
}

EBTNodeResult::Type UPursuitBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ASunriseAIController* AICon = Cast<ASunriseAIController>(OwnerComp.GetAIOwner());
    UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComponent();

    if(AICon)
    {
        UObject* TargetObject = BlackboardComp->GetValueAsObject(BBKeyTargetPlayer.SelectedKeyName);
        ASunrisePlayerCharacter* TargetPlayer = Cast<ASunrisePlayerCharacter>(TargetObject);
        AICon->MoveToActor(TargetPlayer);
        return EBTNodeResult::Succeeded;
    }
    else
    {
        return EBTNodeResult::Failed;
    }
    
}