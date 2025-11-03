// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "Weapons/WeaponManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AFPSCharacter::AFPSCharacter(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	WeaponManager = CreateDefaultSubobject<UWeaponManager>(TEXT("WeaponManager"));
	InventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("InventoryComponent"));
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerMovement = Cast<UPlayerMovementComponent>(GetCharacterMovement());

	WeaponManager->InitializeWeapons(this);
}

//////////////////////////////////////////////////////////////////////////// Input

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AFPSCharacter::ResetMove);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AFPSCharacter::SprintPressed);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AFPSCharacter::CrouchPressed);

		// Aiming
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AFPSCharacter::AimPressed);

		// Attacking
		EnhancedInputComponent->BindAction(AttackPressedAction, ETriggerEvent::Triggered, this, &AFPSCharacter::OnAttackPressed);
		EnhancedInputComponent->BindAction(AttackHeldDownAction, ETriggerEvent::Triggered, this, &AFPSCharacter::OnAttackHeldDown);
		EnhancedInputComponent->BindAction(AttackHeldDownAction, ETriggerEvent::Completed, this, &AFPSCharacter::OnAttackReleased);

		// Reloading
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AFPSCharacter::ReloadPressed);


	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

bool AFPSCharacter::GetIsSprinting() const
{
	return PlayerMovement->IsSprinting();
}

void AFPSCharacter::OnAttackPressed()
{
	if(WeaponManager)
	{
		WeaponManager->OnAttackPressed();
	}
}

void AFPSCharacter::OnAttackHeldDown()
{
	if(WeaponManager)
	{
		WeaponManager->OnAttackHeldDown();
	}
}

void AFPSCharacter::OnAttackReleased()
{
	if(WeaponManager)
	{
		WeaponManager->OnAttackReleased();
	}
}

void AFPSCharacter::ReloadPressed()
{
	if(WeaponManager)
	{
		WeaponManager->ReloadPressed();
	}
}


void AFPSCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		WalkHorizontal = MovementVector.X;
		WalkVertical = MovementVector.Y;
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFPSCharacter::ResetMove()
{
	WalkHorizontal = 0.f;
	WalkVertical = 0.f;
}

void AFPSCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		LookX = LookAxisVector.X;
		LookY = LookAxisVector.Y;
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFPSCharacter::SprintPressed()
{
	if(GetIsSprinting())
	{
		PlayerMovement->SprintReleased();
	}
	else
	{
		PlayerMovement->SprintPressed();
	}
}


void AFPSCharacter::CrouchPressed()
{
	PlayerMovement->CrouchPressed();
}

void AFPSCharacter::AimPressed()
{
	bIsAiming = !bIsAiming;
}
