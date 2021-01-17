// Fill out your copyright notice in the Description page of Project Settings.


#include "InRangeBTService.h"
#include "../../Controller/SunriseAIController.h"
#include "../../Character/SunriseAICharacter.h"
#include "../../Character/SunrisePlayerCharacter.h"
#include "Math/Vector.h"

UInRangeBTService::UInRangeBTService()
{
    BBKeyTargetPlayer.SelectedKeyName = "TargetPlayer";
    BBMeleeRange.SelectedKeyName = "PlayerIsInMeleeRange";
    MeleeRange = 500.0f;
}

void UInRangeBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ASunriseAIController* AICon = Cast<ASunriseAIController>(OwnerComp.GetAIOwner());
    UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComponent();

    if(AICon)
    {
        UObject* TargetPlayer = BlackboardComp->GetValueAsObject(BBKeyTargetPlayer.SelectedKeyName);
        ASunrisePlayerCharacter* PlayerChar = Cast<ASunrisePlayerCharacter>(TargetPlayer);

        if(PlayerChar)
        {
            FVector AILocation = AICon->GetCharacter()->GetActorLocation();
            FVector PlayerLocation = PlayerChar->GetActorLocation();

            float Diff = FVector::Distance(AILocation, PlayerLocation);

            if(Diff <= MeleeRange) 
                BlackboardComp->SetValueAsBool(BBMeleeRange.SelectedKeyName, true);
            else
                BlackboardComp->SetValueAsBool(BBMeleeRange.SelectedKeyName, false);

            // @TODO: update later to get the lose sight radius number from sight sense.
            if(Diff >= MaxRange)
            {
                BlackboardComp->SetValueAsObject(BBKeyTargetPlayer.SelectedKeyName, nullptr);
            }
        }
    }
}
