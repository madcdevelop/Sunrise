// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseRoom.h"
#include "SunriseMapTile.h"

// Sets default values
ASunriseRoom::ASunriseRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
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

void ASunriseRoom::GenerateRoom(ASunriseMapTile* Tile)
{
    int32 SizeX = RoomSizeX;
    int32 SizeY = RoomSizeY;

    int32 TileIndex = 0;
    for(size_t RowIndex = 0; RowIndex < SizeX; ++RowIndex)
    {
        for(size_t ColumnIndex = 0; ColumnIndex < SizeY; ++ColumnIndex)
        {
            int LocationX = (TileIndex / SizeY) * Tile->MeshSize.X;
            int LocationY = (TileIndex % SizeY) * Tile->MeshSize.Y;

            // Add Floors
            FVector FloorLocation(LocationX, LocationY, 0.0f);
            FTransform FloorTransform(FloorLocation);
            Tile->GenerateTile(Tile->Floor, FloorTransform);

            // Add Walls
            // Bottom
            if(RowIndex == 0)
            {
                FRotator WallRotation(0.0f, 0.0f, 0.0f);
                FVector WallLocation(LocationX - (Tile->MeshSize.X * 0.6f), LocationY, 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                Tile->GenerateTile(Tile->Wall, WallTransform);
            }
            // Top
            if(RowIndex == SizeX-1)
            {
                FRotator WallRotation(0.0f, 180.0f, 0.0f);
                FVector WallLocation(LocationX + (Tile->MeshSize.X * 0.6f), LocationY, 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                Tile->GenerateTile(Tile->Wall, WallTransform);
            }
            // Left
            if(ColumnIndex == 0)
            {
                FRotator WallRotation(0.0f, 90.0f, 0.0f);
                FVector WallLocation(LocationX, LocationY - (Tile->MeshSize.X * 0.6f), 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                Tile->GenerateTile(Tile->Wall, WallTransform);
            }
            // Right
            if(ColumnIndex == SizeY-1)
            {
                FRotator WallRotation(0.0f, -90.0f, 0.0f);
                FVector WallLocation(LocationX, LocationY + (Tile->MeshSize.X * 0.6f), 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                Tile->GenerateTile(Tile->Wall, WallTransform);
            }

            // Generate Corner pillars
            // Bottom left
            if(RowIndex == 0 && ColumnIndex == 0)
            {
                FRotator PillarCornerRotation(0.0f, 0.0f, 0.0f);
                FVector PillarCornerLocation(LocationX - (Tile->MeshSize.X * 0.5f), LocationY - (Tile->MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                Tile->GenerateTile(Tile->PillarCorner, PillarCornerTransform);
            }
            // Bottom right
            if(RowIndex == 0 && ColumnIndex == SizeY-1)
            {
                FRotator PillarCornerRotation(0.0f, -90.0f, 0.0f);
                FVector PillarCornerLocation(LocationX - (Tile->MeshSize.X * 0.5f), LocationY + (Tile->MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                Tile->GenerateTile(Tile->PillarCorner, PillarCornerTransform);
            }
            // Top right
            if(RowIndex == SizeX-1 && ColumnIndex == SizeY-1)
            {
                FRotator PillarCornerRotation(0.0f, 180.0f, 0.0f);
                FVector PillarCornerLocation(LocationX + (Tile->MeshSize.X * 0.5f), LocationY + (Tile->MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                Tile->GenerateTile(Tile->PillarCorner, PillarCornerTransform);
            }
            // Top left
            if(RowIndex == SizeX-1 && ColumnIndex == 0)
            {
                FRotator PillarCornerRotation(0.0f, 90.0f, 0.0f);
                FVector PillarCornerLocation(LocationX + (Tile->MeshSize.X * 0.5f), LocationY - (Tile->MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                Tile->GenerateTile(Tile->PillarCorner, PillarCornerTransform);
            }
            ++TileIndex;
        }
    }
}