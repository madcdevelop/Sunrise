// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseSword.h"

// Sets default values
ASunriseSword::ASunriseSword()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Setup
    WeaponId = SWORD;
    Name = "Sword";
    Damage = 50.0f;

}

// Called when the game starts or when spawned
void ASunriseSword::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ASunriseSword::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

