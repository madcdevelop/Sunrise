// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SunriseCharacter.generated.h"

// Logging
DECLARE_LOG_CATEGORY_EXTERN(SunriseGameplayLog, Log, All);


UCLASS(blueprintable)
class SUNRISE_API ASunriseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASunriseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

    UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Properties")
    int32 Health;

protected:

    // Actions
    virtual void Attack();
    virtual void Defend();

};
