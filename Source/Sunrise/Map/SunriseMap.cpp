// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseMap.h"
#include "SunriseRoom.h"
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
        MapTiles.Empty();

        // Initialize empty map
        if(MapSizeX % 2 == 0) ++MapSizeX;
        if(MapSizeY % 2 == 0) ++MapSizeY;
        
        for(size_t RowIndex = 0; RowIndex < MapSizeX; ++RowIndex)
        {
            for(size_t ColumnIndex = 0; ColumnIndex < MapSizeY; ++ColumnIndex)
            {
                int32 TileIndex = RowIndex * MapSizeY + ColumnIndex;
                int32 LocationX = (TileIndex / MapSizeY) * DefaultRoom->MeshSize.X;
                int32 LocationY = (TileIndex % MapSizeY) * DefaultRoom->MeshSize.Y;

                MapTiles.Add(FTile(ETile::None, RowIndex, ColumnIndex, FVector(LocationX, LocationY, 0.0f)));
            }
        }

        // Create path
        BinaryTreeMaze();

        // Add rooms
        DefaultRoom->AddRooms(Stream, MapSizeX, MapSizeY, Rooms, MapTiles);

        // Put outside of map back to None to the map.
        for(size_t RowIndex = 0; RowIndex < MapSizeX; ++RowIndex)
        {
            for(size_t ColumnIndex = 0; ColumnIndex < MapSizeY; ++ColumnIndex)
            {
                int32 TileIndex = RowIndex * MapSizeY + ColumnIndex;

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
                int32 TileIndex = RowIndex * MapSizeY + ColumnIndex;

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
                    if(MapTiles.IsValidIndex(TileIndex+MapSizeY))
                    {
                        if(MapTiles[TileIndex+MapSizeY].Type == ETile::Floor)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(CurrentRoom->MeshSize.X * 0.4, 0.0f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->Wall, TileTransform);
                        }
                    }
                    // South Wall
                    if(MapTiles.IsValidIndex(TileIndex-MapSizeY))
                    {
                        if(MapTiles[TileIndex-MapSizeY].Type == ETile::Floor)
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
                    if(MapTiles.IsValidIndex(TileIndex+MapSizeY) && MapTiles.IsValidIndex(TileIndex+1))
                    {
                        if(MapTiles[TileIndex+MapSizeY].Type == ETile::Floor && MapTiles[TileIndex+1].Type == ETile::Floor)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(CurrentRoom->MeshSize.X * 0.55f, CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }
                    // Northwest Pillar
                    if(MapTiles.IsValidIndex(TileIndex+MapSizeY) && MapTiles.IsValidIndex(TileIndex-1))
                    {
                        if(MapTiles[TileIndex+MapSizeY].Type == ETile::Floor && MapTiles[TileIndex-1].Type == ETile::Floor)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(CurrentRoom->MeshSize.X * 0.55f, -CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }
                    // Southeast Pillar
                    if(MapTiles.IsValidIndex(TileIndex-MapSizeY) && MapTiles.IsValidIndex(TileIndex+1))
                    {
                        if(MapTiles[TileIndex-MapSizeY].Type == ETile::Floor && MapTiles[TileIndex+1].Type == ETile::Floor)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(-CurrentRoom->MeshSize.X * 0.55f, CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }
                    // Southwest Pillar
                    if(MapTiles.IsValidIndex(TileIndex-MapSizeY) && MapTiles.IsValidIndex(TileIndex-1))
                    {
                        if(MapTiles[TileIndex-MapSizeY].Type == ETile::Floor && MapTiles[TileIndex-1].Type == ETile::Floor)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(-CurrentRoom->MeshSize.X * 0.55f, -CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }

                    // Render Inner Corner PIllars
                    // Northeast Pillar
                    if(MapTiles.IsValidIndex(TileIndex-MapSizeY-1) && MapTiles.IsValidIndex(TileIndex-MapSizeY) && MapTiles.IsValidIndex(TileIndex-1))
                    {
                        if(MapTiles[TileIndex-MapSizeY-1].Type == ETile::Floor && MapTiles[TileIndex-MapSizeY].Type == ETile::None && MapTiles[TileIndex-1].Type == ETile::None)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f),  
                                                       MapTiles[TileIndex].Location + FVector(-CurrentRoom->MeshSize.X * 0.55f, -CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }
                    // Northwest Pillar
                    if(MapTiles.IsValidIndex(TileIndex-MapSizeY+1) && MapTiles.IsValidIndex(TileIndex-MapSizeY) && MapTiles.IsValidIndex(TileIndex+1))
                    {
                        if(MapTiles[TileIndex-MapSizeY+1].Type == ETile::Floor && MapTiles[TileIndex-MapSizeY].Type == ETile::None && MapTiles[TileIndex+1].Type == ETile::None)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(-CurrentRoom->MeshSize.X * 0.55f, CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }
                    // Southeast Pillar
                    if(MapTiles.IsValidIndex(TileIndex+MapSizeY-1) && MapTiles.IsValidIndex(TileIndex+MapSizeY) && MapTiles.IsValidIndex(TileIndex-1))
                    {
                        if(MapTiles[TileIndex+MapSizeY-1].Type == ETile::Floor && MapTiles[TileIndex+MapSizeY].Type == ETile::None && MapTiles[TileIndex-1].Type == ETile::None)
                        {
                            TileTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), 
                                                       MapTiles[TileIndex].Location + FVector(CurrentRoom->MeshSize.X * 0.55f, -CurrentRoom->MeshSize.Y * 0.55f, 0.0f));
                            CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, TileTransform);
                        }
                    }
                    // Southwest Pillar
                    if(MapTiles.IsValidIndex(TileIndex+MapSizeY+1) && MapTiles.IsValidIndex(TileIndex+MapSizeY) && MapTiles.IsValidIndex(TileIndex+1))
                    {
                        if(MapTiles[TileIndex+MapSizeY+1].Type == ETile::Floor && MapTiles[TileIndex+MapSizeY].Type == ETile::None && MapTiles[TileIndex+1].Type == ETile::None)
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

        // Spawn End of Level
        // Reverse through map tiles until a valid endpoint is found
        for(int MapIndex = (MapSizeX * MapSizeY)-1; MapIndex > (MapSizeX * MapSizeY) / 2; --MapIndex)
        {
            int32 RandomTileEndIndex = MapIndex;

            // Opening to the left
            if(MapTiles[RandomTileEndIndex].Type == ETile::Floor && 
               MapTiles[RandomTileEndIndex+MapSizeY].Type == ETile::None && MapTiles[RandomTileEndIndex-MapSizeY].Type == ETile::None &&
               MapTiles[RandomTileEndIndex+1].Type == ETile::None && MapTiles[RandomTileEndIndex-1].Type == ETile::Floor)
            {
                CurrentRoom->GenerateTile(CurrentRoom->WallDoor, FTransform(FRotator(0.0f, 90.0f, 0.0f), MapTiles[RandomTileEndIndex].Location + FVector(0.0f, -500.0f, 0.0f)));
                CurrentRoom->GenerateTile(CurrentRoom->WallDoor, FTransform(FRotator(0.0f, 270.0f, 0.0f), MapTiles[RandomTileEndIndex].Location + FVector(0.0f, -500.0f, 0.0f)));
                CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomTileEndIndex].Location + FVector(500.0f, -500.0f, 0.0f)));
                CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomTileEndIndex].Location + FVector(-500.0f, -500.0f, 0.0f)));
                CurrentRoom->GenerateTile(CurrentRoom->Ceiling, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomTileEndIndex].Location + FVector(0.0f, 0.0f, 745.0f)));
                GetWorld()->SpawnActor<ASunriseDoor>(Door, FTransform(FRotator(0.0f, -90.0f, 0.0f), MapTiles[RandomTileEndIndex].Location + FVector(161.0f, -530.0f, 0.0f)));
                GetWorld()->SpawnActor<AActor>(EndLevelTrigger, FTransform(MapTiles[RandomTileEndIndex].Location + FVector(0.0f, 0.0f, 350.0f)));
                break;
            }

            // Opening to the bottom
            if(MapTiles[RandomTileEndIndex].Type == ETile::Floor && 
               MapTiles[RandomTileEndIndex+MapSizeY].Type == ETile::None && MapTiles[RandomTileEndIndex-MapSizeY].Type == ETile::Floor &&
               MapTiles[RandomTileEndIndex+1].Type == ETile::None && MapTiles[RandomTileEndIndex-1].Type == ETile::None)
            {
                CurrentRoom->GenerateTile(CurrentRoom->WallDoor, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomTileEndIndex].Location + FVector(-500.0f, 0.0f, 0.0f)));
                CurrentRoom->GenerateTile(CurrentRoom->WallDoor, FTransform(FRotator(0.0f, 180.0f, 0.0f), MapTiles[RandomTileEndIndex].Location + FVector(-500.0f, 0.0f, 0.0f)));
                CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomTileEndIndex].Location + FVector(-500.0f, 500.0f, 0.0f)));
                CurrentRoom->GenerateTile(CurrentRoom->PillarCorner, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomTileEndIndex].Location + FVector(-500.0f, -500.0f, 0.0f)));
                CurrentRoom->GenerateTile(CurrentRoom->Ceiling, FTransform(FRotator(0.0f, 0.0f, 0.0f), MapTiles[RandomTileEndIndex].Location + FVector(0.0f, 0.0f, 745.0f)));
                GetWorld()->SpawnActor<ASunriseDoor>(Door, FTransform(FRotator(0.0f, 180.0f, 0.0f), MapTiles[RandomTileEndIndex].Location + FVector(-530.0f, -161.0f, 0.0f)));
                GetWorld()->SpawnActor<AActor>(EndLevelTrigger, FTransform(MapTiles[RandomTileEndIndex].Location + FVector(0.0f, 0.0f, 350.0f)));
                break;
            }

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
    for(size_t RowIndex = 1; RowIndex < MapSizeX; RowIndex+=3)
    {
        for(size_t ColumnIndex = 1; ColumnIndex < MapSizeY; ColumnIndex+=3)
        {   
            TileIndex = RowIndex * MapSizeY + ColumnIndex;
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
                MapTiles[TileIndex-1-1].Type = ETile::Floor;
                continue;
            }
            // Go Vertical (South)
            if(ColumnIndex == 1)
            {
                MapTiles[TileIndex].Type = ETile::Floor;
                MapTiles[TileIndex-MapSizeY].Type = ETile::Floor;
                MapTiles[TileIndex-MapSizeY-MapSizeY].Type = ETile::Floor;
                continue;
            }

            // Dice Roll
            int32 DiceRoll = Stream.RandRange(0, 1);
            if(DiceRoll == 0)
            {
                // Go Horizontal (West)
                MapTiles[TileIndex].Type = ETile::Floor;
                MapTiles[TileIndex-1].Type = ETile::Floor;
                MapTiles[TileIndex-1-1].Type = ETile::Floor;
            }
            else
            {
                // Go Vertical (South)
                MapTiles[TileIndex].Type = ETile::Floor;
                MapTiles[TileIndex-MapSizeY].Type = ETile::Floor;
                MapTiles[TileIndex-MapSizeY-MapSizeY].Type = ETile::Floor;
            }
        }
    }
}

void ASunriseMap::SpawnPlayerStart()
{
    int32 PlayerStartIndex = Stream.RandRange(MapSizeY+1, (MapSizeY-1) * 2);

    FActorSpawnParameters SpawnParams;
    GetWorld()->SpawnActor<APlayerStart>(PlayerStart, FTransform(MapTiles[PlayerStartIndex].Location + FVector(0.0f, 0.0f, 110.f)));
}

void ASunriseMap::SpawnAICharacters(TSubclassOf<ACharacter> AICharacter)
{
    int32 Count = 0;
    while(Count < AISpawnCount)
    {
        int32 AICharacterStartIndex = Stream.RandRange((MapSizeY * 2) + 1, MapSizeX * MapSizeY - 1);
        if(MapTiles[AICharacterStartIndex].Type == ETile::Floor)
        {
            GetWorld()->SpawnActor<ASunriseAICharacter>(AICharacter, FTransform(MapTiles[AICharacterStartIndex].Location + FVector(0.0f, 0.0f, 250.f)));
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
        int32 RandomIndex = Stream.RandRange((MapSizeX * MapSizeY * 0.2f) - 1, (MapSizeX * MapSizeY) - 1);
        if(MapTiles[RandomIndex].Type == ETile::Floor)
        {
            GetWorld()->SpawnActor<AActor>(ActorToSpawn, FTransform(MapTiles[RandomIndex].Location + FVector(0.0f, 0.0f, 23.0f)));
            ++Count;
        }
    }
}

