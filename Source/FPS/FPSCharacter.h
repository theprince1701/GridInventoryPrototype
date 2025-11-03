// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/PlayerInventoryComponent.h"
#include "Logging/LogMacros.h"
#include "FPSCharacter.generated.h"

class UWeaponManager;
class UPlayerMovementComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

	//components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerMovementComponent> PlayerMovement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Weapons, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UWeaponManager> WeaponManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Inventory, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerInventoryComponent> InventoryComponent;

	//input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CrouchAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AimAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ReloadAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackPressedAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackHeldDownAction;

	UPROPERTY(EditAnywhere, Category = Aiming)
	float AimSensitivityMultiplier = 0.5f;

public:
	AFPSCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	void Move(const FInputActionValue& Value);
	void ResetMove();
	void Look(const FInputActionValue& Value);

	void SprintPressed();
	void CrouchPressed();

	void OnAttackPressed();
	void OnAttackHeldDown();
	void OnAttackReleased();
	
	void ReloadPressed();

	void AimPressed();
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	UFUNCTION(BlueprintCallable)
	UPlayerInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	UFUNCTION(BlueprintCallable)
	bool GetIsSprinting() const;
	
	UFUNCTION(BlueprintCallable)
	float GetLookX() const { return LookX; }
	UFUNCTION(BlueprintCallable)
	float GetLookY() const { return LookY; }

	UFUNCTION(BlueprintCallable)
	float GetWalkVertical() const { return WalkVertical; }

	UFUNCTION(BlueprintCallable)
	float GetWalkHorizontal() const { return WalkHorizontal; }

	UFUNCTION(BlueprintCallable)
	bool GetIsAiming() const { return bIsAiming;}

private:
	float LookX;
	float LookY;

	float WalkVertical;
	float WalkHorizontal;

	bool bIsAiming;
};

