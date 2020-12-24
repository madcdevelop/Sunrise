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

    // Tile Defaults
    MaxX = 5;
    MaxY = 5;
    TileSize = 1000;
}

// Called when the game starts or when spawned
void ASunriseTile::BeginPlay()
{
	Super::BeginPlay();

    GenerateMazeBinaryTree();
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
    *   Binary Tree
    *   Ref:
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
    * 
    *   Data Structure: None. 
    * 
    *   - Memory-less Maze generation algorithm.
    *   - No limit to size of maze you can create
    */

    // When either X or Y is even increase by 1 for extra wall on the end
    if(MaxX % 2 == 0) ++MaxX;
    if(MaxY % 2 == 0) ++MaxY;

    int32 Area = MaxX * MaxY;
    int32 Start = FMath::RandRange(1, MaxY-1);
    // TODO: Find smarter way to pick end point. Sometimes the end doesn't connect to the maze
    int32 End   = Area - FMath::RandRange(1, MaxY-1);

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

    // if RowIndex 0 or size - 1 then skip as those are all walls
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
            else if (RowIndex-1 == 0){
                TileType[RowIndex].Replace(ColIndex, 0);
                TileType[RowIndex].Replace(ColIndex-1, 0);
                continue;
            }
            // Vertical (South)
            else if (ColIndex-1 == 0) {
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
            else if (End == TileIndex)
            {
                Floor->AddInstance(TileTransform);
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