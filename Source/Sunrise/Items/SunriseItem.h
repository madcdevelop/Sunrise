// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SunriseItem.generated.h"

UENUM(BlueprintType)
enum EItems {
    Shield = 0,
    Hookshot,
    Boomerang,
    MagicWand,
    GoldenKey
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

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Properties")
    TEnumAsByte<EItems> ItemsEnum;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(EditAnywhere, Category = "Components")
    UStaticMeshComponent* ItemMesh;

};
