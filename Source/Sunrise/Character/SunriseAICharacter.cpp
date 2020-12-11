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
	
    OnActorBeginOverlap.AddDynamic(this, &ASunriseAICharacter::OnBeginOverlap);
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

void ASunriseAICharacter::OnBeginOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
    ASunriseWeapon* Weapon = Cast<ASunriseWeapon>(OtherActor);

    if(Weapon){
        
        ASunrisePlayerCharacter* PlayerChar = Cast<ASunrisePlayerCharacter>(Weapon->GetAttachParentActor());
        if(PlayerChar && PlayerChar->GetIsAttacking()){

            FDamageEvent WeaponDamageEvent;
            AController* PlayerCon = PlayerChar->GetInstigatorController();

            float WeaponDamage = TakeDamage(Weapon->GetDamage(), WeaponDamageEvent, PlayerCon, Weapon);
            SetHealth(GetHealth() - WeaponDamage);

            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Weapon hit character %s"), *MyOverlappedActor->GetName()));
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("The %s character's health is %f"), *this->GetName(), GetHealth()));

            if(GetHealth() <= 0.0f){
                SetActorEnableCollision(false);
                StartAnimation(DeathAnimation, DeathAnimationTime);
                GetWorld()->GetTimerManager().SetTimer(CharacterTimerHandle, this, &ASunriseAICharacter::OnDeath, DeathAnimationTime, false);
            }
        }
    }   
}