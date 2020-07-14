// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseHookShot.h"

// Sets default values
ASunriseHookShot::ASunriseHookShot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    ItemId = HOOKSHOT;
    Name = "Hook Shot";
    
}