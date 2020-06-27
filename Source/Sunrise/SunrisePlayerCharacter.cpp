// Fill out your copyright notice in the Description page of Project Settings.


#include "SunrisePlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ASunrisePlayerCharacter::ASunrisePlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);

}

// Called when the game starts or when spawned
void ASunrisePlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ASunrisePlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASunrisePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(InputComponent);

    InputComponent->BindAxis(TEXT("HorizontalPlayerMovement"), this, &ASunrisePlayerCharacter::HorizontalPlayerMovement);
    InputComponent->BindAxis(TEXT("VerticalPlayerMovement"), this, &ASunrisePlayerCharacter::VerticalPlayerMovement);

}

void ASunrisePlayerCharacter::HorizontalPlayerMovement(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is horizontal
        FRotator Rotation = Controller->GetControlRotation();

        // limit pitch when walking or falling
        if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
        {
            Rotation.Pitch = 0.0f;
        }

        // add movement in that direction
        const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ASunrisePlayerCharacter::VerticalPlayerMovement(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is horizontal
        FRotator Rotation = Controller->GetControlRotation();

        // limit pitch when walking or falling
        if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
        {
            Rotation.Pitch = 0.0f;
        }

        // add movement in that direction
        const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

