// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseItem.h"

// Sets default values
ASunriseItem::ASunriseItem()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    if (Root)
    {
        RootComponent = Root;
    }

    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    if (ItemMesh)
    {
        ItemMesh->SetupAttachment(Root);
    }

}

// Called when the game starts or when spawned
void ASunriseItem::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void ASunriseItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

