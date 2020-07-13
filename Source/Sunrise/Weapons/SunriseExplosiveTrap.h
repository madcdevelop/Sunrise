// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SunriseTrap.h"
#include "SunriseExplosiveTrap.generated.h"

/**
 * An explosive trap that damages whoever steps on it.
 */
UCLASS()
class SUNRISE_API ASunriseExplosiveTrap : public ASunriseTrap
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASunriseExplosiveTrap();

    // Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
