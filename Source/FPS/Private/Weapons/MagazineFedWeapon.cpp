// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MagazineFedWeapon.h"

#include "Components/ArrowComponent.h"
#include "Inventory/ContainerCell.h"
#include "Inventory/WeaponItem.h"
#include "Kismet/GameplayStatics.h"

void AMagazineFedWeapon::Attack()
{
	if(!HasMagazine())
	{
		return;
	}

	if(Magazine->GetStackSize() <= 0)
	{
		return;
	}
	
	
	Super::Attack();


	// Weapon Recoil
	RecoilLocationOffset += FVector(FMath::RandRange(-LocationRecoil.X, LocationRecoil.X),
	LocationRecoil.Y,
	FMath::RandRange(-LocationRecoil.Z, LocationRecoil.Z));
	
	RecoilRotationOffset += FVector(FMath::RandRange(-RotationRecoil.Pitch, RotationRecoil.Pitch),
		FMath::RandRange(-RotationRecoil.Yaw, RotationRecoil.Yaw),
		RotationRecoil.Roll)  * (Character->bIsCrouched ? 0.5f : 1.f);

	// Camera Recoil

	if(Character)
	{
		Character->AddControllerPitchInput(CameraRecoilY);

		const float RecoilX = FMath::RandRange(-CameraRecoilX, CameraRecoilX);
		Character->AddControllerYawInput(RecoilX);
	}

	// Muzzle Flash
	if(MuzzleFlashEffect)
	{
		const FVector MuzzleScale =
			FVector(FMath::RandRange(0.5f, 1.f), FMath::RandRange(0.5f, 1.f), FMath::RandRange(0.5f, 1.f));

		UGameplayStatics::SpawnEmitterAttached(	MuzzleFlashEffect, AttackArrowComponent, NAME_None, FVector::ZeroVector,
			FRotator::ZeroRotator, MuzzleScale);
	}

	int32 OutRemainder = 0;
	Magazine->SetStackSize(-1, OutRemainder);
	WeaponItem->OnStackSizeChanged.Broadcast();
}

void AMagazineFedWeapon::OnReloadPressed()
{
	Reload();
}

void AMagazineFedWeapon::Reload()
{
	UMagazineItem* FoundMagazine = nullptr;
	if(UInventoryContainer* VestContainer = Character->GetInventoryComponent()->GetInventoryContainer(EItemTypes::TacticalVest))
	{
		for(auto MagazineRow : GetWeaponItem()->GetStats().CompatibleItems)
		{
			if(UItemObject* MagazineItem = VestContainer->HasItemOfRowName(MagazineRow.RowName))
			{
				FoundMagazine = Cast<UMagazineItem>(MagazineItem);
				break;
			}
		}
	}

	if(FoundMagazine)
	{
		UAnimMontage* CharacterReload = HasMagazine() ? CharacterReloadMontage : CharacterLoadMagMontage;
		UAnimMontage* WeaponReload = HasMagazine() ? WeaponReloadMontage : WeaponLoadMagMontage;

		if(FoundMagazine->GetOwnerCell())
		{
			FoundMagazine->SetItemBusy(true);
		}

		ReloadMagazine = FoundMagazine;
		Character->GetMesh1P()->GetAnimInstance()->Montage_Play(CharacterReload);
		WeaponMesh->GetAnimInstance()->Montage_Play(WeaponReload);

		GetWorld()->GetTimerManager().SetTimer(FullReloadTimerHandle, this, &AMagazineFedWeapon::EndReload, CharacterReload->GetPlayLength());
		GetWorld()->GetTimerManager().SetTimer(ReloadMidTimerHandle, this, &AMagazineFedWeapon::ReloadMid, CharacterReload->GetPlayLength() / 2);

	}
}

void AMagazineFedWeapon::EndReload()
{
	if(ReloadMagazine)
	{
		ReloadMagazine->SetItemBusy(false);
		SetCurrentMagazine(ReloadMagazine);
		GetWeaponItem()->OnDimensionsChanged.Broadcast();
		WeaponItem->GenerateIcon();
		WeaponItem->OnStackSizeChanged.Broadcast();
		ReloadMagazine = nullptr;
	}
}

void AMagazineFedWeapon::ReloadMid()
{
	if(HasMagazine())
	{
		Character->GetInventoryComponent()->TryAddItem(Magazine);
		SetCurrentMagazine(nullptr);
		WeaponItem->OnDimensionsChanged.Broadcast();
		WeaponItem->GenerateIcon();
	}

	if(ReloadMagazine)
	{
		ReloadMagazine->GetOwnerCell()->RemoveItem(ReloadMagazine);
	}
}

bool AMagazineFedWeapon::LoadMagazine(UItemObject* InMagazineItem)
{
	if(InMagazineItem == nullptr)
	{
		if(UInventoryContainer* VestContainer = Character->GetInventoryComponent()->GetInventoryContainer(EItemTypes::TacticalVest))
		{
			for(auto MagazineRow : GetWeaponItem()->GetStats().CompatibleItems)
			{
				if(UItemObject* MagazineItem = VestContainer->HasItemOfRowName(MagazineRow.RowName))
				{
					InMagazineItem = Cast<UMagazineItem>(MagazineItem);
					break;
				}
			}
		}
	}

	if(!InMagazineItem)
	{
		return false;
	}

	if(GetWeaponItem()->GetOwnerCell())
	{
		bool bCanSupportIncrease = GetWeaponItem()->GetOwnerCell()->CanFitIncreasedDimensions(GetWeaponItem(),
			GetWeaponItem()->GetIsRotated() ? 1 : 0, GetWeaponItem()->GetIsRotated() ? 0 : 1);

		if(bCanSupportIncrease)
		{
			SetCurrentMagazine(Cast<UMagazineItem>(InMagazineItem));
			GetWeaponItem()->OnDimensionsChanged.Broadcast();
			return true;
		}
	}
	else
	{
		SetCurrentMagazine(Cast<UMagazineItem>(InMagazineItem));
		GetWeaponItem()->OnDimensionsChanged.Broadcast();
		return true;
	}

	return false;
}

void AMagazineFedWeapon::UnloadMagazine()
{
	if(Character)
	{
		if(HasMagazine())
		{
			Character->GetInventoryComponent()->TryAddItem(Magazine);
		}
	
	
		SetCurrentMagazine(nullptr);
		GetWeaponItem()->OnDimensionsChanged.Broadcast();
	}
}

void AMagazineFedWeapon::SetCurrentMagazine(UMagazineItem* InItem)
{
	Magazine = InItem;
	ToggleMagazine(HasMagazine());
	WeaponItem->GenerateIcon();
}

bool AMagazineFedWeapon::HasMagazine() const
{
	return Magazine->IsValidLowLevel();
}

UMagazineItem* AMagazineFedWeapon::GetMagazineItem() const
{
	return Magazine;
}

void AMagazineFedWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RecoilLocationOffset = FMath::Lerp(RecoilLocationOffset, FVector::ZeroVector, DeltaSeconds * RecoilRecovery);
	RecoilRotationOffset = FMath::Lerp(RecoilRotationOffset, FVector::ZeroVector, DeltaSeconds * RecoilRecovery);

}
