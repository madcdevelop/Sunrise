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
        : Type(ETile::None), RowIndex(0), ColumnIndex(0), Location(FVector(0.0f, 0.0f, 0.0f)), Parent(nullptr)
    {}

    FTile(ETile InType, int32 InRowIndex, int32 InColumnIndex, FVector InLocation, FTile* InParent)
        : Type(InType), RowIndex(InRowIndex), ColumnIndex(InColumnIndex), Location(InLocation), Parent(InParent)
    {}

    UPROPERTY(VisibleAnywhere)
    ETile Type;

    UPROPERTY(VisibleAnywhere)
    int32 RowIndex;

    UPROPERTY(VisibleAnywhere)
    int32 ColumnIndex;

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

    UPROPERTY(EditAnywhere, Category = "Tile")
    TArray<FTile> Tiles;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UInstancedStaticMeshComponent* Floor;

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

    void GenerateRoom(FRandomStream Stream, FVector Offset);

    void GenerateHallwayHorizontal(FRandomStream Stream, FVector Offset);

    void GenerateHallwayVertical(FRandomStream Stream, FVector Offset);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    void GenerateTile(UInstancedStaticMeshComponent* TileMesh, FTransform Transform);

    UBoxComponent* CreateTraceBox(AActor* Parent, FName Name, FRotator Rotation, FVector Location, FVector Extents);
};
