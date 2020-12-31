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
        FVector LocationOffset(0.0f, 0.0f, 0.0f);
        ASunriseMapTile* CurrentRoom = GetWorld()->SpawnActor<ASunriseMapTile>(DefaultTile->GetClass(), LocationOffset, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
        if(CurrentRoom) DefaultRoom->GenerateRoom(CurrentRoom, Stream, LocationOffset);

        if(CurrentRoom->DoorOpeningEast) 
        {
            LocationOffset = CurrentRoom->DoorOpeningEast->GetComponentLocation() + FVector(0.0f, CurrentRoom->MeshSize.Y / 2.0f, 0.0f);
            ASunriseMapTile* CurrentHallway = GetWorld()->SpawnActor<ASunriseMapTile>(DefaultTile->GetClass(), LocationOffset, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
            if(CurrentHallway) DefaultRoom->GenerateHallwayHorizontal(CurrentHallway, Stream, LocationOffset);
        }

        if(CurrentRoom->DoorOpeningNorth)
        {
            LocationOffset = CurrentRoom->DoorOpeningNorth->GetComponentLocation() + FVector(CurrentRoom->MeshSize.X / 2.0f, 0.0f, 0.0f);
            ASunriseMapTile* CurrentHallway = GetWorld()->SpawnActor<ASunriseMapTile>(DefaultTile->GetClass(), LocationOffset, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
            if(CurrentHallway) DefaultRoom->GenerateHallwayVertical(CurrentHallway, Stream, LocationOffset);
        }

        if(CurrentRoom->DoorOpeningWest) 
        {
            LocationOffset = FVector(0.0f, 0.0f, 0.0f);
            ASunriseMapTile* CurrentHallway = GetWorld()->SpawnActor<ASunriseMapTile>(DefaultTile->GetClass(), LocationOffset, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
            if(CurrentHallway) DefaultRoom->GenerateHallwayHorizontal(CurrentHallway, Stream, LocationOffset);
            
            // Move Hallway
            CurrentHallway->Floor->SetRelativeRotation(FQuat(FRotator(0.0f, 180.0f, 0.0f)));
            CurrentHallway->Wall->SetRelativeRotation(FQuat(FRotator(0.0f, 180.0f, 0.0f)));
            CurrentHallway->SetActorRotation(FRotator(0.0f, 180.0, 0.0f));
            CurrentHallway->Floor->SetRelativeLocation(CurrentRoom->DoorOpeningWest->GetComponentLocation() - FVector(0.0f, CurrentRoom->MeshSize.Y / 2.0f, 0.0f));
            CurrentHallway->Wall->SetRelativeLocation(CurrentRoom->DoorOpeningWest->GetComponentLocation() - FVector(0.0f, CurrentRoom->MeshSize.Y / 2.0f, 0.0f));
            CurrentHallway->SetActorLocation(CurrentRoom->DoorOpeningWest->GetComponentLocation() - FVector(0.0f, CurrentRoom->MeshSize.Y / 2.0f, 0.0f));
        }

        if(CurrentRoom->DoorOpeningSouth)
        {
            LocationOffset = FVector(0.0f, 0.0f, 0.0f);
            ASunriseMapTile* CurrentHallway = GetWorld()->SpawnActor<ASunriseMapTile>(DefaultTile->GetClass(), LocationOffset, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
            if(CurrentHallway) DefaultRoom->GenerateHallwayVertical(CurrentHallway, Stream, LocationOffset);

            // Move Hallway
            CurrentHallway->Floor->SetRelativeRotation(FQuat(FRotator(0.0f, 180.0f, 0.0f)));
            CurrentHallway->Wall->SetRelativeRotation(FQuat(FRotator(0.0f, 180.0f, 0.0f)));
            CurrentHallway->SetActorRotation(FRotator(0.0f, 180.0, 0.0f));
            CurrentHallway->Floor->SetRelativeLocation(CurrentRoom->DoorOpeningSouth->GetComponentLocation() - FVector(CurrentRoom->MeshSize.X / 2.0f, 0.0f, 0.0f));
            CurrentHallway->Wall->SetRelativeLocation(CurrentRoom->DoorOpeningSouth->GetComponentLocation() - FVector(CurrentRoom->MeshSize.X / 2.0f, 0.0f, 0.0f));
            CurrentHallway->SetActorLocation(CurrentRoom->DoorOpeningSouth->GetComponentLocation() - FVector(CurrentRoom->MeshSize.X / 2.0f, 0.0f, 0.0f));
        }
    }
}