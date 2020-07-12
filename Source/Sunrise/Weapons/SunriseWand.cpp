// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseWand.h"

// Sets default values
ASunriseWand::ASunriseWand()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Setup
    WeaponId = WAND;
    Name = "Wand";
    Damage = 25;
}

// Called when the game starts or when spawned
void ASunriseWand::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ASunriseWand::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

