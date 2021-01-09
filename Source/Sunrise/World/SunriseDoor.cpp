// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseDoor.h"
#include "../Character/SunrisePlayerCharacter.h"

// Sets default values
ASunriseDoor::ASunriseDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    if(Root) RootComponent = Root;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    if(Mesh) Mesh->SetupAttachment(Root);

    BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
    if(BoxTrigger) BoxTrigger->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ASunriseDoor::BeginPlay()
{
	Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &ASunriseDoor::OnOverlap);
}

// Called every frame
void ASunriseDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(bIsOverlap)
    {
        FRotator CurrentRotation = GetActorRotation();
        FRotator NewRotation = FRotator(FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->DeltaTimeSeconds, 2.0f));
        SetActorRotation(NewRotation);
    }
}

void ASunriseDoor::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    ASunrisePlayerCharacter* PlayerChar = Cast<ASunrisePlayerCharacter>(OtherActor);

    // @TODO: Find key and check value == 3 to trigger overlap
    if(PlayerChar)
    {
        bIsOverlap = true;
        TargetRotation = GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f);
        // @TODO: Remove 3 keys from inventory
    }
}
