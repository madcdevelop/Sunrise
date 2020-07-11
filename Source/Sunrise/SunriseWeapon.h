// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SunriseWeapon.generated.h"

UCLASS()
class SUNRISE_API ASunriseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASunriseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Properties)
	int32 WeaponId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Properties)
	FString Name;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Properties)
	int32 Damage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
