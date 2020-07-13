// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SunriseWeapon.h"
#include "SunriseBomb.generated.h"

/**
 * 
 */
UCLASS()
class SUNRISE_API ASunriseBomb : public ASunriseWeapon
{
	GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASunriseBomb();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:

    FTimerHandle BombTimerHandle;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
    float MaxFuseTime;

    UFUNCTION()
    void Explode();
	
};