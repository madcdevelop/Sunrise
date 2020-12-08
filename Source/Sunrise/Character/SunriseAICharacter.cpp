// Fill out your copyright notice in the Description page of Project Settings.


#include "SunriseAICharacter.h"

ASunriseAICharacter::ASunriseAICharacter()
{

}

void ASunriseAICharacter::Attack()
{
    Super::Attack();

    UE_LOG(LogTemp, Display, TEXT("The AI is attacking"));
}