// Fill out your copyright notice in the Description page of Project Settings.

#include "SunriseAIController.h"
#include "../Character/SunrisePlayerCharacter.h"
#include "../Character/SunriseAICharacter.h"

ASunriseAIController::ASunriseAIController(const FObjectInitializer& ObjectInitializer)
{
    AIBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("AIBlackboard"));
    AIBehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("AIBehaviorTree"));
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

void ASunriseAIController::OnPossess(APawn* AIPawn)
{
    Super::OnPossess(AIPawn);

    ASunriseAICharacter* AICharacter = Cast<ASunriseAICharacter>(AIPawn);

    if(AICharacter)
    {
        AIBlackboard->InitializeBlackboard(*AIBlackboard->GetBlackboardAsset());
    }

    AIBehaviorTreeComp->StartTree(*AICharacter->AIBehaviorTree);
    AIPerception->OnPerceptionUpdated.AddDynamic(this, &ASunriseAIController::OnPerceptionUpdated);
}

void ASunriseAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    UObject* TargetPlayer = GetBlackboardComponent()->GetValueAsObject("TargetPlayer");
    TArray<AActor*> OutActors;
    AIPerception->GetCurrentlyPerceivedActors(nullptr, OutActors);

    if(!TargetPlayer)
    {
        for(auto SingleActor: OutActors)
        {
            ASunrisePlayerCharacter* PlayerCharacter = Cast<ASunrisePlayerCharacter>(SingleActor);

            if(PlayerCharacter)
            {
                GetBlackboardComponent()->SetValueAsObject("TargetPlayer", PlayerCharacter);
                break;
            }
        }
    }
}