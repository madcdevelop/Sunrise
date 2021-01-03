// Fill out your copyright notice in the Description page of Project Settings.


#include "SunrisePlayerController.h"

ASunrisePlayerController::ASunrisePlayerController()
{
}

void ASunrisePlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeUIOnly());
}
