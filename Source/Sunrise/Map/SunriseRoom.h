// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Classes/Components/InstancedStaticMeshComponent.h"
#include "../Classes/Components/BoxComponent.h"

#include "SunriseRoom.generated.h"

USTRUCT()
struct FTile
{
    GENERATED_BODY()

    int32 RowIndex;
    int32 ColumnIndex;
    FTile* Parent;
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

UCLASS()
class SUNRISE_API ASunriseRoom : public AActor
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Map")
    int32 MinSize;

    UPROPERTY(EditAnywhere, Category = "Map")
    int32 MaxSize;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    FVector MeshSize;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UInstancedStaticMeshComponent* Floor;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UInstancedStaticMeshComponent* Wall;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UInstancedStaticMeshComponent* PillarCorner;

    UPROPERTY(EditAnywhere)
    class UBoxComponent* DoorOpeningSouth;

    UPROPERTY(EditAnywhere)
    class UBoxComponent* DoorOpeningEast;

    UPROPERTY(EditAnywhere)
    class UBoxComponent* DoorOpeningNorth;

    UPROPERTY(EditAnywhere)
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
