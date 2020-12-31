// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseRoom.h"
#include "SunriseMapTile.h"

// Sets default values
ASunriseRoom::ASunriseRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Defaults
    MinSize = 3;
    MaxSize = 8;
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

void ASunriseRoom::GenerateRoom(ASunriseMapTile* Tile, FRandomStream Stream, FVector Offset)
{
    int32 SizeX = Stream.RandRange(MinSize, MaxSize);
    int32 SizeY = Stream.RandRange(MinSize, MaxSize);
    int32 MidX = SizeX / 2;
    int32 MidY = SizeY / 2;

    // Remove a random number of doors
    int32 DoorCount = Stream.RandRange(1, 4);
    int32 DoorRemovalCount = 4 - DoorCount;

    TArray<Door> DoorPosition;
    DoorPosition.Add(Door::South);
    DoorPosition.Add(Door::East);
    DoorPosition.Add(Door::North);
    DoorPosition.Add(Door::West);

    for(size_t i = 0; i < DoorRemovalCount; ++i)
    {
        int32 RemoveDoor = Stream.RandRange(0, 1);
        if(RemoveDoor == 1)
        {
            DoorPosition[i] = Door::None;
        }
    }

    // Populate room with meshes
    int32 TileIndex = 0;
    for(size_t RowIndex = 0; RowIndex < SizeX; ++RowIndex)
    {
        for(size_t ColumnIndex = 0; ColumnIndex < SizeY; ++ColumnIndex)
        {
            int LocationX = (TileIndex / SizeY) * Tile->MeshSize.X + Offset.X;
            int LocationY = (TileIndex % SizeY) * Tile->MeshSize.Y + Offset.Y;

            // Add Floors
            FVector FloorLocation(LocationX, LocationY, 0.0f);
            FTransform FloorTransform(FloorLocation);
            Tile->GenerateTile(Tile->Floor, FloorTransform);

            // Add Doors
            FName Name;
            FVector DoorOpeningLocation;
            FVector Extents(Tile->MeshSize.X / 2.0f, Tile->MeshSize.Y / 2.0f, 500.0f);
            if(DoorCount > 0 && DoorPosition[0] == Door::South && RowIndex == 0 && ColumnIndex == MidY)
            {
                DoorOpeningLocation = FVector(LocationX - (Tile->MeshSize.X * 0.5f), LocationY, 0.0f);
                Name = "DoorOpeningSouth";
                Tile->DoorOpeningSouth = Tile->CreateTraceBox(Tile, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                --DoorCount;
                ++TileIndex;
                continue;
            }
            else if(DoorCount > 0 && DoorPosition[1] == Door::East && RowIndex == MidX && ColumnIndex == SizeY-1)
            {
                DoorOpeningLocation = FVector(LocationX, LocationY + (Tile->MeshSize.Y * 0.5f), 0.0f);
                Name = "DoorOpeningEast";
                Tile->DoorOpeningEast = Tile->CreateTraceBox(Tile, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                --DoorCount;
                ++TileIndex;
                continue;
            }
            else if(DoorCount > 0 && DoorPosition[2] == Door::North && RowIndex == SizeX-1 && ColumnIndex == MidY) 
            {
                DoorOpeningLocation = FVector(LocationX + (Tile->MeshSize.X * 0.5f), LocationY, 0.0f);
                Name = "DoorOpeningNorth";
                Tile->DoorOpeningNorth = Tile->CreateTraceBox(Tile, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                --DoorCount;
                ++TileIndex;
                continue;
            }
            else if(DoorCount > 0 && DoorPosition[3] == Door::West && RowIndex == MidX && ColumnIndex == 0)
            {
                DoorOpeningLocation = FVector(LocationX, LocationY - (Tile->MeshSize.Y * 0.5f), 0.0f);
                Name = "DoorOpeningWest";
                Tile->DoorOpeningWest = Tile->CreateTraceBox(Tile, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                --DoorCount;
                ++TileIndex;
                continue;
            }
            
            // Add Walls
            if(RowIndex == 0)
            {
                FRotator WallRotation(0.0f, 0.0f, 0.0f);
                FVector WallLocation(LocationX - (Tile->MeshSize.X * 0.6f), LocationY, 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                Tile->GenerateTile(Tile->Wall, WallTransform);
            }
            if(RowIndex == SizeX-1)
            {
                FRotator WallRotation(0.0f, 180.0f, 0.0f);
                FVector WallLocation(LocationX + (Tile->MeshSize.X * 0.6f), LocationY, 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                Tile->GenerateTile(Tile->Wall, WallTransform);
            }
            if(ColumnIndex == 0)
            {
                FRotator WallRotation(0.0f, 90.0f, 0.0f);
                FVector WallLocation(LocationX, LocationY - (Tile->MeshSize.Y * 0.6f), 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                Tile->GenerateTile(Tile->Wall, WallTransform);
            }
            if(ColumnIndex == SizeY-1)
            {
                FRotator WallRotation(0.0f, -90.0f, 0.0f);
                FVector WallLocation(LocationX, LocationY + (Tile->MeshSize.Y * 0.6f), 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                Tile->GenerateTile(Tile->Wall, WallTransform);
            }

            // Add Corner pillars
            if(RowIndex == 0 && ColumnIndex == 0)
            {
                FRotator PillarCornerRotation(0.0f, 0.0f, 0.0f);
                FVector PillarCornerLocation(LocationX - (Tile->MeshSize.X * 0.5f), LocationY - (Tile->MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                Tile->GenerateTile(Tile->PillarCorner, PillarCornerTransform);
            }
            else if(RowIndex == 0 && ColumnIndex == SizeY-1)
            {
                FRotator PillarCornerRotation(0.0f, -90.0f, 0.0f);
                FVector PillarCornerLocation(LocationX - (Tile->MeshSize.X * 0.5f), LocationY + (Tile->MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                Tile->GenerateTile(Tile->PillarCorner, PillarCornerTransform);
            }
            else if(RowIndex == SizeX-1 && ColumnIndex == SizeY-1)
            {
                FRotator PillarCornerRotation(0.0f, 180.0f, 0.0f);
                FVector PillarCornerLocation(LocationX + (Tile->MeshSize.X * 0.5f), LocationY + (Tile->MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                Tile->GenerateTile(Tile->PillarCorner, PillarCornerTransform);
            }
            else if(RowIndex == SizeX-1 && ColumnIndex == 0)
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

void ASunriseRoom::GenerateHallwayHorizontal(ASunriseMapTile* Tile, FRandomStream Stream, FVector Offset)
{
    int32 SizeX = 1;
    int32 SizeY = Stream.RandRange(1, MaxSize);

    int32 TileIndex = 0;
    for(size_t RowIndex = 0; RowIndex < SizeX; ++RowIndex)
    {
        for(size_t ColumnIndex = 0; ColumnIndex < SizeY; ++ColumnIndex)
        {
            int LocationX = (TileIndex / SizeY) * Tile->MeshSize.X + Offset.X;
            int LocationY = (TileIndex % SizeY) * Tile->MeshSize.Y + Offset.Y;

            // Add Floors
            FVector FloorLocation(LocationX, LocationY, 0.0f);
            FTransform FloorTransform(FloorLocation);
            Tile->GenerateTile(Tile->Floor, FloorTransform);

            // Add Walls
            FRotator WallRotation(0.0f, 0.0f, 0.0f);
            FVector WallLocation(LocationX - (Tile->MeshSize.X * 0.6f), LocationY, 0.0f);
            FTransform WallTransform(WallRotation, WallLocation);
            Tile->GenerateTile(Tile->Wall, WallTransform);

            WallRotation = FRotator(0.0f, 180.0f, 0.0f);
            WallLocation = FVector(LocationX + (Tile->MeshSize.X * 0.6f), LocationY , 0.0f);
            WallTransform = FTransform(WallRotation, WallLocation);
            Tile->GenerateTile(Tile->Wall, WallTransform);

            // Add Doors
            if(ColumnIndex == 0)
            {
                FName Name = "DoorOpeningWest";
                FVector DoorOpeningLocation(LocationX, LocationY - (Tile->MeshSize.Y * 0.5f), 0.0f);
                FVector Extents(Tile->MeshSize.X / 2.0f, Tile->MeshSize.Y / 2.0f, 500.0f);
                Tile->DoorOpeningWest = Tile->CreateTraceBox(Tile, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
            }
            if(ColumnIndex == SizeY-1)
            {
                FName Name = "DoorOpeningEast";
                FVector DoorOpeningLocation(LocationX, LocationY + (Tile->MeshSize.Y * 0.5f), 0.0f);
                FVector Extents(Tile->MeshSize.X / 2.0f, Tile->MeshSize.Y / 2.0f, 500.0f);
                Tile->DoorOpeningEast = Tile->CreateTraceBox(Tile, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
            }
            ++TileIndex;
        }
    }
}

void ASunriseRoom::GenerateHallwayVertical(ASunriseMapTile* Tile, FRandomStream Stream, FVector Offset)
{
    int32 SizeX = Stream.RandRange(1, MaxSize);
    int32 SizeY = 1;

    int32 TileIndex = 0;
    for(size_t RowIndex = 0; RowIndex < SizeX; ++RowIndex)
    {
        for(size_t ColumnIndex = 0; ColumnIndex < SizeY; ++ColumnIndex)
        {
            int LocationX = (TileIndex / SizeY) * Tile->MeshSize.X + Offset.X;
            int LocationY = (TileIndex % SizeY) * Tile->MeshSize.Y + Offset.Y;

            // Add Floors
            FVector FloorLocation(LocationX, LocationY, 0.0f);
            FTransform FloorTransform(FloorLocation);
            Tile->GenerateTile(Tile->Floor, FloorTransform);

            // Add Walls
            FRotator WallRotation(0.0f, -90.0f, 0.0f);
            FVector WallLocation(LocationX, LocationY + (Tile->MeshSize.Y * 0.6f), 0.0f);
            FTransform WallTransform(WallRotation, WallLocation);
            Tile->GenerateTile(Tile->Wall, WallTransform);

            WallRotation = FRotator(0.0f, 90.0f, 0.0f);
            WallLocation = FVector(LocationX, LocationY - (Tile->MeshSize.Y * 0.6f), 0.0f);
            WallTransform = FTransform(WallRotation, WallLocation);
            Tile->GenerateTile(Tile->Wall, WallTransform);

            // Add Doors
            if(RowIndex == 0)
            {
                FName Name = "DoorOpeningSouth";
                FVector DoorOpeningLocation(LocationX - (Tile->MeshSize.X * 0.5f), LocationY, 0.0f);
                FVector Extents(Tile->MeshSize.X / 2.0f, Tile->MeshSize.Y / 2.0f, 500.0f);
                Tile->DoorOpeningSouth = Tile->CreateTraceBox(Tile, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
            }
            if(RowIndex == SizeX-1)
            {
                FName Name = "DoorOpeningNorth";
                FVector DoorOpeningLocation(LocationX + (Tile->MeshSize.X * 0.5f), LocationY, 0.0f);
                FVector Extents(Tile->MeshSize.X / 2.0f, Tile->MeshSize.Y / 2.0f, 500.0f);
                Tile->DoorOpeningNorth = Tile->CreateTraceBox(Tile, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
            }
            ++TileIndex;
        }
    }
}
