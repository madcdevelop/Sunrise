// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseDoor.h"

// Sets default values
ASunriseDoor::ASunriseDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    if(Root) RootComponent = Root;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    if(Mesh) Mesh->SetupAttachment(Root);

    // BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
    // if(BoxTrigger) BoxTrigger->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ASunriseDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASunriseDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

