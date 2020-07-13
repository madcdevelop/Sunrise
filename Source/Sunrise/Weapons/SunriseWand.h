// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SunriseWeapon.h"
#include "SunriseWand.generated.h"

/**
 * A wand that does damage from ranged.
 */
UCLASS()
class SUNRISE_API ASunriseWand : public ASunriseWeapon
{
	GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASunriseWand();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

};