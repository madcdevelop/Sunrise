// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SunriseCharacter.generated.h"

/**
 *  The base character class.
 */
UCLASS(blueprintable)
class SUNRISE_API ASunriseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASunriseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual const bool GetIsAttacking() const { return isAttacking; };

	virtual const float GetHealth() const { return Health; };

	virtual void SetHealth(float CurrentHealth) { Health = CurrentHealth; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Actions
    virtual void Attack();
    virtual void Defend();

	// Animations
	virtual void StartAnimation(class UAnimationAsset* Animation, float AnimationTime);
	virtual void EndAnimation();

public:

	FTimerHandle CharacterTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimationAsset* AttackAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    float AttackAnimationTime;

protected:
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
    float Health;

	/* Checks whether the character is currently attacking or not. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
    bool isAttacking;

	/* The weapon the character is using */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class ASunriseWeapon> WeaponClass;

};
