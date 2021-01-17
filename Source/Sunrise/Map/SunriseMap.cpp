// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseMap.h"
#include "SunriseRoom.h"
#include "../Classes/Kismet/GameplayStatics.h"

// Sets default values
ASunriseMap::ASunriseMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    MaxMapSizeX = 25;
    MaxMapSizeY = 25;

    for(size_t RowIndex = 0; RowIndex < MaxMapSizeX; ++RowIndex)
    {
        for(size_t ColumnIndex = 0; ColumnIndex < MaxMapSizeY; ++ColumnIndex)
        {
            int32 TileIndex = RowIndex * MaxMapSizeY + ColumnIndex;
            // @CONSTANTS: Find way to not use constant for tile size.
            int32 LocationX = (TileIndex / MaxMapSizeY) * 1000.0f;
            int32 LocationY = (TileIndex % MaxMapSizeY) * 1000.0f;

            MapTiles.Add(FTile(ETile::None, RowIndex, ColumnIndex, FVector(LocationX, LocationY, 0.0f)));
        }
    }
}

// Called when the game starts or when spawned
void ASunriseMap::BeginPlay()
{
	Super::BeginPlay();

    Seed = FMath::RandRange(1, 900000);
    Stream.Initialize(Seed);
    GenerateMap();
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
    ASunriseRoom* DefaultRoom = Cast<ASunriseRoom>(Room->GetDefaultObject());
    if(DefaultRoom)
    {
        // Look for all actors and objects that exist for map generation and delete
        DeleteActors(Room);
        DeleteActors(PlayerStart);
        DeleteActors(MeleeAICharacter);
        DeleteActors(Door);
        DeleteActors(GoldenKey);
        DeleteActors(EndLevelTrigger);

        // Initialize empty map
        if(MapSizeX % 2 == 0) ++MapSizeX;
        if(MapSizeY % 2 == 0) ++MapSizeY;
        
        // reset map on next map generation
        for(size_t RowIndex = 0; RowIndex < MaxMapSizeX; ++RowIndex)
        {
            for(size_t ColumnIndex = 0; ColumnIndex < MaxMapSizeY; ++ColumnIndex)
            {
                int32 TileIndex = RowIndex * MaxMapSizeY + ColumnIndex;

                MapTiles[TileIndex].Type = ETile::None;
            }
        }

        // Create path
        BinaryTreeMaze();

        // Add rooms
        DefaultRoom->AddRooms(Stream, MapSizeX, MapSizeY, Rooms, MapTiles);

        // Put outside of map back to None to the map. Rooms hit edges.
        for(size_t RowIndex = 0; RowIndex < MapSizeX; ++RowIndex)
        {
            for(size_t ColumnIndex = 0; ColumnIndex < MapSizeY; ++ColumnIndex)
            {
                int32 TileIndex = RowIndex * MaxMapSizeY + ColumnIndex;

                if(RowIndex == 0 || ColumnIndex == 0 || RowIndex == MapSizeX-1 || ColumnIndex == MapSizeY-1)
                {
                    MapTiles[TileIndex].Type = ETile::None;
                }
            }
        }
        
        // Render Map
        FActorSpawnParameters SpawnParams;
        FVector LocationOffset(0.0f, 0.0f, 0.0f);
        ASunriseRoom* CurrentRoom = GetWorld()->SpawnActor<ASunriseRoom>(DefaultRoom->GetClass(), LocationOffset, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);

        for(size_t RowIndex = 0; RowIndex < MapSizeX; ++RowIndex)
        {
            for(size_t ColumnIndex = 0; ColumnIndex < MapSizeY; ++ColumnIndex)
            {
                int32 TileIndex = RowIndex * MaxMapSizeY + ColumnIndex;

                // Render Floor
                if(MapTiles[TileIndex].Type == ETile::Floor)
                {
                    CurrentRoom->GenerateTile(CurrentRoom->Floor, FTransform(MapTiles[TileIndex].Location));
                }
                // Render Walls
                else if(MapTiles[TileIndex].Type == ETile::None)
                {
                    CurrentRoom->GenerateTile(CurrentRoom->Ceiling, FTransform(MapTiles[TileIndex].Location + FVector(0.0f, 0.0f, CurrentRoom->MeshSize.Z)));
                    
                    // North Wall
                    FTransform TileTransform;
                    if(MapTiles.IsValidIndex(TileIndex+MaxMapSizeY))
                    {
                        if(MapTiles[TileIndex+MaxMapSizeY].Type == ETile::Floor)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(CurrentRoom->MeshSize.X * 0.4, 0.0f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->Wall, TileTransform);
                        }
                    }
                    // South Wall
                    if(MapTiles.IsValidIndex(TileIndex-MaxMapSizeY))
                    {
                        if(MapTiles[TileIndex-MaxMapSizeY].Type == ETile::Floor)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 180.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location - FVector(CurrentRoom->MeshSize.X * 0.4, 0.0f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->Wall, TileTransform);
                        }
                    }
                    // East Wall
                    if(MapTiles.IsValidIndex(TileIndex+1))
                    {
                        if(MapTiles[TileIndex+1].Type == ETile::Floor)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 90.0f, 0.0f),
                                                       MapTiles[TileIndex].Location + FVector(0.0f, CurrentRoom->MeshSize.Y * 0.4, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->Wall, TileTransform);
                        }
                    }
                    // West Wall
                    if(MapTiles.IsValidIndex(TileIndex-1))
                    {
                        if(MapTiles[TileIndex-1].Type == ETile::Floor)
                        {
                            TileTransform = FTransform(FRotator(0.0f, -90.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location - FVector(0.0f, CurrentRoom->MeshSize.Y * 0.4, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->Wall, TileTransform);
                        }
                    }

                    // Render Outer Corner Pillars
                    // Northeast Pillar
                    if(MapTiles.IsValidIndex(TileIndex+MaxMapSizeY) && MapTiles.IsValidIndex(TileIndex+1))
                    {
                        if(MapTiles[TileIndex+MaxMapSizeY].Type == ETile::Floor && MapTiles[TileIndex+1].Type == ETile::Floor)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(CurrentRoom->MeshSize.X * 0.55f, CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }
                    // Northwest Pillar
                    if(MapTiles.IsValidIndex(TileIndex+MaxMapSizeY) && MapTiles.IsValidIndex(TileIndex-1))
                    {
                        if(MapTiles[TileIndex+MaxMapSizeY].Type == ETile::Floor && MapTiles[TileIndex-1].Type == ETile::Floor)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(CurrentRoom->MeshSize.X * 0.55f, -CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }
                    // Southeast Pillar
                    if(MapTiles.IsValidIndex(TileIndex-MaxMapSizeY) && MapTiles.IsValidIndex(TileIndex+1))
                    {
                        if(MapTiles[TileIndex-MaxMapSizeY].Type == ETile::Floor && MapTiles[TileIndex+1].Type == ETile::Floor)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(-CurrentRoom->MeshSize.X * 0.55f, CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }
                    // Southwest Pillar
                    if(MapTiles.IsValidIndex(TileIndex-MaxMapSizeY) && MapTiles.IsValidIndex(TileIndex-1))
                    {
                        if(MapTiles[TileIndex-MaxMapSizeY].Type == ETile::Floor && MapTiles[TileIndex-1].Type == ETile::Floor)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(-CurrentRoom->MeshSize.X * 0.55f, -CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }

                    // Render Inner Corner PIllars
                    // Northeast Pillar
                    if(MapTiles.IsValidIndex(TileIndex-MaxMapSizeY-1) && MapTiles.IsValidIndex(TileIndex-MaxMapSizeY) && MapTiles.IsValidIndex(TileIndex-1))
                    {
                        if(MapTiles[TileIndex-MaxMapSizeY-1].Type == ETile::Floor && MapTiles[TileIndex-MaxMapSizeY].Type == ETile::None && MapTiles[TileIndex-1].Type == ETile::None)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f),  
                                                       MapTiles[TileIndex].Location + FVector(-CurrentRoom->MeshSize.X * 0.55f, -CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }
                    // Northwest Pillar
                    if(MapTiles.IsValidIndex(TileIndex-MaxMapSizeY+1) && MapTiles.IsValidIndex(TileIndex-MaxMapSizeY) && MapTiles.IsValidIndex(TileIndex+1))
                    {
                        if(MapTiles[TileIndex-MaxMapSizeY+1].Type == ETile::Floor && MapTiles[TileIndex-MaxMapSizeY].Type == ETile::None && MapTiles[TileIndex+1].Type == ETile::None)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(-CurrentRoom->MeshSize.X * 0.55f, CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }
                    // Southeast Pillar
                    if(MapTiles.IsValidIndex(TileIndex+MaxMapSizeY-1) && MapTiles.IsValidIndex(TileIndex+MaxMapSizeY) && MapTiles.IsValidIndex(TileIndex-1))
                    {
                        if(MapTiles[TileIndex+MaxMapSizeY-1].Type == ETile::Floor && MapTiles[TileIndex+MaxMapSizeY].Type == ETile::None && MapTiles[TileIndex-1].Type == ETile::None)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(CurrentRoom->MeshSize.X * 0.55f, -CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }
                    // Southwest Pillar
                    if(MapTiles.IsValidIndex(TileIndex+MaxMapSizeY+1) && MapTiles.IsValidIndex(TileIndex+MaxMapSizeY) && MapTiles.IsValidIndex(TileIndex+1))
                    {
                        if(MapTiles[TileIndex+MaxMapSizeY+1].Type == ETile::Floor && MapTiles[TileIndex+MaxMapSizeY].Type == ETile::None && MapTiles[TileIndex+1].Type == ETile::None)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(CurrentRoom->MeshSize.X * 0.55f, CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }
                }
            }
        }

        // Spawn
        SpawnPlayerStart();
        SpawnAICharacters(MeleeAICharacter);
        SpawnActors(3, GoldenKey);

        // Spawn end door to exit level
        // Find Available spawns
        TArray<int32> AvailableSpawns;
        for(int MapTileIndex = (MapSizeX * MapSizeY)-1; MapTileIndex > MaxMapSizeY * 0.9f; --MapTileIndex)
        {
            // Opening to left corridor
            if(MapTiles[MapTileIndex].Type == ETile::Floor && 
               MapTiles[MapTileIndex+MaxMapSizeY].Type == ETile::None && MapTiles[MapTileIndex-MaxMapSizeY].Type == ETile::None &&
               MapTiles[MapTileIndex+1].Type == ETile::None && MapTiles[MapTileIndex-1].Type == ETile::Floor)
            {
                AvailableSpawns.Add(MapTileIndex);
            }
            // Opening to bottom corridor
            else if(MapTiles[MapTileIndex].Type == ETile::Floor && 
               MapTiles[MapTileIndex+MaxMapSizeY].Type == ETile::None && MapTiles[MapTileIndex-MaxMapSizeY].Type == ETile::Floor &&
               MapTiles[MapTileIndex+1].Type == ETile::None && MapTiles[MapTileIndex-1].Type == ETile::None)
            {
                AvailableSpawns.Add(MapTileIndex);
            }
        }

        // Find only if there are no corridors to spawn end room
        if(AvailableSpawns.Num() == 0)
        {
            for(int MapTileIndex = (MapSizeX * MapSizeY)-1; MapTileIndex > MaxMapSizeY * 0.9f; --MapTileIndex)
            {
                // Opening to left wall
                if(MapTiles[MapTileIndex].Type == ETile::Floor && 
                MapTiles[MapTileIndex+MaxMapSizeY].Type == ETile::Floor && MapTiles[MapTileIndex-MaxMapSizeY].Type == ETile::Floor &&
                MapTiles[MapTileIndex+1].Type == ETile::None && MapTiles[MapTileIndex-1].Type == ETile::Floor)
                {
                    AvailableSpawns.Add(MapTileIndex);
                }
                // Opening to bottom wall
                else if(MapTiles[MapTileIndex].Type == ETile::Floor && 
                MapTiles[MapTileIndex+MaxMapSizeY].Type == ETile::None && MapTiles[MapTileIndex-MaxMapSizeY].Type == ETile::Floor &&
                MapTiles[MapTileIndex+1].Type == ETile::Floor && MapTiles[MapTileIndex-1].Type == ETile::Floor)
                {
                    AvailableSpawns.Add(MapTileIndex);
                }
            }
        }

        // spawn end room
        int32 MaxIndex = AvailableSpawns.Num()-1;
        int32 RandomIndex = Stream.RandRange(0, MaxIndex);
        int32 RandomSpawnIndex = AvailableSpawns[RandomIndex];

        // Opening to left corridor
        if(MapTiles[RandomSpawnIndex].Type == ETile::Floor && 
            MapTiles[RandomSpawnIndex+MaxMapSizeY].Type == ETile::None && MapTiles[RandomSpawnIndex-MaxMapSizeY].Type == ETile::None &&
            MapTiles[RandomSpawnIndex+1].Type == ETile::None && MapTiles[RandomSpawnIndex-1].Type == ETile::Floor)
        {
            CurrentRoom->GenerateTile(CurrentRoom->WallDoor, FTransform(FRotator(0.0f, 90.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(0.0f, -500.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->WallDoor, FTransform(FRotator(0.0f, 270.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(0.0f, -500.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(500.0f, -500.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(-500.0f, -500.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->Ceiling, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(0.0f, 0.0f, 745.0f)));
            GetWorld()->SpawnActor<ASunriseDoor>(Door, FTransform(FRotator(0.0f, -90.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(161.0f, -530.0f, 0.0f)));
            GetWorld()->SpawnActor<AActor>(EndLevelTrigger, FTransform(MapTiles[RandomSpawnIndex].Location + FVector(0.0f, 0.0f, 350.0f)));
        }
        // Opening to left wall
        else if(MapTiles[RandomSpawnIndex].Type == ETile::Floor && 
            MapTiles[RandomSpawnIndex+MaxMapSizeY].Type == ETile::Floor && MapTiles[RandomSpawnIndex-MaxMapSizeY].Type == ETile::Floor &&
            MapTiles[RandomSpawnIndex+1].Type == ETile::None && MapTiles[RandomSpawnIndex-1].Type == ETile::Floor)
        {
            CurrentRoom->GenerateTile(CurrentRoom->WallDoor, FTransform(FRotator(0.0f, 90.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(0.0f, -500.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->WallDoor, FTransform(FRotator(0.0f, 270.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(0.0f, -500.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(500.0f, -500.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(-500.0f, -500.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->Ceiling, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(0.0f, 0.0f, 745.0f)));
            GetWorld()->SpawnActor<ASunriseDoor>(Door, FTransform(FRotator(0.0f, -90.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(161.0f, -530.0f, 0.0f)));
            GetWorld()->SpawnActor<AActor>(EndLevelTrigger, FTransform(MapTiles[RandomSpawnIndex].Location + FVector(0.0f, 0.0f, 350.0f)));
        }
        // Opening to bottom corridor
        else if(MapTiles[RandomSpawnIndex].Type == ETile::Floor && 
            MapTiles[RandomSpawnIndex+MaxMapSizeY].Type == ETile::None && MapTiles[RandomSpawnIndex-MaxMapSizeY].Type == ETile::Floor &&
            MapTiles[RandomSpawnIndex+1].Type == ETile::None && MapTiles[RandomSpawnIndex-1].Type == ETile::None)
        {
            CurrentRoom->GenerateTile(CurrentRoom->WallDoor, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(-500.0f, 0.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->WallDoor, FTransform(FRotator(0.0f, 180.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(-500.0f, 0.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(-500.0f, 500.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(-500.0f, -500.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->Ceiling, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(0.0f, 0.0f, 745.0f)));
            GetWorld()->SpawnActor<ASunriseDoor>(Door, FTransform(FRotator(0.0f, 180.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(-530.0f, -161.0f, 0.0f)));
            GetWorld()->SpawnActor<AActor>(EndLevelTrigger, FTransform(MapTiles[RandomSpawnIndex].Location + FVector(0.0f, 0.0f, 350.0f)));
        }
        // Opening to bottom wall
        else if(MapTiles[RandomSpawnIndex].Type == ETile::Floor && 
            MapTiles[RandomSpawnIndex+MaxMapSizeY].Type == ETile::None && MapTiles[RandomSpawnIndex-MaxMapSizeY].Type == ETile::Floor &&
            MapTiles[RandomSpawnIndex+1].Type == ETile::Floor && MapTiles[RandomSpawnIndex-1].Type == ETile::Floor)
        {
            CurrentRoom->GenerateTile(CurrentRoom->WallDoor, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(-500.0f, 0.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->WallDoor, FTransform(FRotator(0.0f, 180.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(-500.0f, 0.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(-500.0f, 500.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(-500.0f, -500.0f, 0.0f)));
            CurrentRoom->GenerateTile(CurrentRoom->Ceiling, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(0.0f, 0.0f, 745.0f)));
            GetWorld()->SpawnActor<ASunriseDoor>(Door, FTransform(FRotator(0.0f, 180.0f, 0.0f), MapTiles[RandomSpawnIndex].Location + FVector(-530.0f, -161.0f, 0.0f)));
            GetWorld()->SpawnActor<AActor>(EndLevelTrigger, FTransform(MapTiles[RandomSpawnIndex].Location + FVector(0.0f, 0.0f, 350.0f)));
        }

    }
}

void ASunriseMap::BinaryTreeMaze()
{
    /*
    *   Binary Tree Algorithm
    *   Reference:
    *       - https://medium.com/analytics-vidhya/maze-generations-algorithms-and-visualizations-9f5e88a3ae37
    *       - http://weblog.jamisbuck.org/2011/2/1/maze-generation-binary-tree-algorithm
    *
    *   For each step choose between two possible options
    *       - For each cell in the grid, toss a coin to decide whether to carve a passage south or west
    */
    int32 TileIndex = 0;
    for(size_t RowIndex = 1; RowIndex < MapSizeX; RowIndex+=2)
    {
        for(size_t ColumnIndex = 1; ColumnIndex < MapSizeY; ColumnIndex+=2)
        {   
            TileIndex = RowIndex * MaxMapSizeY + ColumnIndex;
            // start tile
            if(RowIndex == 1 && ColumnIndex == 1)
            {
                MapTiles[TileIndex].Type = ETile::Floor;
                continue;
            }
            // Go Horizontal (West)
            if(RowIndex == 1)
            {
                MapTiles[TileIndex].Type = ETile::Floor;
                MapTiles[TileIndex-1].Type = ETile::Floor;
                continue;
            }
            // Go Vertical (South)
            if(ColumnIndex == 1)
            {
                MapTiles[TileIndex].Type = ETile::Floor;
                MapTiles[TileIndex-MaxMapSizeY].Type = ETile::Floor;
                continue;
            }

            // Dice Roll
            int32 DiceRoll = Stream.RandRange(0, 1);
            if(DiceRoll == 0)
            {
                // Go Horizontal (West)
                MapTiles[TileIndex].Type = ETile::Floor;
                MapTiles[TileIndex-1].Type = ETile::Floor;
            }
            else
            {
                // Go Vertical (South)
                MapTiles[TileIndex].Type = ETile::Floor;
                MapTiles[TileIndex-MaxMapSizeY].Type = ETile::Floor;
            }
        }
    }
}

void ASunriseMap::SpawnPlayerStart()
{
    int32 RandomRowIndex = 1;
    int32 RandomColumnIndex = Stream.RandRange(1, MapSizeY-1);
    int32 PlayerStartIndex = RandomRowIndex * MaxMapSizeY + RandomColumnIndex;

    FActorSpawnParameters SpawnParams;
    GetWorld()->SpawnActor<APlayerStart>(PlayerStart, FTransform(MapTiles[PlayerStartIndex].Location + FVector(0.0f, 0.0f, 110.f)));
}

void ASunriseMap::SpawnAICharacters(TSubclassOf<ACharacter> AICharacter)
{
    // @TODO: Use crowd manager max agents instead of hardcoded value
    // Match with crowd manager settings
    int32 MaxSpawnCount = 200;
    checkf(AISpawnCount <= MaxSpawnCount, TEXT("The number of agents (%d) spawning is greater than Max Agents (%d) set in Crowd Manager settings. Some agents will not move."), AISpawnCount, MaxSpawnCount)

    int32 Count = 0;
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    while(Count < AISpawnCount)
    {
        // Never spawn in 1st row where player spawns
        int32 RandomRowIndex = Stream.RandRange(2, MapSizeX-1);
        int32 RandomColumnIndex = Stream.RandRange(1, MapSizeY-1);
        int32 AICharacterStartIndex = RandomRowIndex * MaxMapSizeY + RandomColumnIndex;
        int32 TileRandomX = Stream.RandRange(1, 450);
        int32 TileRandomY = Stream.RandRange(1, 450);
        if(MapTiles[AICharacterStartIndex].Type == ETile::Floor)
        {
            GetWorld()->SpawnActor<ASunriseAICharacter>(AICharacter, FTransform(MapTiles[AICharacterStartIndex].Location + FVector(TileRandomX, TileRandomY, 250.f)), SpawnParams);
            ++Count;
        }

    }
}

void ASunriseMap::DeleteActors(TSubclassOf<AActor> ActorToDelete)
{
    TArray<AActor*> OutActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActorToDelete, OutActors);
    for(auto Actor: OutActors)
    {
        Actor->Destroy();
    }    
}

void ASunriseMap::SpawnActors(int32 ActorSpawnCount, TSubclassOf<AActor> ActorToSpawn)
{
    int32 Count = 0;
    while(Count < ActorSpawnCount)
    {
        // Never spawn in 1st row where player spawns
        int32 RandomRowIndex = Stream.RandRange(2, MapSizeX-1);
        int32 RandomColumnIndex = Stream.RandRange(1, MapSizeY-1);
        int32 RandomIndex = RandomRowIndex * MaxMapSizeY + RandomColumnIndex;
        if(MapTiles[RandomIndex].Type == ETile::Floor)
        {
            GetWorld()->SpawnActor<AActor>(ActorToSpawn, FTransform(MapTiles[RandomIndex].Location + FVector(0.0f, 0.0f, 23.0f)));
            ++Count;
        }
    }
}

