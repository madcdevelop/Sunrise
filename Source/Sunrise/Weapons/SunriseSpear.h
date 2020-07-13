// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SunriseWeapon.h"
#include "SunriseSpear.generated.h"

/**
 * A spear that does damage.
 */
UCLASS()
class SUNRISE_API ASunriseSpear : public ASunriseWeapon
{
	GENERATED_BODY()
	
public:
    // Sets default values for this actor's properties
    ASunriseSpear();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

};