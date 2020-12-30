// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SunriseMapTile.h"
#include "SunriseRoom.generated.h"

UENUM()
enum class Door 
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

public:	
	// Sets default values for this actor's properties
	ASunriseRoom();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void GenerateRoom(ASunriseMapTile* Tile, FRandomStream Stream);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
