// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseCharMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "../Classes/Kismet/KismetMathLibrary.h"


USunriseCharMovementComponent::USunriseCharMovementComponent()
{
    // Mass

    // MaxSpeed

    // MaxForce

    // RotationRate

    WanderRadius = 100.0f;
    WanderDistance = 200.0f;
    WanderJitter = 2.0f;
}

void USunriseCharMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    // do something
}

FVector USunriseCharMovementComponent::Wander()
{
    // add small random vector to the target's position
    // Use a clamp function that returns a random value between -1 and 1
    //wanter_target=FVector2D(RandomClampled() * WanterJitter, RandomClampled() * WanterJitter);
    FVector WanderTarget = FVector(FMath::RandRange(-1.0f, 1.0f) * WanderJitter, FMath::RandRange(-1.0f, 1.0f) * WanderJitter, 0.0f);

    // reproject this new vector back onto a unit circle
    //WanterTarget.Normalize();
    WanderTarget.Normalize();

    // increase the length of the vector to the same as the radius of the wander circle
    //WanderTarget *= WanderRadius;
    WanderTarget *= WanderRadius;

    //move the target into a position WanderDist in front of the agent
    //FVector2D targetLocal = WanderTarget + FVector2D(WanderDistance, 0);
    FVector TargetLocal = WanderTarget + FVector(WanderDistance, 0.0f, 0.0f);

    // project target into world space
    //FVector2D targetWorld = PointToWorldSpace(targetLocal, Vehicle->Heading(), Vehicle->Side(), Vehicle->Pos());
    FTransform ActorTransform;
    FVector TargetWorld = UKismetMathLibrary::TransformLocation(ActorTransform, TargetLocal);

    // streer toward it
    //return targetWorld - Vehicle->Pos();
    return TargetWorld - GetActorLocation();

}


