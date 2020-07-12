// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseWeapon.h"

// Sets default values
ASunriseWeapon::ASunriseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

}

// Called when the game starts or when spawned
void ASunriseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASunriseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

