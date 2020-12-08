// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBTTaskNode.h"
#include "../SunriseAIController.h"
#include "../../Character/SunriseAICharacter.h"
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
        UObject* TargetPlayer = BlackboardComp->GetValueAsObject(BBKeyTargetPlayer.SelectedKeyName);
        ASunrisePlayerCharacter* PlayerChar = Cast<ASunrisePlayerCharacter>(TargetPlayer);

        if(PlayerChar)
        {
            ACharacter* AIChar = AICon->GetCharacter();
            ASunriseAICharacter* SunriseAIChar = Cast<ASunriseAICharacter>(AIChar);
            SunriseAIChar->Attack();

            return EBTNodeResult::Succeeded;
        }
        else
        {
            return EBTNodeResult::Failed;    
        }
    }
    else
    {
        return EBTNodeResult::Failed;    
    }
    

}