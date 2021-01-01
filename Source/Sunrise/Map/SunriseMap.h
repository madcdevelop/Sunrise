// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SunriseRoom.h"

#include "SunriseMap.generated.h"


UCLASS()
class SUNRISE_API ASunriseMap : public AActor
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Map")
    int32 MapSizeX;

    UPROPERTY(EditAnywhere, Category = "Map")
    int32 MapSizeY;

    UPROPERTY(EditAnywhere, Category = "Map")
    FRandomStream Stream;

    UPROPERTY(EditAnywhere, Category = "Map")
    int32 Seed;

    UPROPERTY(EditAnywhere, Category = "Map")
    bool bNewSeed;

    UPROPERTY(EditDefaultsOnly, Category = "Room")
    TSubclassOf<class ASunriseRoom> Room;

    TArray<FTile> MapTiles;

public:	
	// Sets default values for this actor's properties
	ASunriseMap();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    void GenerateMap();
};
