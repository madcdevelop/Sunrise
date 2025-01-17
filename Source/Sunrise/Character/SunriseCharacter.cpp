// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseCharacter.h"

// Sets default values
ASunriseCharacter::ASunriseCharacter()
{
	// Defaults
    isAttacking = false;
    AttackAnimationTime = 1.0f;
    DeathAnimationTime = 1.0f;


 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASunriseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASunriseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASunriseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASunriseCharacter::Attack()
{
    if (!GetIsAttacking())
    {
        isAttacking = true;
        StartAnimation(AttackAnimation, AttackAnimationTime);
    }
}

void ASunriseCharacter::StartDefend()
{
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
    GetMesh()->SetAnimation(DefendAnimation);
    GetMesh()->Play(false);
}

void ASunriseCharacter::EndDefend()
{
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}

void ASunriseCharacter::StartAnimation(class UAnimationAsset* Animation, float AnimationTime)
{
        GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
        GetMesh()->SetAnimation(Animation);
        GetMesh()->Play(false);
        GetWorld()->GetTimerManager().SetTimer(CharacterTimerHandle, this, &ASunriseCharacter::EndAnimation, AnimationTime, false);
}

void ASunriseCharacter::EndAnimation()
{
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    if(isAttacking) isAttacking = false;
}

void ASunriseCharacter::OnDeath()
{
    SetActorEnableCollision(true);
    TArray<class AActor*> AttachedActors;
    GetAttachedActors(AttachedActors);
    for (auto Actor: AttachedActors)
    {
        Actor->Destroy();
    }
    Destroy();
}