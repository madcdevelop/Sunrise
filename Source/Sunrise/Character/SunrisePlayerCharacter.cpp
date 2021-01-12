// Fill out your copyright notice in the Description page of Project Settings.


#include "SunrisePlayerCharacter.h"
#include "SunriseAICharacter.h"
#include "../Controller/SunrisePlayerController.h"
#include "../Weapons/SunriseWeapon.h"
#include "../SunriseGameMode.h"

#include "../Classes/Kismet/GameplayStatics.h"
#include "../Classes/Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
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
    bActiveCamera = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

    MaxCamArmLength = 3000.0f;
    MinCamArmLength = 1600.0f;
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = MaxCamArmLength;
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false;

    SpringArmComponentMiniMap = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponentMiniMap"));
    SpringArmComponentMiniMap->SetupAttachment(RootComponent);

    CaptureComponent2DMiniMap = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComponent2DMiniMap"));
    CaptureComponent2DMiniMap->SetupAttachment(SpringArmComponentMiniMap, USpringArmComponent::SocketName);

    SetTickableWhenPaused(true);
}

// Called when the game starts or when spawned
void ASunrisePlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &ASunrisePlayerCharacter::OnBeginOverlap);

    
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
    if(bActiveCamera)
    {
        PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &APawn::AddControllerYawInput);
        PlayerInputComponent->BindAxis(TEXT("CameraZoom"), this, &ASunrisePlayerCharacter::ZoomCamera);
    }
    
    // Movement
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASunrisePlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASunrisePlayerCharacter::MoveRight);

    // Input
    if (!GetIsAttacking())
        PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &ASunrisePlayerCharacter::Attack);

    PlayerInputComponent->BindAction(TEXT("Defend"), IE_Pressed, this, &ASunrisePlayerCharacter::StartDefend);
    PlayerInputComponent->BindAction(TEXT("Defend"), IE_Released, this, &ASunrisePlayerCharacter::EndDefend);
    PlayerInputComponent->BindAction(TEXT("UseItem"), IE_Pressed, this, &ASunrisePlayerCharacter::UseItem);
    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ASunrisePlayerCharacter::Interact);

    // Start button
    PlayerInputComponent->BindAction(TEXT("Start"), IE_Pressed, this, &ASunrisePlayerCharacter::PauseGame).bExecuteWhenPaused = true;
    
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

    /** Radial Damage */
    TArray <FHitResult> OutHits;
    FVector StartLocation = GetActorLocation();
    FVector EndLocation = StartLocation + GetActorForwardVector() * 750.0f;

    FCollisionShape MyColShape = FCollisionShape::MakeBox(FVector(500.0f, 500.0f, 500.0f));
    bool bIsHit = GetWorld()->SweepMultiByChannel(OutHits, EndLocation, EndLocation, FQuat::Identity, ECC_WorldDynamic, MyColShape);
    // Uncomment to draw hit box of attack
    // DrawDebugBox(GetWorld(), EndLocation, MyColShape.GetExtent(), FColor::Red, false, 2.0f);
    GetWorld()->SpawnActor<AActor>(AttackEffect, FTransform(FVector(EndLocation.X, EndLocation.Y, 0.0f)));
    GetWorld()->GetTimerManager().SetTimer(AttackEffectTimerHandle, this, &ASunrisePlayerCharacter::DestroyEffect, AttackEffectAnimationTime, false);

    if(bIsHit)
    {
        TArray<AActor*> IgnoreActors;
        for(auto& Hit : OutHits)
        {
            ASunriseAICharacter* AIChar = Cast<ASunriseAICharacter>(Hit.GetActor());
            if(AIChar)
            {
                TArray<AActor*> AttachedActors;
                AIChar->GetAttachedActors(AttachedActors);
                for(auto & Actor : AttachedActors)
                {
                    ASunriseWeapon* Weapon = Cast<ASunriseWeapon>(Actor);
                    if(Weapon)
                    {
                        UGameplayStatics::ApplyRadialDamage(GetWorld(), Weapon->GetDamage(), Hit.ImpactPoint, 250.0f, WeaponDamageType, IgnoreActors, this, GetController());
                        IgnoreActors.Add(AIChar);         
                    }
                }
            }
        }
    }
}

void ASunrisePlayerCharacter::StartDefend()
{
    Super::StartDefend();

    UE_LOG(LogTemp, Display, TEXT("The player is defending."));
}

void ASunrisePlayerCharacter::EndDefend()
{
    Super::EndDefend();
}

void ASunrisePlayerCharacter::UseItem()
{
    UE_LOG(LogTemp, Display, TEXT("The player is using an item."));
}

void ASunrisePlayerCharacter::Interact()
{
    UE_LOG(LogTemp, Display, TEXT("The player is interacting."));
}

void ASunrisePlayerCharacter::PauseGame()
{

    ASunrisePlayerController* PlayerController = Cast<ASunrisePlayerController>(GetController());
    ASunriseGameMode* GameMode = Cast<ASunriseGameMode>(GetWorld()->GetAuthGameMode());
    if(PlayerController && GameMode)
    {
        if(!bIsGamePaused)
        {
            UE_LOG(LogTemp, Display, TEXT("Player has paused the game."));
            PlayerController->bShowMouseCursor = true;
            PlayerController->SetInputMode(FInputModeGameAndUI());
            GameMode->ChangeMenuWidget(PauseMenuWidget);

            bIsGamePaused = UGameplayStatics::SetGamePaused(GetWorld(), true);
        }
        else
        {
            UE_LOG(LogTemp, Display, TEXT("Player has resumed the game."));
            PlayerController->bShowMouseCursor = false;
            PlayerController->SetInputMode(FInputModeGameOnly());
            GameMode->ChangeMenuWidget(InGameWidget);
            bIsGamePaused = UGameplayStatics::SetGamePaused(GetWorld(), false);
        }
    }
}

void ASunrisePlayerCharacter::OnDeath()
{
    Super::OnDeath();
}

void ASunrisePlayerCharacter::OnBeginOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
    ASunriseWeapon* Weapon = Cast<ASunriseWeapon>(OtherActor);

    if(Weapon){

        ASunriseAICharacter* AIChar = Cast<ASunriseAICharacter>(Weapon->GetAttachParentActor());
        if(AIChar && AIChar->GetIsAttacking()){

            FDamageEvent WeaponDamageEvent;
            AController* AICon = AIChar->GetInstigatorController();

            float WeaponDamage = TakeDamage(Weapon->GetDamage(), WeaponDamageEvent, AICon, Weapon);
            SetHealth(GetHealth() - WeaponDamage);

            if(GetHealth() <= 0.0f){
                SetActorEnableCollision(false);
                StartAnimation(DeathAnimation, DeathAnimationTime);
                GetWorld()->GetTimerManager().SetTimer(CharacterTimerHandle, this, &ASunrisePlayerCharacter::OnDeath, DeathAnimationTime, false);
            }
            
        }
    }
}

void ASunrisePlayerCharacter::DestroyEffect()
{
    TArray<AActor*> OutActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AttackEffect, OutActors);
    for(auto Actor: OutActors)
    {
        Actor->Destroy();
    }   
}