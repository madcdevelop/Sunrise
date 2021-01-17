// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBTTaskNode.h"
#include "../../Controller/SunriseAIController.h"
#include "../../Character/SunriseAICharacter.h"
#include "../../Character/SunrisePlayerCharacter.h"

UAttackBTTaskNode::UAttackBTTaskNode()
{
    BBKeyTargetPlayer.SelectedKeyName = "TargetPlayer";
}

EBTNodeResult::Type UAttackBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ASunriseAIController* AICon = Cast<ASunriseAIController>(OwnerComp.GetAIOwner());

    if(AICon)
    {
        UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComponent();
        UObject* TargetPlayer = BlackboardComp->GetValueAsObject(BBKeyTargetPlayer.SelectedKeyName);
        ASunrisePlayerCharacter* PlayerChar = Cast<ASunrisePlayerCharacter>(TargetPlayer);
        ACharacter* AIChar = AICon->GetCharacter();
        ASunriseAICharacter* SunriseAIChar = Cast<ASunriseAICharacter>(AIChar);

        if(PlayerChar && SunriseAIChar)
        {
            if(SunriseAIChar->GetHealth() >= 1.0f)
            {
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
    else
    {
        return EBTNodeResult::Failed;    
    }
    

}