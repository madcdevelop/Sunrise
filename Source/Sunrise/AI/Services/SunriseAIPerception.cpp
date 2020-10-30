// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseAIPerception.h"
#include "../SunriseAIController.h"
#include "../Classes/Kismet/GameplayStatics.h"
#include "../Classes/Kismet/KismetSystemLibrary.h"


void USunriseAIPerception::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ASunriseAIController* AICon = Cast<ASunriseAIController>(OwnerComp.GetAIOwner());
    ACharacter* AIChar = AICon->GetCharacter();


    // TODO: where does GEditor come from in other files.
    auto World = GetWorld();
    check(World);

    TArray<AActor*> OutActors;

    UGameplayStatics::GetAllActorsOfClass(World, PlayerCharacter, OutActors);

    FVector MyLocation = AIChar->GetActorLocation();

    TArray<FHitResult> OutHits;

	// static bool SphereTraceMultiForObjects(UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits, bool bIgnoreSelf, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);
    UKismetSystemLibrary::SphereTraceMultiForObjects(World, MyLocation, FVector(0.0f, 0.0f, 15.0f), 1500.0f, DesiredObjectTypes, false, OutActors, EDrawDebugTrace::None, OutHits, true);

}