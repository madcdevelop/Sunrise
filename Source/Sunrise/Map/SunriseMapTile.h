// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Classes/Components/InstancedStaticMeshComponent.h"
#include "SunriseMapTile.generated.h"

UCLASS()
class SUNRISE_API ASunriseMapTile : public AActor
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    FVector MeshSize;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UInstancedStaticMeshComponent* Floor;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UInstancedStaticMeshComponent* Wall;

    UPROPERTY(EditDefaultsOnly, Category = "Tile")
    class UInstancedStaticMeshComponent* PillarCorner;

private:
    int32 ColumnIndex;
    
    int32 RowIndex;

    UPROPERTY(EditDefaultsOnly, Category = "Default")
    class USceneComponent* Root;

    ASunriseMapTile* ParentTile;

public:	
	// Sets default values for this actor's properties
	ASunriseMapTile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void GenerateTile(UInstancedStaticMeshComponent* TileMesh, FTransform Transform);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
