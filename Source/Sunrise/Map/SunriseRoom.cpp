// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseRoom.h"

// Sets default values
ASunriseRoom::ASunriseRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Defaults
    MinSize = 3;
    MaxSize = 8;

    // Components
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    if (Root) RootComponent = Root;
    Floor = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor"));
    Ceiling = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Ceiling"));
    Wall = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall"));
    PillarCorner = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PillarCorner"));
    WallDoor = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallDoor"));
}

// Called when the game starts or when spawned
void ASunriseRoom::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASunriseRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASunriseRoom::AddRooms(FRandomStream Stream, int32 MapSizeX, int32 MapSizeY, int32 Rooms, TArray<FTile>& MapTiles)
{
    for(size_t RoomCount = 0; RoomCount < Rooms; ++RoomCount)
    {
        int32 RoomSizeX = Stream.RandRange(MinSize, MaxSize);
        int32 RoomSizeY = Stream.RandRange(MinSize, MaxSize);
        int32 RoomTileIndex = Stream.RandRange(1, (MapSizeX * MapSizeY)-1);

        // Loop through starting at a specific tile index
        for(size_t RowIndex = 0; RowIndex < RoomSizeX; ++RowIndex)
        {
            for(size_t ColumnIndex = 0; ColumnIndex < RoomSizeY; ++ColumnIndex)
            {
                if(MapTiles.IsValidIndex(RoomTileIndex))
                {
                    MapTiles[RoomTileIndex].Type = ETile::Floor;
                }
                ++RoomTileIndex;
            }
            RoomTileIndex += MapSizeY;
        }
    }
}

void ASunriseRoom::GenerateTile(UInstancedStaticMeshComponent* TileMesh, FTransform Transform)
{
    if(TileMesh) 
    {
        TileMesh->AddInstance(Transform);
    }
}

UBoxComponent* ASunriseRoom::CreateTraceBox(AActor* Parent, FName Name, FRotator Rotation, FVector Location, FVector Extents)
{
    if(!Parent) return nullptr;

    FTransform Transform = FTransform(Rotation, Location, FVector(1.0f, 1.0f, 1.0f));

    UBoxComponent* BoxComponent = NewObject<UBoxComponent>(Parent, Name);

    if(BoxComponent)
    {
        BoxComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
        BoxComponent->RegisterComponent();
        BoxComponent->bSelectable = true;
        BoxComponent->bEditableWhenInherited = true;

        BoxComponent->SetBoxExtent(Extents);
        BoxComponent->SetWorldTransform(Transform);
        BoxComponent->SetVisibility(true);
        BoxComponent->SetHiddenInGame(true);
        return BoxComponent;
    }
    else
    {
        return nullptr;
    }
}