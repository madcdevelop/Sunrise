// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SunriseWeapon.generated.h"

UENUM(BlueprintType)
enum EWeapons {
    Sword = 0,
    Spear,
    Wand,
    Bomb
};

/**
 * The base weapon class.
 */
UCLASS()
class SUNRISE_API ASunriseWeapon : public AActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Properties")
    TEnumAsByte<EWeapons> WeaponsEnum;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
    float Damage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* WeaponMesh;

public:	
    // Sets default values for this actor's properties
    ASunriseWeapon();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    FORCEINLINE float   GetDamage() const { return Damage; }
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

};
