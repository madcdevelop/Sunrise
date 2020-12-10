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

/**
 *  The character that the player controls.
 */
UCLASS()
class SUNRISE_API ASunrisePlayerCharacter : public ASunriseCharacter
{
    GENERATED_BODY()

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* CameraComponent;
    
    /* The max length of the camera arm.*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    float MaxCamArmLength;
    
    /* The min length of the camera arm. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    float MinCamArmLength;

    // Input variables
    float ZoomFactor;
    bool bZoomingIn;
    bool bZoomingOut;

public:
    // Sets default values for this character's properties
    ASunrisePlayerCharacter();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Attack() override;
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Input functions
    void ZoomCamera(float AxisValue);
    void MoveForward(float AxisValue);
    void MoveRight(float AxisValue);

    // Actions
    void StartDefend() override;
    void EndDefend() override;

    void UseItem();
    void Interact();

private:
    UFUNCTION()
    void OnBeginOverlap(AActor* MyOverlappedActor, AActor* OtherActor);

};
