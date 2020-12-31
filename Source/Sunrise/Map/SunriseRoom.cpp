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
    Wall = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall"));
    PillarCorner = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PillarCorner"));
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

void ASunriseRoom::GenerateRoom(FRandomStream Stream, FVector Offset)
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
    int32 TileIndex = 0;
    for(size_t RowIndex = 0; RowIndex < SizeX; ++RowIndex)
    {
        for(size_t ColumnIndex = 0; ColumnIndex < SizeY; ++ColumnIndex)
        {
            int LocationX = (TileIndex / SizeY) * MeshSize.X + Offset.X;
            int LocationY = (TileIndex % SizeY) * MeshSize.Y + Offset.Y;

            // Add Floors
            FVector FloorLocation(LocationX, LocationY, 0.0f);
            FTransform FloorTransform(FloorLocation);
            GenerateTile(Floor, FloorTransform);

            // Add Doors
            FName Name;
            FVector DoorOpeningLocation;
            FVector Extents(MeshSize.X / 2.0f, MeshSize.Y / 2.0f, 500.0f);
            if(DoorCount > 0 && DoorPosition[0] == EDoor::South && RowIndex == 0 && ColumnIndex == MidY)
            {
                DoorOpeningLocation = FVector(LocationX - (MeshSize.X * 0.5f), LocationY, 0.0f);
                Name = "DoorOpeningSouth";
                DoorOpeningSouth = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                --DoorCount;
                ++TileIndex;
                continue;
            }
            else if(DoorCount > 0 && DoorPosition[1] == EDoor::East && RowIndex == MidX && ColumnIndex == SizeY-1)
            {
                DoorOpeningLocation = FVector(LocationX, LocationY + (MeshSize.Y * 0.5f), 0.0f);
                Name = "DoorOpeningEast";
                DoorOpeningEast = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                --DoorCount;
                ++TileIndex;
                continue;
            }
            else if(DoorCount > 0 && DoorPosition[2] == EDoor::North && RowIndex == SizeX-1 && ColumnIndex == MidY) 
            {
                DoorOpeningLocation = FVector(LocationX + (MeshSize.X * 0.5f), LocationY, 0.0f);
                Name = "DoorOpeningNorth";
                DoorOpeningNorth = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                --DoorCount;
                ++TileIndex;
                continue;
            }
            else if(DoorCount > 0 && DoorPosition[3] == EDoor::West && RowIndex == MidX && ColumnIndex == 0)
            {
                DoorOpeningLocation = FVector(LocationX, LocationY - (MeshSize.Y * 0.5f), 0.0f);
                Name = "DoorOpeningWest";
                DoorOpeningWest = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
                --DoorCount;
                ++TileIndex;
                continue;
            }
            
            // Add Walls
            if(RowIndex == 0)
            {
                FRotator WallRotation(0.0f, 0.0f, 0.0f);
                FVector WallLocation(LocationX - (MeshSize.X * 0.6f), LocationY, 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                GenerateTile(Wall, WallTransform);
            }
            if(RowIndex == SizeX-1)
            {
                FRotator WallRotation(0.0f, 180.0f, 0.0f);
                FVector WallLocation(LocationX + (MeshSize.X * 0.6f), LocationY, 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                GenerateTile(Wall, WallTransform);
            }
            if(ColumnIndex == 0)
            {
                FRotator WallRotation(0.0f, 90.0f, 0.0f);
                FVector WallLocation(LocationX, LocationY - (MeshSize.Y * 0.6f), 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                GenerateTile(Wall, WallTransform);
            }
            if(ColumnIndex == SizeY-1)
            {
                FRotator WallRotation(0.0f, -90.0f, 0.0f);
                FVector WallLocation(LocationX, LocationY + (MeshSize.Y * 0.6f), 0.0f);
                FTransform WallTransform(WallRotation, WallLocation);
                GenerateTile(Wall, WallTransform);
            }

            // Add Corner pillars
            if(RowIndex == 0 && ColumnIndex == 0)
            {
                FRotator PillarCornerRotation(0.0f, 0.0f, 0.0f);
                FVector PillarCornerLocation(LocationX - (MeshSize.X * 0.5f), LocationY - (MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                GenerateTile(PillarCorner, PillarCornerTransform);
            }
            else if(RowIndex == 0 && ColumnIndex == SizeY-1)
            {
                FRotator PillarCornerRotation(0.0f, -90.0f, 0.0f);
                FVector PillarCornerLocation(LocationX - (MeshSize.X * 0.5f), LocationY + (MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                GenerateTile(PillarCorner, PillarCornerTransform);
            }
            else if(RowIndex == SizeX-1 && ColumnIndex == SizeY-1)
            {
                FRotator PillarCornerRotation(0.0f, 180.0f, 0.0f);
                FVector PillarCornerLocation(LocationX + (MeshSize.X * 0.5f), LocationY + (MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                GenerateTile(PillarCorner, PillarCornerTransform);
            }
            else if(RowIndex == SizeX-1 && ColumnIndex == 0)
            {
                FRotator PillarCornerRotation(0.0f, 90.0f, 0.0f);
                FVector PillarCornerLocation(LocationX + (MeshSize.X * 0.5f), LocationY - (MeshSize.Y * 0.5f), 0.0f);
                FTransform PillarCornerTransform(PillarCornerRotation, PillarCornerLocation);
                GenerateTile(PillarCorner, PillarCornerTransform);
            }
            ++TileIndex;
        }
    }
}

void ASunriseRoom::GenerateHallwayHorizontal(FRandomStream Stream, FVector Offset)
{
    int32 SizeX = 1;
    int32 SizeY = Stream.RandRange(1, MaxSize);

    int32 TileIndex = 0;
    for(size_t RowIndex = 0; RowIndex < SizeX; ++RowIndex)
    {
        for(size_t ColumnIndex = 0; ColumnIndex < SizeY; ++ColumnIndex)
        {
            int LocationX = (TileIndex / SizeY) * MeshSize.X + Offset.X;
            int LocationY = (TileIndex % SizeY) * MeshSize.Y + Offset.Y;

            // Add Floors
            FVector FloorLocation(LocationX, LocationY, 0.0f);
            FTransform FloorTransform(FloorLocation);
            GenerateTile(Floor, FloorTransform);

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
            }
            if(ColumnIndex == SizeY-1)
            {
                FName Name = "DoorOpeningEast";
                FVector DoorOpeningLocation(LocationX, LocationY + (MeshSize.Y * 0.5f), 0.0f);
                FVector Extents(MeshSize.X / 2.0f, MeshSize.Y / 2.0f, 500.0f);
                DoorOpeningEast = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
            }
            ++TileIndex;
        }
    }
}

void ASunriseRoom::GenerateHallwayVertical(FRandomStream Stream, FVector Offset)
{
    int32 SizeX = Stream.RandRange(1, MaxSize);
    int32 SizeY = 1;

    int32 TileIndex = 0;
    for(size_t RowIndex = 0; RowIndex < SizeX; ++RowIndex)
    {
        for(size_t ColumnIndex = 0; ColumnIndex < SizeY; ++ColumnIndex)
        {
            int LocationX = (TileIndex / SizeY) * MeshSize.X + Offset.X;
            int LocationY = (TileIndex % SizeY) * MeshSize.Y + Offset.Y;

            // Add Floors
            FVector FloorLocation(LocationX, LocationY, 0.0f);
            FTransform FloorTransform(FloorLocation);
            GenerateTile(Floor, FloorTransform);

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
            }
            if(RowIndex == SizeX-1)
            {
                FName Name = "DoorOpeningNorth";
                FVector DoorOpeningLocation(LocationX + (MeshSize.X * 0.5f), LocationY, 0.0f);
                FVector Extents(MeshSize.X / 2.0f, MeshSize.Y / 2.0f, 500.0f);
                DoorOpeningNorth = CreateTraceBox(this, Name, FRotator(0.0f), DoorOpeningLocation, Extents);
            }
            ++TileIndex;
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