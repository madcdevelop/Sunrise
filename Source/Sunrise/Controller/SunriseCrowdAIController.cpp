// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseCrowdAIController.h"
#include "../Classes/Navigation/CrowdFollowingComponent.h"

ASunriseCrowdAIController::ASunriseCrowdAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

void ASunriseCrowdAIController::OnPossess(APawn* AIPawn)
{
    Super::OnPossess(AIPawn);
}