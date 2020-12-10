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

	FTimerHandle CharacterTimerHandle;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    float AttackAnimationTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimationAsset* AttackAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    class UAnimationAsset* DefendAnimation;

protected:
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
    float Health;

	/* Checks whether the character is currently attacking or not. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
    bool isAttacking;


public:
	// Sets default values for this character's properties
	ASunriseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE virtual const bool GetIsAttacking() const { return isAttacking; };

	FORCEINLINE virtual const float GetHealth() const { return Health; };
	FORCEINLINE virtual void SetHealth(float CurrentHealth) { Health = CurrentHealth; };

    // Actions
    virtual void Attack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Actions
    virtual void StartDefend();
	virtual void EndDefend();

	// Animations
	virtual void StartAnimation(class UAnimationAsset* Animation, float AnimationTime);
	virtual void EndAnimation();



};
