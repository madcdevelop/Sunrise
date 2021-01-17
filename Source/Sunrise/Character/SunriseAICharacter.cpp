// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseAICharacter.h"
#include "../Weapons/SunriseWeapon.h"
#include "SunrisePlayerCharacter.h"

ASunriseAICharacter::ASunriseAICharacter()
{
}

// Called when the game starts or when spawned
void ASunriseAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
    OnTakeRadialDamage.AddDynamic(this, &ASunriseAICharacter::OnRadialDamage);
}

void ASunriseAICharacter::Attack()
{
    Super::Attack();

    UE_LOG(LogTemp, Display, TEXT("The AI is attacking"));
}

void ASunriseAICharacter::OnDeath()
{
    Super::OnDeath();
}

void ASunriseAICharacter::OnRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
    Health = Health - Damage;

    if(Health <= 1.0f)
    {
        SetActorEnableCollision(false);
        StartAnimation(DeathAnimation, DeathAnimationTime);
        GetWorld()->GetTimerManager().SetTimer(CharacterTimerHandle, this, &ASunriseAICharacter::OnDeath, DeathAnimationTime, false);
    }
}