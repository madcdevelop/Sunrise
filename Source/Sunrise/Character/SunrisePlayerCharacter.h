// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "SunriseCharacter.h"
#include "SunrisePlayerCharacter.generated.h"


UCLASS()
class SUNRISE_API ASunrisePlayerCharacter : public ASunriseCharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASunrisePlayerCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* CameraComponent;
    
    // Input variables
    float ZoomFactor;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    float MaxCamArmLength;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    float MinCamArmLength;
    bool bZoomingIn;
    bool bZoomingOut;

    // Input functions
    void ZoomCamera(float AxisValue);
    void MoveForward(float AxisValue);
    void MoveRight(float AxisValue);

    // Actions
    void Attack() override;
    void Defend() override;
    void UseItem();
    void Interact();

};
