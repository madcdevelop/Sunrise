// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Classes/Components/InstancedStaticMeshComponent.h"
#include "../Classes/Components/BoxComponent.h"

#include "SunriseRoom.generated.h"

UENUM()
enum class ETile
{
    None = 0,
    Floor,
    FloorHallway,
    Door,
    WallSouth,
    WallEast,
    WallNorth,
    WallWest,
    CornerWallSouthWest,
    CornerWallSouthEast,
    CornerWallNorthWest,
    CornerWallNorthEast
};

UENUM()
enum class EDoor 
{
    None = 0,
    South,
    East,
    North,
    West
};

USTRUCT()
struct FTile
{
    GENERATED_BODY()

    FTile()
        : Type(ETile::None), RowIndexMap(0), ColumnIndexMap(0), RowIndexRoom(0), ColumnIndexRoom(0), 
          Location(FVector(0.0f, 0.0f, 0.0f)), Parent(nullptr)
    {}

    FTile(ETile InType, int32 InRowIndexMap, int32 InColumnIndexMap, int32 InRowIndexRoom, int32 InColumnIndexRoom, FVector InLocation, FTile* InParent)
        : Type(InType), RowIndexMap(InRowIndexMap), ColumnIndexMap(InColumnIndexMap), RowIndexRoom(InRowIndexRoom), ColumnIndexRoom(InColumnIndexRoom), 
          Location(InLocation), Parent(InParent)
    {}

    UPROPERTY(VisibleAnywhere)
    ETile Type;

    UPROPERTY(VisibleAnywhere)
    int32 RowIndexMap;

    UPROPERTY(VisibleAnywhere)
    int32 ColumnIndexMap;

    UPROPERTY(VisibleAnywhere)
    int32 RowIndexRoom;

    UPROPERTY(VisibleAnywhere)
    int32 ColumnIndexRoom;

    UPROPERTY(VisibleAnywhere)
    FVector Location;

    FTile* Parent;
};

UCLASS()
class SUNRISE_API ASunriseRoom : public AActor
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Room")
    int32 MinSize;

    UPROPERTY(EditAnywhere, Category = "Room")
    int32 MaxSize;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    FVector MeshSize;

    UPROPERTY(VisibleAnywhere, Category = "Tile")
    TArray<FTile> Tiles;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UInstancedStaticMeshComponent* Floor;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UInstancedStaticMeshComponent* Ceiling;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UInstancedStaticMeshComponent* Wall;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UInstancedStaticMeshComponent* PillarCorner;

    UPROPERTY(EditAnywhere, Category = "Room")
    class UBoxComponent* DoorOpeningSouth;

    UPROPERTY(EditAnywhere, Category = "Room")
    class UBoxComponent* DoorOpeningEast;

    UPROPERTY(EditAnywhere, Category = "Room")
    class UBoxComponent* DoorOpeningNorth;

    UPROPERTY(EditAnywhere, Category = "Room")
    class UBoxComponent* DoorOpeningWest;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Default")
    class USceneComponent* Root;

public:	
	// Sets default values for this actor's properties
	ASunriseRoom();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void GenerateRoom(TArray<FTile>& MapTiles, int32 StartMapTileIndex, int32 MapSizeY, FRandomStream Stream, FVector Offset);

    void GenerateHallwayHorizontal(FRandomStream Stream, FVector Offset);

    void GenerateHallwayVertical(FRandomStream Stream, FVector Offset);

    void AddRooms(FRandomStream Stream, int32 MapSizeX, int32 MapSizeY, int32 Rooms, TArray<FTile>& MapTiles);

    void GenerateTile(UInstancedStaticMeshComponent* TileMesh, FTransform Transform);
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    UBoxComponent* CreateTraceBox(AActor* Parent, FName Name, FRotator Rotation, FVector Location, FVector Extents);
};
