// Fill out your copyright notice in the Description page of Project Settings.


#include "WanderBTTaskNode.h"
#include "../SunriseAIController.h"
#include "../../Character/SunriseAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h"
#include "../Classes/Kismet/KismetMathLibrary.h"

UWanderBTTaskNode::UWanderBTTaskNode()
{
    WanderRadius   = 100.0f;
    WanderDistance = 200.0f;
    WanderJitter   = 20.0f;
    WanderRatio    = 0.4f;
}

EBTNodeResult::Type UWanderBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ASunriseAIController* AICon = Cast<ASunriseAIController>(OwnerComp.GetAIOwner());
    ACharacter* AIChar = AICon->GetCharacter();

    if (AICon && AIChar)
    {
        // Setup Wander Target unit circle
        float Theta = FMath::RandRange(0.0f, 1.0f) * (PI * 2);
        WanderTarget = FVector(WanderRadius * cos(Theta), WanderRadius * sin(Theta), 0.0f);

        // Add small random vector to the target's position using a random value between -1 and 1
        WanderTarget += FVector(FMath::RandRange(-1.0f, 1.0f) * WanderJitter, FMath::RandRange(-1.0f, 1.0f) * WanderJitter, 0.0f);

        // Reproject this new vector back onto a unit circle
        WanderTarget.Normalize();

        // Increase the length of the vector to the same as the radius of the wander circle
        WanderTarget *= WanderRadius;

        // Move the target into a position WanderDist in front/back of the agent
        FVector TargetLocal;
        if(FMath::RandRange(0.0f, 1.0f) > WanderRatio)
            TargetLocal = WanderTarget + FVector(WanderDistance, 0.0f, 0.0f);
        else
            TargetLocal = WanderTarget + FVector(-WanderDistance, 0.0f, 0.0f);

        // Project target into world space
        FTransform AITransform(AIChar->GetActorRotation(), AIChar->GetActorLocation(), AIChar->GetActorScale3D());
        FVector TargetWorld = UKismetMathLibrary::TransformLocation(AITransform, TargetLocal);

        // Streer toward it
        // FVector NextLocation = TargetWorld - AIChar->GetActorLocation();

        // Set new location for actor
        UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComponent();
        BlackboardComp->SetValueAsVector("NextLocation", TargetWorld);

        return EBTNodeResult::Succeeded;

    }
    else
    {
        return EBTNodeResult::Failed;
    }
}