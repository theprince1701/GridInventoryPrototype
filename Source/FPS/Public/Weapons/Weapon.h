// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS/FPSCharacter.h"
#include "GameFramework/Actor.h"
#include "Inventory/ItemActor.h"
#include "Inventory/MagazineItem.h"
#include "Weapon.generated.h"

class UWeaponItem;

UENUM(BlueprintType)
enum EWeaponState
{
	Physics, // simulating physics on the floor
	Static, // static on the floor
	InHands, // in hands on character
	OnBack, // on back on character
	InInventory // in inventory
};

UENUM(BlueprintType)
enum EFireMode
{
	Single,
	FullAuto
};


UCLASS()
class FPS_API AWeapon : public AItemActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FVector IconGenerationLocation;
	UPROPERTY(EditAnywhere)
	FRotator IconGenerationRotation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> IdlePose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> WalkPose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> SprintPose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> CrouchPose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> AimPose;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	bool bCanAim;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations|Offsets")
	FVector StandingLocationOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations|Offsets")
	FRotator StandingRotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations|Offsets")
	FVector SprintLocationOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations|Offsets")
	FRotator SprintRotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations|Offsets")
	FVector CrouchLocationOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations|Offsets")
	FRotator CrouchRotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations|Offsets")
	FVector AimLocationOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations|Offsets")
	FRotator AimRotationOffset;

	
public:
	AWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Icons")
	float IconCaptureLocationMultiplier = 200;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Attacking")
	float AttackRate = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Attacking")
	float AttackBaseDamage = 35.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Attacking")
	TEnumAsByte<EFireMode> FireMode;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mesh")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Icons")
	TObjectPtr<USceneCaptureComponent2D> DefaultSceneCaptureComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Arrows")
	UArrowComponent* AttackArrowComponent;

public:
	UFUNCTION(BlueprintCallable)
	virtual bool HasMagazine() const { return false;}

	UFUNCTION(BlueprintCallable)
	virtual UMagazineItem* GetMagazineItem() const { return nullptr; }
	
	virtual void SetCurrentMagazine(UMagazineItem* InItem) { };

	UFUNCTION(BlueprintCallable)
	UWeaponItem* GetWeaponItem() const { return WeaponItem; }

	UFUNCTION(BlueprintCallable)
	AFPSCharacter* GetCharacter() const { return Character; }
	
	void SetWeaponItem(UWeaponItem* InItem) { WeaponItem = InItem; }
	
	void InitializeWeapon(AFPSCharacter* InCharacter);

	virtual void InitializeItemActor() override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponState(EWeaponState State);
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EWeaponState> GetWeaponState() const { return WeaponState; }
	
	UFUNCTION(BlueprintCallable)
	virtual FVector GetStandingLocationOffset() const { return StandingLocationOffset; }

	UFUNCTION(BlueprintCallable)
	virtual FRotator GetStandingRotationOffset() const { return StandingRotationOffset; }

	UFUNCTION(BlueprintCallable)
	virtual FVector GetAimingLocation() const { return AimLocationOffset;; }

	UFUNCTION(BlueprintCallable)
	virtual FRotator GetAimingRotation() const { return AimRotationOffset; }
	
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponStateChanged();

public:
	//for single fire
	virtual void OnAttackPressed();

	//for full auto
	virtual void OnAttackHeldDown();
	virtual void OnAttackReleased();
	
	virtual void OnReloadPressed();
	
	virtual bool CanAim() { return bCanAim; }
	virtual bool CanAttack() { return (GetWorld()->GetTimeSeconds() - LastAttackTime) >= (1.0f / AttackRate); }

	UFUNCTION(BlueprintCallable)
	virtual bool LoadMagazine(UItemObject* InMagazineItem);
	UFUNCTION(BlueprintCallable)
	virtual void UnloadMagazine();
	
protected:
	virtual void BeginPlay() override;

	virtual void Attack();

	UPROPERTY()
	TObjectPtr<AFPSCharacter> Character;
	
	UPROPERTY()
	TObjectPtr<UWeaponItem> WeaponItem;
	
	TEnumAsByte<EWeaponState> WeaponState;

	float LastAttackTime;
	bool bIsFiring;
};

