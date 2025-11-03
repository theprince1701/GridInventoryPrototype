// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPS/FPSCharacter.h"
#include "WeaponManager.generated.h"


class AWeapon;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponListChanged);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPrimaryWeaponChanged);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPS_API UWeaponManager : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category=Weapons)
	TSubclassOf<AWeapon> DefaultWeaponClass;
	UPROPERTY(EditDefaultsOnly, Category=Weapons)
	FName SocketName = "ik_hand_gun";


	UPROPERTY(EditAnywhere, Category=Breathing)
	float SwayAmountA = 1;
	UPROPERTY(EditAnywhere, Category=Breathing)
	float SwayAmountB = 2;
	UPROPERTY(EditAnywhere, Category=Breathing)
	float SwaySmoothing = 5.f;
	UPROPERTY(EditAnywhere, Category=Breathing)
	float SwayScale = 400.f;

	UPROPERTY(EditAnywhere, Category=ArmStamina)
	float MaxArmStamina = 1.f;
	UPROPERTY(EditAnywhere, Category=ArmStamina)
	float ArmStaminaDrainRate = .5f;
	UPROPERTY(EditAnywhere, Category=ArmStamina)
	float ArmStaminaBreathingMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, Category = Aiming)
	float AimFOV = 75;
	UPROPERTY(EditAnywhere, Category = Aiming)
	float FOVSpeed = 5.f;

public:
	
	UWeaponManager();
	virtual void InitializeWeapons(AFPSCharacter* InCharacter);

	
	//input
	virtual void ReloadPressed();

	UFUNCTION(BlueprintCallable)
	AFPSCharacter* GetCharacter() const { return Character; }
	UFUNCTION(BlueprintCallable)
	AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
	UFUNCTION(BlueprintCallable)
	TArray<AWeapon*> GetAllWeapons() const { return Weapons; }
	UFUNCTION(BlueprintCallable)
	FVector GetBreatheLocationOffset() const { return LocationOffset; }
	UFUNCTION(BlueprintCallable)
	float GetArmStamina() const { return ArmStamina; }

	UPROPERTY(BlueprintAssignable)
	FOnWeaponListChanged OnWeaponListChanged;
	UPROPERTY(BlueprintAssignable)
	FOnPrimaryWeaponChanged OnPrimaryWeaponChanged;
	UFUNCTION(BlueprintCallable)
	void OnWeaponEquipped(int32 Index);

	UFUNCTION(BlueprintCallable)
	void AddItemToWeapons(UItemObject* InItem);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromWeapons(UItemObject* InItem);
	
	
	
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	bool CanAim() const;
	bool CanReload();
	bool CanFire();

	void OnAttackPressed();

	void OnAttackHeldDown();
	void OnAttackReleased();
	
private:
	UPROPERTY()
	TObjectPtr<AFPSCharacter> Character;

	UPROPERTY()
	TArray<TObjectPtr<AWeapon>> Weapons;

	UPROPERTY()
	TObjectPtr<AWeapon> CurrentWeapon;

	UPROPERTY()
	TObjectPtr<AWeapon> DefaultWeapon;
	
	FVector RawLocationOffset;
	FVector LocationOffset;

	float TimeElapsed;
	float ArmStamina;
	float DefaultFOV;
};
