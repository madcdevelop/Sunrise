// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SunriseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SunriseAICharacter.generated.h"

/**
 *  The character that the AI controls.
 */
UCLASS()
class SUNRISE_API ASunriseAICharacter : public ASunriseCharacter
{
	GENERATED_BODY()


public:

    UPROPERTY(EditAnywhere, Category = "AI")
    class UBehaviorTree* AIBehaviorTree;

public:
    ASunriseAICharacter();

    // Actions
    void Attack() override;
};
