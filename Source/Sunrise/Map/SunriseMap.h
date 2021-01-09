// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Classes/GameFramework/PlayerStart.h"

#include "SunriseRoom.h"
#include "../Character/SunriseAICharacter.h"
#include "../World/SunriseDoor.h"

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

    UPROPERTY(EditAnywhere, Category = "Map")
    int32 Rooms;

    UPROPERTY(EditDefaultsOnly, Category = "Room")
    TSubclassOf<class ASunriseRoom> Room;

    UPROPERTY(EditAnywhere, Category = "Player Character")
    TSubclassOf<class APlayerStart> PlayerStart;

    UPROPERTY(EditAnywhere, Category = "AI Character")
    int32 AISpawnCount;

    UPROPERTY(EditAnywhere, Category = "AI Character")
    TSubclassOf<class ASunriseAICharacter> MeleeAICharacter;

    UPROPERTY(EditAnywhere, Category = "Door")
    TSubclassOf<class ASunriseDoor> Door;

    TArray<FTile> MapTiles;

    UPROPERTY(EditAnywhere, Category = "Goal")
    TSubclassOf<class AActor> GoldenKey;

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
    void BinaryTreeMaze();
    void SpawnPlayerStart();
    void SpawnAICharacters(TSubclassOf<ACharacter> AICharacter);
    void DeleteActors(TSubclassOf<AActor> ActorToDelete);
    void SpawnActors(int32 ActorSpawnCount, TSubclassOf<AActor> ActorToSpawn);
};
