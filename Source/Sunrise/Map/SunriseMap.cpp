// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseMap.h"
#include "SunriseRoom.h"
#include "SunriseMapTile.h"
#include "../Classes/Kismet/GameplayStatics.h"

// Sets default values
ASunriseMap::ASunriseMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
    if(bNewSeed)
    {
        bNewSeed = false;
        Seed = FMath::RandRange(1, 900000);
        Stream.Initialize(Seed);
    }
    else
    {
        Stream.Initialize(Seed);
    }
    GenerateMap();
}


void ASunriseMap::GenerateMap()
{
    ASunriseMapTile* DefaultTile = Cast<ASunriseMapTile>(Tile->GetDefaultObject());
    ASunriseRoom* DefaultRoom = Cast<ASunriseRoom>(Room->GetDefaultObject());
    if(DefaultTile && DefaultRoom)
    {
        // Look for all actors of type ASunriseMapTile and destroy
        TArray<AActor*> OutActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), Tile, OutActors);
        for(auto Actor: OutActors)
        {
            Actor->Destroy();
        }

        // Spawn Tile to create next room
        FActorSpawnParameters SpawnParams;
        ASunriseMapTile* CurrentTile = GetWorld()->SpawnActor<ASunriseMapTile>(DefaultTile->GetClass(), FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
        if(CurrentTile)
        {
            DefaultRoom->GenerateRoom(CurrentTile, Stream);
        }
    }
}