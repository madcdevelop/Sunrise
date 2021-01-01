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
        TArray<AActor*> OutActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), Room, OutActors);
        for(auto Actor: OutActors)
        {
            Actor->Destroy();
        }
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

                MapTiles.Add(FTile(ETile::None, RowIndex, ColumnIndex, -1, -1, FVector(LocationX, LocationY, 0.0f), nullptr));
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

                if(MapTiles[TileIndex].Type == ETile::Floor)
                {
                    CurrentRoom->GenerateTile(CurrentRoom->Floor, FTransform(MapTiles[TileIndex].Location));
                }
                else if(MapTiles[TileIndex].Type == ETile::None || RowIndex == 0)
                {
                    // check 4 tiles around None Tile
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
                }
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
    for(size_t RowIndex = 1; RowIndex < MapSizeX; RowIndex+=2)
    {
        for(size_t ColumnIndex = 1; ColumnIndex < MapSizeY; ColumnIndex+=2)
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
                continue;
            }
            // Go Vertical (South)
            if(ColumnIndex == 1)
            {
                MapTiles[TileIndex].Type = ETile::Floor;
                MapTiles[TileIndex-MapSizeY].Type = ETile::Floor;
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
                MapTiles[TileIndex-MapSizeY].Type = ETile::Floor;
            }
        }
    }
}

void ASunriseMap::AddRooms(ASunriseRoom* DefaultRoom)
{
    for(size_t RoomCount = 0; RoomCount < Rooms; ++RoomCount)
    {
        int32 RoomSizeX = Stream.RandRange(DefaultRoom->MinSize, DefaultRoom->MaxSize);
        int32 RoomSizeY = Stream.RandRange(DefaultRoom->MinSize, DefaultRoom->MaxSize);
        int32 RoomTileIndex = Stream.RandRange(0, (MapSizeX * MapSizeY)-1);

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