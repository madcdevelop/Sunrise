// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Classes/Components/InstancedStaticMeshComponent.h"
#include "SunriseTile.generated.h"

class APlayerStart;

/*  
*   This class is the tile that is placed in the level.
*/
UCLASS()
class SUNRISE_API ASunriseTile : public AActor
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Map")
    int32 MaxX;

    UPROPERTY(EditAnywhere, Category = "Map")
    int32 MaxY;

    UPROPERTY(EditAnywhere, Category = "Map")
    int32 TileSize;

    UPROPERTY(EditAnywhere, Category = "Map")
    TSubclassOf<APlayerStart> PlayerStart;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Default")
    class USceneComponent* Root;

    UPROPERTY(EditDefaultsOnly, Category = "Geometry")
    class UInstancedStaticMeshComponent* Floor;
    
    UPROPERTY(EditDefaultsOnly, Category = "Geometry")
    class UInstancedStaticMeshComponent* Wall;

public:	
	// Sets default values for this actor's properties
	ASunriseTile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    void GenerateTiles();
};
