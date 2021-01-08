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

void ASunriseRoom::GenerateRoom(TArray<FTile>& MapTiles, int32 StartMapTileIndex, int32 MapSizeY, FRandomStream Stream, FVector Offset)
{
    int32 SizeX = Stream.RandRange(MinSize, MaxSize);
    int32 SizeY = Stream.RandRange(MinSize, MaxSize);

    int32 MidX = SizeX / 2;
    int32 MidY = SizeY / 2;

    // Remove a random number of doors
    int32 DoorCount = Stream.RandRange(1, 4);
    int32 DoorRemovalCount = 4 - DoorCount;

    TArray<EDoor> DoorPosition;
    DoorPosition.Add(EDoor::South);
    DoorPosition.Add(EDoor::East);
    DoorPosition.Add(EDoor::North);
    DoorPosition.Add(EDoor::West);

    for(size_t i = 0; i < DoorRemovalCount; ++i)
    {
        int32 RemoveDoor = Stream.RandRange(0, 1);
        if(RemoveDoor == 1)
        {
            DoorPosition[i] = EDoor::None;
        }
    }

    // Populate room with meshes
    for(size_t RowIndex = 0; RowIndex < SizeX; ++RowIndex)
    {
        for(size_t ColumnIndex = 0; ColumnIndex < SizeY; ++ColumnIndex)
        {
            int32 TileIndex = RowIndex * SizeY + ColumnIndex;
            int LocationX = (TileIndex / SizeY) * MeshSize.X + Offset.X;
            int LocationY = (TileIndex % SizeY) * MeshSize.Y + Offset.Y;

            // Add Floors
            FVector FloorLocation(LocationX, LocationY, 0.0f);
            FTransform FloorTransform(FloorLocation);
            GenerateTile(Floor, FloorTransform);

            Tiles.Add(FTile(ETile::Floor, 0, 0, RowIndex, ColumnIndex, FloorLocation, nullptr));
            
            // Add Doors
            FName Name;
            FVector DoorOpeningLocation;
            FVector Extents(MeshSize.X / 2.0f, MeshSize.Y / 2.0f, 500.0f);
            if(DoorCount > 0 && DoorPosition[0] == EDoor::South && RowIndex == 0 && ColumnIndex == MidY)
            {
                DoorOpeningLocation = FVector(LocationX - (MeshSize.X * 0.5f), LocationY, 0.0f);
                Name = "DoorOpeningSouth";
                DoorOpeningSouth = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                Tiles[TileIndex].Type = ETile::Door;
                --DoorCount;
                continue;
            }
            else if(DoorCount > 0 && DoorPosition[1] == EDoor::East && RowIndex == MidX && ColumnIndex == SizeY-1)
            {
                DoorOpeningLocation = FVector(LocationX, LocationY + (MeshSize.Y * 0.5f), 0.0f);
                Name = "DoorOpeningEast";
                DoorOpeningEast = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                Tiles[TileIndex].Type = ETile::Door;
                --DoorCount;
                continue;
            }
            else if(DoorCount > 0 && DoorPosition[2] == EDoor::North && RowIndex == SizeX-1 && ColumnIndex == MidY) 
            {
                DoorOpeningLocation = FVector(LocationX + (MeshSize.X * 0.5f), LocationY, 0.0f);
                Name = "DoorOpeningNorth";
                DoorOpeningNorth = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                Tiles[TileIndex].Type = ETile::Door;
                --DoorCount;
                continue;
            }
            else if(DoorCount > 0 && DoorPosition[3] == EDoor::West && RowIndex == MidX && ColumnIndex == 0)
            {
                DoorOpeningLocation = FVector(LocationX, LocationY - (MeshSize.Y * 0.5f), 0.0f);
                Name = "DoorOpeningWest";
                DoorOpeningWest = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                Tiles[TileIndex].Type = ETile::Door;
                --DoorCount;
                continue;
            }
            
            // Add Walls
            if(RowIndex == 0)
            {
                FRotator WallRotation(0.0f, 0.0f, 0.0f);
                FVector WallLocation(LocationX - (MeshSize.X * 0.6f), LocationY, 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                GenerateTile(Wall, WallTransform);
                Tiles[TileIndex].Type = ETile::WallSouth;
            }
            if(RowIndex == SizeX-1)
            {
                FRotator WallRotation(0.0f, 180.0f, 0.0f);
                FVector WallLocation(LocationX + (MeshSize.X * 0.6f), LocationY, 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                GenerateTile(Wall, WallTransform);
                Tiles[TileIndex].Type = ETile::WallNorth;
            }
            if(ColumnIndex == 0)
            {
                FRotator WallRotation(0.0f, 90.0f, 0.0f);
                FVector WallLocation(LocationX, LocationY - (MeshSize.Y * 0.6f), 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                GenerateTile(Wall, WallTransform);
                Tiles[TileIndex].Type = ETile::WallWest;
            }
            if(ColumnIndex == SizeY-1)
            {
                FRotator WallRotation(0.0f, -90.0f, 0.0f);
                FVector WallLocation(LocationX, LocationY + (MeshSize.Y * 0.6f), 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                GenerateTile(Wall, WallTransform);
                Tiles[TileIndex].Type = ETile::WallEast;
            }

            // Add Corner pillars
            if(RowIndex == 0 && ColumnIndex == 0)
            {
                FRotator PillarCornerRotation(0.0f, 0.0f, 0.0f);
                FVector PillarCornerLocation(LocationX - (MeshSize.X * 0.5f), LocationY - (MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                GenerateTile(PillarCorner, PillarCornerTransform);
                Tiles[TileIndex].Type = ETile::CornerWallSouthWest;
            }
            else if(RowIndex == 0 && ColumnIndex == SizeY-1)
            {
                FRotator PillarCornerRotation(0.0f, -90.0f, 0.0f);
                FVector PillarCornerLocation(LocationX - (MeshSize.X * 0.5f), LocationY + (MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                GenerateTile(PillarCorner, PillarCornerTransform);
                Tiles[TileIndex].Type = ETile::CornerWallSouthEast;
            }
            else if(RowIndex == SizeX-1 && ColumnIndex == SizeY-1)
            {
                FRotator PillarCornerRotation(0.0f, 180.0f, 0.0f);
                FVector PillarCornerLocation(LocationX + (MeshSize.X * 0.5f), LocationY + (MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                GenerateTile(PillarCorner, PillarCornerTransform);
                Tiles[TileIndex].Type = ETile::CornerWallNorthEast;
            }
            else if(RowIndex == SizeX-1 && ColumnIndex == 0)
            {
                FRotator PillarCornerRotation(0.0f, 90.0f, 0.0f);
                FVector PillarCornerLocation(LocationX + (MeshSize.X * 0.5f), LocationY - (MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                GenerateTile(PillarCorner, PillarCornerTransform);
                Tiles[TileIndex].Type = ETile::CornerWallNorthWest;
            }
        }
    }
}

void ASunriseRoom::GenerateHallwayHorizontal(FRandomStream Stream, FVector Offset)
{
    int32 SizeX = 1;
    int32 SizeY = Stream.RandRange(1, MaxSize);

    for(size_t RowIndex = 0; RowIndex < SizeX; ++RowIndex)
    {
        for(size_t ColumnIndex = 0; ColumnIndex < SizeY; ++ColumnIndex)
        {
            int32 TileIndex = RowIndex * SizeY + ColumnIndex;
            int LocationX = (TileIndex / SizeY) * MeshSize.X + Offset.X;
            int LocationY = (TileIndex % SizeY) * MeshSize.Y + Offset.Y;

            // Add Floors
            FVector FloorLocation(LocationX, LocationY, 0.0f);
            FTransform FloorTransform(FloorLocation);
            GenerateTile(Floor, FloorTransform);

            Tiles.Add(FTile(ETile::FloorHallway, 0, 0, RowIndex, ColumnIndex, FloorLocation, nullptr));

            // Add Walls
            FRotator WallRotation(0.0f, 0.0f, 0.0f);
            FVector WallLocation(LocationX - (MeshSize.X * 0.6f), LocationY, 0.0f);
            FTransform WallTransform(WallRotation, WallLocation);
            GenerateTile(Wall, WallTransform);

            WallRotation = FRotator(0.0f, 180.0f, 0.0f);
            WallLocation = FVector(LocationX + (MeshSize.X * 0.6f), LocationY , 0.0f);
            WallTransform = FTransform(WallRotation, WallLocation);
            GenerateTile(Wall, WallTransform);

            // Add Doors
            if(ColumnIndex == 0)
            {
                FName Name = "DoorOpeningWest";
                FVector DoorOpeningLocation(LocationX, LocationY - (MeshSize.Y * 0.5f), 0.0f);
                FVector Extents(MeshSize.X / 2.0f, MeshSize.Y / 2.0f, 500.0f);
                DoorOpeningWest = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                Tiles[TileIndex].Type = ETile::Door;
            }
            if(ColumnIndex == SizeY-1)
            {
                FName Name = "DoorOpeningEast";
                FVector DoorOpeningLocation(LocationX, LocationY + (MeshSize.Y * 0.5f), 0.0f);
                FVector Extents(MeshSize.X / 2.0f, MeshSize.Y / 2.0f, 500.0f);
                DoorOpeningEast = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                Tiles[TileIndex].Type = ETile::Door;
            }
        }
    }
}

void ASunriseRoom::GenerateHallwayVertical(FRandomStream Stream, FVector Offset)
{
    int32 SizeX = Stream.RandRange(1, MaxSize);
    int32 SizeY = 1;

    for(size_t RowIndex = 0; RowIndex < SizeX; ++RowIndex)
    {
        for(size_t ColumnIndex = 0; ColumnIndex < SizeY; ++ColumnIndex)
        {
            int32 TileIndex = RowIndex * SizeY + ColumnIndex;
            int LocationX = (TileIndex / SizeY) * MeshSize.X + Offset.X;
            int LocationY = (TileIndex % SizeY) * MeshSize.Y + Offset.Y;

            // Add Floors
            FVector FloorLocation(LocationX, LocationY, 0.0f);
            FTransform FloorTransform(FloorLocation);
            GenerateTile(Floor, FloorTransform);

            Tiles.Add(FTile(ETile::FloorHallway, 0, 0, RowIndex, ColumnIndex, FloorLocation, nullptr));

            // Add Walls
            FRotator WallRotation(0.0f, -90.0f, 0.0f);
            FVector WallLocation(LocationX, LocationY + (MeshSize.Y * 0.6f), 0.0f);
            FTransform WallTransform(WallRotation, WallLocation);
            GenerateTile(Wall, WallTransform);

            WallRotation = FRotator(0.0f, 90.0f, 0.0f);
            WallLocation = FVector(LocationX, LocationY - (MeshSize.Y * 0.6f), 0.0f);
            WallTransform = FTransform(WallRotation, WallLocation);
            GenerateTile(Wall, WallTransform);

            // Add Doors
            if(RowIndex == 0)
            {
                FName Name = "DoorOpeningSouth";
                FVector DoorOpeningLocation(LocationX - (MeshSize.X * 0.5f), LocationY, 0.0f);
                FVector Extents(MeshSize.X / 2.0f, MeshSize.Y / 2.0f, 500.0f);
                DoorOpeningSouth = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                Tiles[TileIndex].Type = ETile::Door;
            }
            if(RowIndex == SizeX-1)
            {
                FName Name = "DoorOpeningNorth";
                FVector DoorOpeningLocation(LocationX + (MeshSize.X * 0.5f), LocationY, 0.0f);
                FVector Extents(MeshSize.X / 2.0f, MeshSize.Y / 2.0f, 500.0f);
                DoorOpeningNorth = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                Tiles[TileIndex].Type = ETile::Door;
            }
        }
    }
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