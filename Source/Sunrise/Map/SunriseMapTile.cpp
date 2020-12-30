// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseMapTile.h"

// Sets default values
ASunriseMapTile::ASunriseMapTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Components
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    if (Root) RootComponent = Root;
    Floor = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor"));
    Wall = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall"));
    PillarCorner = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PillarCorner"));

}

// Called when the game starts or when spawned
void ASunriseMapTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASunriseMapTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASunriseMapTile::GenerateTile(UInstancedStaticMeshComponent* TileMesh, FTransform Transform)
{
    if(TileMesh) 
    {
        TileMesh->AddInstance(Transform);
    }
}


