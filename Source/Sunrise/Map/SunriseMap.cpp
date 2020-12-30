// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseMap.h"
#include "SunriseRoom.h"
#include "SunriseMapTile.h"

// Sets default values
ASunriseMap::ASunriseMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASunriseMap::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASunriseMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASunriseMap::OnConstruction(const FTransform& Transform)
{
    GenerateMap();
}


void ASunriseMap::GenerateMap()
{
    ASunriseRoom* CurrentRoom = Cast<ASunriseRoom>(Room->GetDefaultObject());
    ASunriseMapTile* CurrentTile = Cast<ASunriseMapTile>(Tile->GetDefaultObject());
    if(CurrentRoom && CurrentTile)
    {
        CurrentRoom->GenerateRoom(CurrentTile);
    }
}
