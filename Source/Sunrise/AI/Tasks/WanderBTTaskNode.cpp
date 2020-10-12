// Fill out your copyright notice in the Description page of Project Settings.


#include "WanderBTTaskNode.h"
#include "../SunriseAIController.h"
#include "../../Character/SunriseAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h"
#include "../Classes/Kismet/KismetMathLibrary.h"

UWanderBTTaskNode::UWanderBTTaskNode()
{
    WanderRadius = 100.0f;
    WanderDistance = 200.0f;
    WanderJitter = 2.0f;
}


EBTNodeResult::Type UWanderBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ASunriseAIController* AICon = Cast<ASunriseAIController>(OwnerComp.GetAIOwner());

    // TODO: debug wander movement to get AICharacter going to a specific location
    if (AICon)
    {
        UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComponent();

        // add small random vector to the target's position
        // Use a clamp function that returns a random value between -1 and 1
        FVector WanderTarget = FVector(FMath::RandRange(-1.0f, 1.0f) * WanderJitter, FMath::RandRange(-1.0f, 1.0f) * WanderJitter, 0.0f);

        // reproject this new vector back onto a unit circle
        WanderTarget.Normalize();

        // increase the length of the vector to the same as the radius of the wander circle
        WanderTarget *= WanderRadius;

        //move the target into a position WanderDist in front of the agent
        FVector TargetLocal = WanderTarget + FVector(WanderDistance, 0.0f, 0.0f);

        // project target into world space
        FTransform AITransform;
        FVector TargetWorld = UKismetMathLibrary::TransformLocation(AITransform, TargetLocal);

        // streer toward it
        FVector NewLocation = TargetWorld - AICon->GetCharacter()->GetActorLocation();

        // set blackboard new location of actor
        BlackboardComp->SetValueAsVector("NextLocation", NewLocation);

        return EBTNodeResult::Succeeded;

    }
    else
    {
        return EBTNodeResult::Failed;
    }
}