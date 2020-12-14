// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SunriseAIController.h"
#include "SunriseCrowdAIController.generated.h"

/**
 * Controls the Crowd AI.
 */
UCLASS()
class SUNRISE_API ASunriseCrowdAIController : public ASunriseAIController
{
	GENERATED_BODY()


public:
    ASunriseCrowdAIController(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void OnPossess(APawn* AIPawn) override;
};
