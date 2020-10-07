// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SunriseCharMovementComponent.generated.h"

/**
 * Component for handling Sunrise character movement
 */
UCLASS()
class SUNRISE_API USunriseCharMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
    USunriseCharMovementComponent();

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
    
    void Wander();
	
};
