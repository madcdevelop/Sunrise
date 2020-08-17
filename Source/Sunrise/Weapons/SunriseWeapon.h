// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SunriseWeapon.generated.h"

enum Weapons {
    SWORD = 0,
    SPEAR,
    WAND,
    BOMB
};

/**
 * The base weapon class.
 */
UCLASS()
class SUNRISE_API ASunriseWeapon : public AActor
{
    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    ASunriseWeapon();

    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
    int32 WeaponId;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
    FString Name;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
    float Damage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* WeaponMesh;

};
