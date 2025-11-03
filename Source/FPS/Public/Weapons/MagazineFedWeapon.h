// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "MagazineFedWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AMagazineFedWeapon : public AWeapon
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Attacking|WeaponRecoil")
	FVector LocationRecoil;
	UPROPERTY(EditAnywhere, Category="Attacking|WeaponRecoil")
	FRotator RotationRecoil;
	UPROPERTY(EditAnywhere, Category="Attacking|WeaponRecoil")
	float RecoilRecovery = 35.f;
	
	UPROPERTY(EditAnywhere, Category="Attacking|CameraRecoil")
	float CameraRecoilX;
	UPROPERTY(EditAnywhere, Category="Attacking|CameraRecoil")
	float CameraRecoilY;
	
	
	UPROPERTY(EditAnywhere, Category="Attacking|MuzzleFlash")
	UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Animations|Montages")
	TObjectPtr<UAnimMontage> CharacterReloadMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animations|Montages")
	TObjectPtr<UAnimMontage> WeaponReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animations|Montages")
	TObjectPtr<UAnimMontage> CharacterLoadMagMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animations|Montages")
	TObjectPtr<UAnimMontage> WeaponLoadMagMontage;

	

public:
	virtual void Attack() override;
	virtual void OnReloadPressed() override;

	virtual FVector GetAimingLocation() const override { return Super::GetAimingLocation() + RecoilLocationOffset / 2; }
	virtual FRotator GetAimingRotation() const override { return Super::GetAimingRotation() +
		FRotator(RecoilRotationOffset.X / 2, RecoilRotationOffset.Y / 2, RecoilRotationOffset.Z / 2); }

	virtual FVector GetStandingLocationOffset() const override { return Super::GetStandingLocationOffset() + RecoilLocationOffset; }
	virtual FRotator GetStandingRotationOffset() const override { return Super::GetStandingRotationOffset() +
			FRotator(RecoilRotationOffset.X, RecoilRotationOffset.Y, RecoilRotationOffset.Z); }

	virtual bool LoadMagazine(UItemObject* InMagazineItem) override;
	virtual void UnloadMagazine() override;

	virtual bool HasMagazine() const override;
	virtual UMagazineItem* GetMagazineItem() const override;

	virtual void SetCurrentMagazine(UMagazineItem* InItem) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleMagazine(bool bVisible);
protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void Reload();

	virtual void EndReload();
	virtual void ReloadMid();

private:
	FVector RecoilLocationOffset;
	FVector RecoilRotationOffset;

	FTimerHandle FullReloadTimerHandle;
	FTimerHandle ReloadMidTimerHandle;
	
	UPROPERTY()
	TObjectPtr<UMagazineItem> ReloadMagazine;
	
	UPROPERTY()
	TObjectPtr<UMagazineItem> Magazine;
};

