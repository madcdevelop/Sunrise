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


        if(MapSizeX % 2 == 0) ++MapSizeX;
        if(MapSizeY % 2 == 0) ++MapSizeY;

        // Initialize empty map
        int TileIndex = 0;
        for(size_t RowIndex = 0; RowIndex < MapSizeX; ++RowIndex)
        {
            for(size_t ColumnIndex = 0; ColumnIndex < MapSizeY; ++ColumnIndex)
            {
                int LocationX = (TileIndex / MapSizeY) * DefaultRoom->MeshSize.X;
                int LocationY = (TileIndex % MapSizeY) * DefaultRoom->MeshSize.Y;

                MapTiles.Add(FTile(ETile::None, RowIndex, ColumnIndex, -1, -1, FVector(LocationX, LocationY, 0.0f), nullptr));
                ++TileIndex;
            }
        }

        // Create path
        BinaryTreeMaze();

        // Add rooms
        for(size_t RoomCount = 0; RoomCount < Rooms; ++RoomCount)
        {
            int32 RoomSizeX = Stream.RandRange(DefaultRoom->MinSize, DefaultRoom->MaxSize);
            int32 RoomSizeY = Stream.RandRange(DefaultRoom->MinSize, DefaultRoom->MaxSize);
            int32 StartTileIndex = Stream.RandRange(0, (MapSizeX * MapSizeY)-1);

            // Loop through starting at a specific tile index
            int32 RoomTileIndex = StartTileIndex;
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
        
        // Render Map
        FActorSpawnParameters SpawnParams;
        FVector LocationOffset(0.0f, 0.0f, 0.0f);
        ASunriseRoom* CurrentRoom = GetWorld()->SpawnActor<ASunriseRoom>(DefaultRoom->GetClass(), LocationOffset, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);

        int TileIndexRender = 0;
        for(size_t RowIndex = 0; RowIndex < MapSizeX; ++RowIndex)
        {
            for(size_t ColumnIndex = 0; ColumnIndex < MapSizeY; ++ColumnIndex)
            {
                int LocationX = (TileIndexRender / MapSizeY) * DefaultRoom->MeshSize.X;
                int LocationY = (TileIndexRender % MapSizeY) * DefaultRoom->MeshSize.Y;

                if(MapTiles[TileIndexRender].Type == ETile::Floor)
                {
                    CurrentRoom->GenerateFloor(MapTiles[TileIndexRender].Location);
                }
                ++TileIndexRender;
            }
        }

        // Spawn Tile to create next room
    //     FActorSpawnParameters SpawnParams;
    //     FVector LocationOffset(0.0f, 0.0f, 0.0f);
    //     ASunriseRoom* CurrentRoom = GetWorld()->SpawnActor<ASunriseRoom>(DefaultRoom->GetClass(), LocationOffset, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);

        
    //     if(CurrentRoom) CurrentRoom->GenerateRoom(MapTiles, 0, MapSizeY, Stream, LocationOffset);

    //     if(CurrentRoom->DoorOpeningEast) 
    //     {
    //         LocationOffset = CurrentRoom->DoorOpeningEast->GetComponentLocation() + FVector(0.0f, CurrentRoom->MeshSize.Y / 2.0f, 0.0f);
    //         ASunriseRoom* CurrentHallway = GetWorld()->SpawnActor<ASunriseRoom>(DefaultRoom->GetClass(), LocationOffset, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
    //         if(CurrentHallway) CurrentHallway->GenerateHallwayHorizontal(Stream, LocationOffset);
    //     }

    //     if(CurrentRoom->DoorOpeningNorth)
    //     {
    //         LocationOffset = CurrentRoom->DoorOpeningNorth->GetComponentLocation() + FVector(CurrentRoom->MeshSize.X / 2.0f, 0.0f, 0.0f);
    //         ASunriseRoom* CurrentHallway = GetWorld()->SpawnActor<ASunriseRoom>(DefaultRoom->GetClass(), LocationOffset, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
    //         if(CurrentHallway) CurrentHallway->GenerateHallwayVertical(Stream, LocationOffset);
    //     }

    //     if(CurrentRoom->DoorOpeningWest) 
    //     {
    //         LocationOffset = FVector(0.0f, 0.0f, 0.0f);
    //         ASunriseRoom* CurrentHallway = GetWorld()->SpawnActor<ASunriseRoom>(DefaultRoom->GetClass(), LocationOffset, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
    //         if(CurrentHallway) CurrentHallway->GenerateHallwayHorizontal(Stream, LocationOffset);
            
    //         // Move Hallway
    //         CurrentHallway->Floor->SetRelativeRotation(FQuat(FRotator(0.0f, 180.0f, 0.0f)));
    //         CurrentHallway->Wall->SetRelativeRotation(FQuat(FRotator(0.0f, 180.0f, 0.0f)));
    //         CurrentHallway->SetActorRotation(FRotator(0.0f, 180.0, 0.0f));
    //         CurrentHallway->Floor->SetRelativeLocation(CurrentRoom->DoorOpeningWest->GetComponentLocation() - FVector(0.0f, CurrentRoom->MeshSize.Y / 2.0f, 0.0f));
    //         CurrentHallway->Wall->SetRelativeLocation(CurrentRoom->DoorOpeningWest->GetComponentLocation() - FVector(0.0f, CurrentRoom->MeshSize.Y / 2.0f, 0.0f));
    //         CurrentHallway->SetActorLocation(CurrentRoom->DoorOpeningWest->GetComponentLocation() - FVector(0.0f, CurrentRoom->MeshSize.Y / 2.0f, 0.0f));
    //     }

    //     if(CurrentRoom->DoorOpeningSouth)
    //     {
    //         LocationOffset = FVector(0.0f, 0.0f, 0.0f);
    //         ASunriseRoom* CurrentHallway = GetWorld()->SpawnActor<ASunriseRoom>(DefaultRoom->GetClass(), LocationOffset, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
    //         if(CurrentHallway) CurrentHallway->GenerateHallwayVertical(Stream, LocationOffset);

    //         // Move Hallway
    //         CurrentHallway->Floor->SetRelativeRotation(FQuat(FRotator(0.0f, 180.0f, 0.0f)));
    //         CurrentHallway->Wall->SetRelativeRotation(FQuat(FRotator(0.0f, 180.0f, 0.0f)));
    //         CurrentHallway->SetActorRotation(FRotator(0.0f, 180.0, 0.0f));
    //         CurrentHallway->Floor->SetRelativeLocation(CurrentRoom->DoorOpeningSouth->GetComponentLocation() - FVector(CurrentRoom->MeshSize.X / 2.0f, 0.0f, 0.0f));
    //         CurrentHallway->Wall->SetRelativeLocation(CurrentRoom->DoorOpeningSouth->GetComponentLocation() - FVector(CurrentRoom->MeshSize.X / 2.0f, 0.0f, 0.0f));
    //         CurrentHallway->SetActorLocation(CurrentRoom->DoorOpeningSouth->GetComponentLocation() - FVector(CurrentRoom->MeshSize.X / 2.0f, 0.0f, 0.0f));
    //     }
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
    *   Steps
    *       - For each existing cell in the grid:
    *           - 1. Get if they exist, north or west neighbors.
    *           - 2. Toss a coin to connect one of them.
    *       - It is already done!
    */
    


    for(size_t RowIndex = 0; RowIndex < MapSizeX; RowIndex+=2)
    {
        for(size_t ColumnIndex = 0; ColumnIndex < MapSizeY; ColumnIndex+=2)
        {   
            int32 TileIndex = RowIndex * MapSizeY + ColumnIndex;
            // start tile
            if(RowIndex == 0 && ColumnIndex == 0)
            {
                MapTiles[TileIndex].Type = ETile::Floor;
                TileIndex+=2;
                continue;
            }
            // Go Horizontal (West)
            if(RowIndex == 0)
            {
                MapTiles[TileIndex].Type = ETile::Floor;
                MapTiles[TileIndex-1].Type = ETile::Floor;
                TileIndex+=2;
                continue;
            }
            // Go Vertical (South)
            if(ColumnIndex == 0)
            {
                MapTiles[TileIndex].Type = ETile::Floor;
                MapTiles[TileIndex-MapSizeY].Type = ETile::Floor;
                TileIndex+=2;
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
            TileIndex+=2;
        }
    }
}

void ASunriseMap::AddRooms(int32 RoomCount)
{
    

}
