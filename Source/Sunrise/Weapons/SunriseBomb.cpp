// Fill out your copyright notice in the Description page of Project Settings.

#include "SunriseBomb.h"

// Sets default values
ASunriseBomb::ASunriseBomb()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Setup
    Damage = 150.0f;
    BombFuseTime = 5.0f;

}

// Called when the game starts or when spawned
void ASunriseBomb::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(BombTimerHandle, this, &ASunriseBomb::Explode, BombFuseTime, false);

}

// Called every frame
void ASunriseBomb::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ASunriseBomb::Explode()
{

    // Destory object and clear timer
    this->Destroy();
    GetWorld()->GetTimerManager().ClearTimer(BombTimerHandle);
    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    UE_LOG(LogTemp, Display, TEXT("The bomb has exploded!"));
}

