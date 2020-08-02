// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SunriseCharacter.generated.h"

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
    int32 Health;


	// Animations
	FTimerHandle CharacterTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimationAsset* AttackAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    float AttackAnimationTime;

private:
	
	/* Checks whether the character is currently attacking or not. */
    bool isAttacking;

};
