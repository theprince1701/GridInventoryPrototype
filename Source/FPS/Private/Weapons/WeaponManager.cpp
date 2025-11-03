// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponManager.h"

#include "Camera/CameraComponent.h"
#include "Inventory/WeaponItem.h"
#include "Math/UnrealMathUtility.h"

#include "Weapons/Weapon.h"

UWeaponManager::UWeaponManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UWeaponManager::InitializeWeapons(AFPSCharacter* InCharacter)
{
	Character = InCharacter;
	ArmStamina = MaxArmStamina;

	if(AWeapon* SpawnedDefaultWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass))
	{
		DefaultWeapon = SpawnedDefaultWeapon;
		CurrentWeapon = SpawnedDefaultWeapon;
	}

	DefaultFOV = Character->GetFirstPersonCameraComponent()->FieldOfView;
}

void UWeaponManager::ReloadPressed()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->OnReloadPressed();
	}
}


void UWeaponManager::OnWeaponEquipped(int32 Index)
{
	if(!Weapons.IsValidIndex(Index))
	{
		return;
	}

	if(AWeapon* Weapon = Weapons[Index])
	{
		Weapon->SetWeaponState(InHands);
		Weapon->AttachToComponent(Character->GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		Weapon->InitializeWeapon(Character);
		CurrentWeapon = Weapon;
		OnPrimaryWeaponChanged.Broadcast();
	}

	OnWeaponListChanged.Broadcast();
	
}

void UWeaponManager::AddItemToWeapons(UItemObject* InItem)
{
	AItemActor* ItemActor = InItem->GetItemActor();

	if(UWeaponItem* WeaponItem = Cast<UWeaponItem>(InItem))
	{
		if(AWeapon* WeaponActor = Cast<AWeapon>(ItemActor))
		{
			if(!Weapons.Contains(WeaponActor))
			{
				WeaponItem->SetWeapon(WeaponActor);
				WeaponActor->SetWeaponItem(WeaponItem);
				WeaponActor->SetWeaponState(OnBack);
				Weapons.Add(WeaponActor);
				OnWeaponListChanged.Broadcast();

				if(CurrentWeapon == DefaultWeapon)
				{
					OnWeaponEquipped(0);
				}

			}
		}
	}
}

void UWeaponManager::RemoveItemFromWeapons(UItemObject* InItem)
{
	AItemActor* ItemActor = InItem->GetItemActor();

	if(AWeapon* WeaponActor = Cast<AWeapon>(ItemActor))
	{
		if(Weapons.Contains(WeaponActor))
		{
			Weapons.Remove(WeaponActor);
			OnWeaponListChanged.Broadcast();
		}
	}

	if(Weapons.Num() <= 0)
	{
		CurrentWeapon = DefaultWeapon;
	}
}

void UWeaponManager::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Character->GetIsAiming())
	{
		ArmStamina -= DeltaTime * ArmStaminaDrainRate;
		ArmStamina = FMath::Clamp(ArmStamina,0,MaxArmStamina);
	}
	else
	{
		ArmStamina += DeltaTime * ArmStaminaDrainRate * 1.75f;
		ArmStamina = FMath::Clamp(ArmStamina,0,MaxArmStamina);
	}
	
	RawLocationOffset = FVector(FMath::Sin(TimeElapsed),0
		, SwayAmountA * FMath::Sin(SwayAmountB * TimeElapsed + PI)) / SwayScale * ((1- ArmStamina) * ArmStaminaBreathingMultiplier);
	
	LocationOffset = FMath::Lerp(LocationOffset, RawLocationOffset, DeltaTime * SwaySmoothing);
	TimeElapsed += DeltaTime;

	if(TimeElapsed > 6.3f)
	{
		TimeElapsed = 0.f;
	}

	const float FOV = Character->GetIsAiming() ? AimFOV : DefaultFOV;

	Character->GetFirstPersonCameraComponent()->FieldOfView =
		FMath::Lerp(Character->GetFirstPersonCameraComponent()->FieldOfView, FOV, FOVSpeed* DeltaTime);
}

bool UWeaponManager::CanAim() const
{
	if(CurrentWeapon)
	{
		return CurrentWeapon->CanAim();
	}

	return false;
}

bool UWeaponManager::CanReload()
{
	return true;
}

bool UWeaponManager::CanFire()
{
	return true;
}

void UWeaponManager::OnAttackPressed()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->OnAttackPressed();
	}
}

void UWeaponManager::OnAttackHeldDown()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->OnAttackHeldDown();
	}
}

void UWeaponManager::OnAttackReleased()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->OnAttackReleased();
	}
}

