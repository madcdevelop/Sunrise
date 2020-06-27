// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseCharacter.h"

// Sets default values
ASunriseCharacter::ASunriseCharacter()
{
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

