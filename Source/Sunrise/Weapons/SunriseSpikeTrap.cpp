// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseSpikeTrap.h"

// Sets default values
ASunriseSpikeTrap::ASunriseSpikeTrap()
{
	// Defaults
	Damage = 10.0f;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASunriseSpikeTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASunriseSpikeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}