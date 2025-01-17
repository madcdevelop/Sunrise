// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../Classes/Components/SceneCaptureComponent2D.h"

#include "../Items/SunriseItem.h"
#include "SunriseCharacter.h"
#include "SunrisePlayerCharacter.generated.h"


/**
 *  The character that the player controls.
 */
UCLASS()
class SUNRISE_API ASunrisePlayerCharacter : public ASunriseCharacter
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TMap<TEnumAsByte<EItems>, int32> ItemInventory;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    bool bActiveCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap")
    class USpringArmComponent* SpringArmComponentMiniMap;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap")
    class USceneCaptureComponent2D* CaptureComponent2DMiniMap;

    // Input variables
    float ZoomFactor;
    bool bZoomingIn;
    bool bZoomingOut;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    class TSubclassOf<class UUserWidget> InGameWidget;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    class TSubclassOf<class UUserWidget> PauseMenuWidget;

    bool bIsGamePaused;

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
    TSubclassOf<class UDamageType> WeaponDamageType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    float AttackEffectAnimationTime;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TSubclassOf<class AActor> AttackEffect;

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

    void PauseGame();

    virtual void OnDeath() override;
    
private:
    UFUNCTION()
    void OnBeginOverlap(AActor* MyOverlappedActor, AActor* OtherActor);

    UFUNCTION()
    void DestroyActor(AActor* Actor);

};
