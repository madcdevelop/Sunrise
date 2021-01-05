// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"

#include "SunriseGameMode.generated.h"

/**
 *  Define and implement rules for game.
 */
UCLASS()
class SUNRISE_API ASunriseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Game Rules")
    int32 KeysToFind;

    

    /** The widget class we will use as our menu when the game starts. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> StartingWidgetClass;

    /** The widget instance that we are using as our menu. */
    UPROPERTY()
    UUserWidget* CurrentWidget;

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:
     /** Called when the game starts. */
    virtual void BeginPlay() override;


};
