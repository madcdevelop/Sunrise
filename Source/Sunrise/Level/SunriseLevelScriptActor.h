// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"

#include "SunriseLevelScriptActor.generated.h"

/**
 * Sunrise's base level script class.
 */
UCLASS()
class SUNRISE_API ASunriseLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
    
public:
    ASunriseLevelScriptActor();

protected:
	// Called when the game starts or when spawned
    virtual void BeginPlay() override;

};
