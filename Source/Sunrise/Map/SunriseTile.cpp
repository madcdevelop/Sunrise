// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseTile.h"
#include "../Character/SunrisePlayerCharacter.h"
#include "../Classes/GameFramework/PlayerStart.h"

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
    Door  = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Door"));

    // Tile Defaults
    MaxX = 5;
    MaxY = 5;
    TileSize = 1000;
}

// Called when the game starts or when spawned
void ASunriseTile::BeginPlay()
{
	Super::BeginPlay();

    GenerateMazeBinaryTreeRoom();
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

void ASunriseTile::GenerateMazeTiles()
{
    int32 Area  = MaxX * MaxY;
    int32 Start = MaxY / 2;
    int32 End   = Area - Start;

    // Map
    TArray<int32> TileType;
    for(size_t i = 0; i < Area; ++i)
    {
        if(i == 0 || i == Start || i == End) TileType.Add(0);
        else TileType.Add(FMath::RandRange(0, 1));
    }

    // Spawn
    int32 TileIndex = 0;
    for(const auto &Type : TileType)
    {
        int32 X = (TileIndex / MaxY) * TileSize;
        int32 Y = (TileIndex % MaxY) * TileSize;

        FVector TileLocation(X, Y, 0.0f);
        FTransform TileTransform(TileLocation);

        if(Start == TileIndex) 
        {
            Floor->AddInstance(TileTransform);
            FActorSpawnParameters SpawnParams;
            GetWorld()->SpawnActor<APlayerStart>(PlayerStart, TileTransform, SpawnParams);
            ASunrisePlayerCharacter* PlayerChar = Cast<ASunrisePlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
            PlayerChar->SetActorLocation(TileLocation);
        }
        else if (Type == 0)
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

void ASunriseTile::GenerateMazeBinaryTree()
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

    // Maze generation requires odd number of tiles
    if(MaxX % 2 == 0) ++MaxX;
    if(MaxY % 2 == 0) ++MaxY;

    int32 Area = MaxX * MaxY;
    int32 Start = FMath::RandRange(1, MaxY-2);

    USTRUCT()
    struct TilesArray {
        TArray<int32> Tiles;

        int32 operator[](int32 Index) {
            return Tiles[Index];
        }

        void Replace(int32 Index, int32 Type) {
            Tiles[Index] = Type;
        }
    };

    TArray<TilesArray> TileType;
    TilesArray TileTypeCol;
    
    TileTypeCol.Tiles.Init(1, MaxY);
    TileType.Init(TileTypeCol, MaxX);

    // if RowIndex 0 or MaxX-1 or MaxY-1 then skip as those are all walls
    for(size_t RowIndex = 1; RowIndex < MaxX; RowIndex+=2)
    {
        for(size_t ColIndex = 1; ColIndex < MaxY; ColIndex+=2)
        {
            // Start tile
            if (RowIndex-1 == 0 && ColIndex-1 == 0) {
                TileType[RowIndex].Replace(ColIndex, 0);
                continue;
            }
            // Horizontal (West)
            if (RowIndex-1 == 0){
                TileType[RowIndex].Replace(ColIndex, 0);
                TileType[RowIndex].Replace(ColIndex-1, 0);
                continue;
            }
            // Vertical (South)
            if (ColIndex-1 == 0) {
                TileType[RowIndex].Replace(ColIndex, 0);
                TileType[RowIndex-1].Replace(ColIndex, 0);
                continue;
            }
            
            int32 DiceRoll = FMath::RandRange(0, 100);
            if (DiceRoll < 50)
            {
                // Horizontal (West)
                TileType[RowIndex].Replace(ColIndex, 0);
                TileType[RowIndex].Replace(ColIndex-1, 0);
            }
            else
            {
                // Vertical (South)
                TileType[RowIndex].Replace(ColIndex, 0);
                TileType[RowIndex-1].Replace(ColIndex, 0);
            }
        }
    }


    // End of Maze
    int32 Random = FMath::RandRange(1,MaxY-1);
    while(true)
    {
        if(0 == TileType[MaxX-2][Random]) 
        {
            TileType[MaxX-1].Replace(Random, 0);
            break;
        }
        else
        {
            Random = FMath::RandRange(1,MaxY-1);
        }
    }

    
    // Spawn
    int32 TileIndex = 0;
    for(const auto &Row : TileType)
    {
        for(const auto &Type : Row.Tiles) {
            int32 X = (TileIndex / MaxY) * TileSize;
            int32 Y = (TileIndex % MaxY) * TileSize;

            FVector TileLocation(X, Y, 0.0f);
            FTransform TileTransform(TileLocation);

            if(Start == TileIndex) 
            {
                Floor->AddInstance(TileTransform);
                FActorSpawnParameters SpawnParams;
                GetWorld()->SpawnActor<APlayerStart>(PlayerStart, TileTransform, SpawnParams);
                ASunrisePlayerCharacter* PlayerChar = Cast<ASunrisePlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
                PlayerChar->SetActorLocation(TileLocation);
            }
            else if (Type == 0)
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
}

void ASunriseTile::GenerateMazeBinaryTreeRoom()
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
    if(MaxX % 2 == 0) ++MaxX;
    if(MaxY % 2 == 0) ++MaxY;

    int32 Area = MaxX * MaxY;
    int32 Start = FMath::RandRange(1, MaxY-2);

    USTRUCT()
    struct RoomsArray {
        TArray<ERoomType> Rooms;

        ERoomType operator[](int32 Index) {
            return Rooms[Index];
        }

        void Replace(int32 Index, ERoomType Type) {
            Rooms[Index] = Type;
        }
    };

    TArray<RoomsArray> RoomType;
    RoomsArray RoomTypeCol;
    
    RoomTypeCol.Rooms.Init(ERoomType::None, MaxY);
    RoomType.Init(RoomTypeCol, MaxX);

    // if RowIndex 0 or MaxX-1 or MaxY-1 then skip as those are all walls
    for(size_t RowIndex = 1; RowIndex < MaxX; RowIndex+=2)
    {
        for(size_t ColIndex = 1; ColIndex < MaxY; ColIndex+=2)
        {
            // Start
            if (RowIndex-1 == 0 && ColIndex-1 == 0) {
                RoomType[RowIndex].Replace(ColIndex, ERoomType::Default);
                continue;
            }
            // Horizontal (West)
            if (RowIndex-1 == 0){
                RoomType[RowIndex].Replace(ColIndex, ERoomType::Default);
                RoomType[RowIndex].Replace(ColIndex-1, ERoomType::Default);
                continue;
            }
            // Vertical (South)
            if (ColIndex-1 == 0) {
                RoomType[RowIndex].Replace(ColIndex, ERoomType::Default);
                RoomType[RowIndex-1].Replace(ColIndex, ERoomType::Default);
                continue;
            }
            
            int32 DiceRoll = FMath::RandRange(0, 100);
            if (DiceRoll < 50)
            {
                // Horizontal (West)
                RoomType[RowIndex].Replace(ColIndex, ERoomType::Default);
                RoomType[RowIndex].Replace(ColIndex-1, ERoomType::Default);
            }
            else
            {
                // Vertical (South)
                RoomType[RowIndex].Replace(ColIndex, ERoomType::Default);
                RoomType[RowIndex-1].Replace(ColIndex, ERoomType::Default);
            }
        }
    }

    // End of Maze
    while(true)
    {
        int32 Random = FMath::RandRange(1,MaxY-2);
        if(ERoomType::Default == RoomType[MaxX-2][Random])
        {
            RoomType[MaxX-1].Replace(Random, ERoomType::Default);
            break;
        }
    }

    // Add walls
    RoomMaxX += 2;
    RoomMaxY += 2;

    int32 RoomSizeX = RoomMaxX * TileSize;
    int32 RoomSizeY = RoomMaxY * TileSize;

    // Spawn
    int32 RoomIndex = 0;
    for(const auto &Row : RoomType)
    {
        for(const auto &Type : Row.Rooms) {
            int32 XOffset = (RoomIndex / MaxY) * RoomSizeX;
            int32 YOffset = (RoomIndex % MaxY) * RoomSizeY;

            if(Start == RoomIndex) 
            {
                GenerateRoom(RoomMaxX, RoomMaxY, XOffset, YOffset, ERoomType::Start);
            }
            else if (Type == ERoomType::Default)
            {
                GenerateRoom(RoomMaxX, RoomMaxY, XOffset, YOffset, ERoomType::Default);
            }
            ++RoomIndex;    
        }
    }

}

void ASunriseTile::GenerateRoom(int32 LengthX, int32 LengthY, int32 OffsetX, int32 OffsetY, ERoomType Type)
{
    // Calculate half of each side X and Y to place doors
    int32 MidX = LengthX / 2;
    int32 MidY = LengthY / 2;

    // Go through X and Y in 2 loops to generate tiles
    int32 TileIndex = 0;
    for(size_t RowIndex = 0; RowIndex < LengthX; ++RowIndex)
    {
        for(size_t ColIndex = 0; ColIndex < LengthY; ++ColIndex)
        {
            int32 XLocation = (TileIndex / LengthY) * TileSize + OffsetX;
            int32 YLocation = (TileIndex % LengthY) * TileSize + OffsetY;
            
            FVector TileLocation(XLocation, YLocation, 0.0f);
            FTransform TileTransform(TileLocation);

            // TODO: Change logic for doors after maze generating algorithm done
            if ((ColIndex == 0 && RowIndex == MidX) || 
                (RowIndex == 0 && ColIndex == MidY) ||
                (ColIndex == LengthY-1 && RowIndex == MidX) || 
                (RowIndex == LengthX-1 && ColIndex == MidY))
            {
                Floor->AddInstance(TileTransform);
            }
            else if(RowIndex == 0 || RowIndex == LengthX-1)
            {
                Wall->AddInstance(TileTransform);
            }
            else if (ColIndex == 0 || ColIndex == LengthY-1)
            {
                Wall->AddInstance(TileTransform);
            }
            else if (Type == ERoomType::Start && RowIndex == MidX && ColIndex == MidY)
            {
                Floor->AddInstance(TileTransform);
                FActorSpawnParameters SpawnParams;
                GetWorld()->SpawnActor<APlayerStart>(PlayerStart, TileTransform, SpawnParams);
                ASunrisePlayerCharacter* PlayerChar = Cast<ASunrisePlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
                PlayerChar->SetActorLocation(TileLocation);
            }
            else
            {
                Floor->AddInstance(TileTransform);
            }
            ++TileIndex;
        }
    }


}