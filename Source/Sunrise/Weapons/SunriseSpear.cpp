// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseSpear.h"

// Sets default values
ASunriseSpear::ASunriseSpear()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Setup
    WeaponId = SPEAR;
    Name = "Spear";
    Damage = 75.0f;

}

// Called when the game starts or when spawned
void ASunriseSpear::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ASunriseSpear::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


