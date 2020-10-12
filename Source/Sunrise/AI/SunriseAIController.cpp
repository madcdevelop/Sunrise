// Fill out your copyright notice in the Description page of Project Settings.

#include "SunriseAIController.h"
#include "../Character/SunriseAICharacter.h"

ASunriseAIController::ASunriseAIController()
{
    AIBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("AIBlackboard"));
    AIBehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("AIBehaviorTree"));

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

}