// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseTile.h"

// Sets default values
ASunriseTile::ASunriseTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Components
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    if (Root) RootComponent = Root;
    Floor = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor"));
    Wall  = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall"));

    // Tile Defaults
    MaxX = 5;
    MaxY = 5;
    TileSize = 1000;
}

// Called when the game starts or when spawned
void ASunriseTile::BeginPlay()
{
	Super::BeginPlay();

    GenerateTiles();
}

// Called every frame
void ASunriseTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASunriseTile::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

void ASunriseTile::GenerateTiles()
{
    int32 Area  = MaxX * MaxY;
    int32 Start = MaxY / 2;
    int32 End   = Area - Start;

    // Map
    TArray<int32> TileType;
    for(size_t i = 0; i < Area; ++i)
    {
        if(i == 0 || i == Start || i == End) TileType.Add(0);
        else TileType.Add(1);
    }

    // Spawn
    int32 TileIndex = 0;
    for(const auto &Type : TileType)
    {
        int32 X = (TileIndex / MaxY) * TileSize;
        int32 Y = (TileIndex % MaxY) * TileSize;

        FVector TileLocation(X, Y, 0.0f);
        FTransform TileTransform(TileLocation);

        if (Type == 0)
        {
            Floor->AddInstance(TileTransform);
        }
        else if (Type == 1)
        {
            Wall->AddInstance(TileTransform);
        }
        ++TileIndex;
    }

}