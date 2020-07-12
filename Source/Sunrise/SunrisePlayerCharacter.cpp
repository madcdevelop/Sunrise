// Fill out your copyright notice in the Description page of Project Settings.


#include "SunrisePlayerCharacter.h"

#include "Engine/GameEngine.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ASunrisePlayerCharacter::ASunrisePlayerCharacter()
{
    // Character Defaults
    Health = 100;


    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 600.0f;
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
    SpringArmComponent->TargetArmLength = FMath::Lerp<float>(600.0f, 400.0f, ZoomFactor);
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

    UE_LOG(SunriseGameplayLog, Display, TEXT("The player is attacking."));
}

void ASunrisePlayerCharacter::Defend()
{
    Super::Defend();

    UE_LOG(SunriseGameplayLog, Display, TEXT("The player is defending."));
}

void ASunrisePlayerCharacter::UseItem()
{
    UE_LOG(SunriseGameplayLog, Display, TEXT("The player is using an item."));
}

void ASunrisePlayerCharacter::Interact()
{
    UE_LOG(SunriseGameplayLog, Display, TEXT("The player is interacting."));
}