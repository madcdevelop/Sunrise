// Fill out your copyright notice in the Description page of Project Settings.


#include "SunrisePlayerCharacter.h"

#include "Engine/GameEngine.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ASunrisePlayerCharacter::ASunrisePlayerCharacter()
{
    // Character Defaults
    Health = 400.0f;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

    MaxCamArmLength = 800.0f;
    MinCamArmLength = 400.0f;
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = MaxCamArmLength;
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false;

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

    if (bZoomingIn)
        ZoomFactor += DeltaTime / 0.5f;
    if (bZoomingOut)
        ZoomFactor -= DeltaTime / 0.5f;

    ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
    CameraComponent->FieldOfView = FMath::Lerp<float>(90.0f, 70.0f, ZoomFactor);
    SpringArmComponent->TargetArmLength = FMath::Lerp<float>(MaxCamArmLength, MinCamArmLength, ZoomFactor);
}

// Called to bind functionality to input
void ASunrisePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);

    // Camera
    PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis(TEXT("CameraZoom"), this, &ASunrisePlayerCharacter::ZoomCamera);

    // Movement
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASunrisePlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASunrisePlayerCharacter::MoveRight);

    // Input
    if (!GetIsAttacking())
        PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &ASunrisePlayerCharacter::Attack);

    PlayerInputComponent->BindAction(TEXT("Defend"), IE_Pressed, this, &ASunrisePlayerCharacter::Defend);
    PlayerInputComponent->BindAction(TEXT("UseItem"), IE_Pressed, this, &ASunrisePlayerCharacter::UseItem);
    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ASunrisePlayerCharacter::Interact);
}


void ASunrisePlayerCharacter::ZoomCamera(float AxisValue)
{
    if ((Controller != NULL) && (AxisValue != 0.0f))
    {
        if (AxisValue < 0.0f)
        {
            bZoomingIn = true;
            bZoomingOut = false;
        }
        if (AxisValue > 0.0f)
        {
            bZoomingIn = false;
            bZoomingOut = true;
        }
    }
    else
    {
        bZoomingIn = false;
        bZoomingOut = false;
    }
}

void ASunrisePlayerCharacter::MoveForward(float AxisValue)
{
    FRotator Rotation = Controller->GetControlRotation();
    FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

    FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, AxisValue);
}

void ASunrisePlayerCharacter::MoveRight(float AxisValue)
{
    FRotator Rotation = Controller->GetControlRotation();
    FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

    FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(Direction, AxisValue);
}

void ASunrisePlayerCharacter::Attack()
{
    Super::Attack();

    UE_LOG(LogTemp, Display, TEXT("The player is attacking."));

}

void ASunrisePlayerCharacter::Defend()
{
    Super::Defend();

    UE_LOG(LogTemp, Display, TEXT("The player is defending."));
}

void ASunrisePlayerCharacter::UseItem()
{
    UE_LOG(LogTemp, Display, TEXT("The player is using an item."));
}

void ASunrisePlayerCharacter::Interact()
{
    UE_LOG(LogTemp, Display, TEXT("The player is interacting."));
}