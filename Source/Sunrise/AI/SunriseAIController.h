// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../Classes/BehaviorTree/BlackboardComponent.h"
#include "../Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "../Classes/Perception/AIPerceptionComponent.h"
#include "SunriseAIController.generated.h"

/**
 * Controls the AI
 */
UCLASS()
class SUNRISE_API ASunriseAIController : public AAIController
{
	GENERATED_BODY()

private:

    UPROPERTY(EditAnywhere, Category = "AI")
    class UBlackboardComponent* AIBlackboard;

    UPROPERTY(EditAnywhere, Category = "AI")
    class UBehaviorTreeComponent* AIBehaviorTreeComp;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    class UAIPerceptionComponent* AIPerception;


public:
    ASunriseAIController();

    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return AIBlackboard; }
    FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeComponent() const { return AIBehaviorTreeComp; }

protected:
    virtual void OnPossess(APawn* AIPawn) override;

};
