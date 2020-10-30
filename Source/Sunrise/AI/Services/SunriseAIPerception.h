// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "../../Character/SunrisePlayerCharacter.h"
#include "SunriseAIPerception.generated.h"

/**
 * Service that runs AI perception if the AI sees the player.
 */
UCLASS()
class SUNRISE_API USunriseAIPerception : public UBTService
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    TSubclassOf<ASunrisePlayerCharacter> PlayerCharacter;

    UPROPERTY(EditAnywhere, Category = "Player")
    TArray<TEnumAsByte<EObjectTypeQuery>> DesiredObjectTypes;

protected:

    /** update next tick interval
	 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
