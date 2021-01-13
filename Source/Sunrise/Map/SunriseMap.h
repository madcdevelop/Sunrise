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
    bool bNewSeed;

    UPROPERTY(EditAnywhere, Category = "Map")
    FRandomStream Stream;

    UPROPERTY(EditAnywhere, Category = "Map")
    int32 Seed;

    UPROPERTY(EditDefaultsOnly, Category = "Map")
    int32 MaxMapSizeX;

    UPROPERTY(EditDefaultsOnly, Category = "Map")
    int32 MaxMapSizeY;

    UPROPERTY(EditAnywhere, Category = "Map")
    int32 MapSizeX;

    UPROPERTY(EditAnywhere, Category = "Map")
    int32 MapSizeY;

    UPROPERTY(EditAnywhere, Category = "Map")
    int32 Rooms;

    UPROPERTY(EditAnywhere, Category = "Map")
    int32 AISpawnCount;

    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TSubclassOf<class ASunriseRoom> Room;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<class APlayerStart> PlayerStart;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<class ASunriseAICharacter> MeleeAICharacter;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<class ASunriseDoor> Door;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<class AActor> GoldenKey;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<class AActor> EndLevelTrigger;

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
    void BinaryTreeMaze();
    void SpawnPlayerStart();
    void SpawnAICharacters(TSubclassOf<ACharacter> AICharacter);
    void DeleteActors(TSubclassOf<AActor> ActorToDelete);
    void SpawnActors(int32 ActorSpawnCount, TSubclassOf<AActor> ActorToSpawn);
};
