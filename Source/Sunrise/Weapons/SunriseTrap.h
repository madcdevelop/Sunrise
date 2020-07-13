// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SunriseWeapon.h"
#include "SunriseTrap.generated.h"

/**
 * A trap class to hold data about traps and functions of them.
 */
UCLASS()
class SUNRISE_API ASunriseTrap : public ASunriseWeapon
{
	GENERATED_BODY()
	
    public:	
	// Sets default values for this actor's properties
	ASunriseTrap();

    // Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};