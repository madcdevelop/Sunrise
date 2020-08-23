// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseWeapon.h"
#include "../Character/SunriseAICharacter.h"
#include "../Character/SunrisePlayerCharacter.h"
#include "../Classes/Kismet/GameplayStatics.h"

// Sets default values
ASunriseWeapon::ASunriseWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    if (Root)
    {
        RootComponent = Root;
    }

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    if (WeaponMesh)
    {
        WeaponMesh->SetupAttachment(Root);
    }

}

// Called when the game starts or when spawned
void ASunriseWeapon::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &ASunriseWeapon::OnOverlap);
    
}

// Called every frame
void ASunriseWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ASunriseWeapon::OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
    ASunriseAICharacter* AICharacter = Cast<ASunriseAICharacter>(OtherActor);
    ASunrisePlayerCharacter* PlayerCharacter = Cast<ASunrisePlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    
    if (AICharacter && PlayerCharacter && PlayerCharacter->GetIsAttacking()) {
        FDamageEvent WeaponDamageEvent;
        AController* PlayerController = GetInstigatorController();

        float WeaponDamage = AICharacter->TakeDamage(Damage, WeaponDamageEvent, PlayerController, this);
        AICharacter->SetHealth(AICharacter->GetHealth() - WeaponDamage);

        UE_LOG(LogTemp, Display, TEXT("The character's health is %f"), AICharacter->GetHealth());
    }

}

