// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SunriseMapTile.h"
#include "SunriseRoom.generated.h"

UCLASS()
class SUNRISE_API ASunriseRoom : public AActor
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Map")
    int32 RoomSizeX;

    UPROPERTY(EditAnywhere, Category = "Map")
    int32 RoomSizeY;



public:	
	// Sets default values for this actor's properties
	ASunriseRoom();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void GenerateRoom(ASunriseMapTile* Tile);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
