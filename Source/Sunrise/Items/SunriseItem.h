// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SunriseItem.generated.h"

enum Items {
    SHIELD = 0,
    HOOKSHOT,
    BOOMERANG,
    MAGICWAND
};

UCLASS()
class SUNRISE_API ASunriseItem : public AActor
{
    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    ASunriseItem();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
    int32 ItemId;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
    FString Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(EditAnywhere, Category = "Components")
    UStaticMeshComponent* ItemMesh;

};
