// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseGameMode.h"

void ASunriseGameMode::BeginPlay()
{
    Super::BeginPlay();

    ChangeMenuWidget(StartingWidgetClass);
}

void ASunriseGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if(CurrentWidget)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }

    if(NewWidgetClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if(CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}
