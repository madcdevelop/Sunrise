// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Classes/Components/BoxComponent.h"

#include "SunriseDoor.generated.h"

UCLASS()
class SUNRISE_API ASunriseDoor : public AActor
{
	GENERATED_BODY()
	
public:
    bool bIsOverlap;
    
    FRotator TargetRotation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(EditAnywhere, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, Category = "Components")
    class UBoxComponent* BoxTrigger;

public:	
	// Sets default values for this actor's properties
	ASunriseDoor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
