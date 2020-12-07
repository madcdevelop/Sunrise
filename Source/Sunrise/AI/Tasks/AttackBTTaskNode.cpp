// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBTTaskNode.h"
#include "../SunriseAIController.h"
#include "../../Character/SunrisePlayerCharacter.h"

UAttackBTTaskNode::UAttackBTTaskNode()
{
    BBKeyTargetPlayer.SelectedKeyName = "TargetPlayer";
}

EBTNodeResult::Type UAttackBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ASunriseAIController* AICon = Cast<ASunriseAIController>(OwnerComp.GetAIOwner());
    UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComponent();

    if(AICon)
    {
        // call attack function
        return EBTNodeResult::Succeeded;
    }
    else
    {
        return EBTNodeResult::Failed;    
    }
    

}