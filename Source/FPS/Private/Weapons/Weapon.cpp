// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "Components/ArrowComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Inventory/ContainerCell.h"
#include "Inventory/WeaponItem.h"

AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());
	
	DefaultSceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("DefaultSceneCapture"));
	DefaultSceneCaptureComponent->SetupAttachment(WeaponMesh);

	AttackArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	AttackArrowComponent->SetupAttachment(WeaponMesh);
	PrimaryActorTick.bCanEverTick = true;

}


void AWeapon::InitializeItemActor()
{
	WeaponItem = Cast<UWeaponItem>(GetItemObject());

	if(WeaponItem)
	{
		WeaponItem->SetWeapon(this);
		SetWeaponItem(WeaponItem);
	}
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	OnWeaponStateChanged();
}


void AWeapon::OnAttackPressed()
{
	if(FireMode == Single)
	{
		Attack();
	}
}

void AWeapon::OnAttackHeldDown()
{
	bIsFiring = true;
	if(FireMode == FullAuto && CanAttack())
	{
		Attack();
	}
}

void AWeapon::OnAttackReleased()
{
	bIsFiring = false;
}

void AWeapon::OnReloadPressed()
{
}


/*/
bool AWeapon::LoadMagazine(UMagazineItem* InItem)
{
	if(InItem == nullptr)
	{
		if(UInventoryContainer* VestContainer = Character->GetInventoryComponent()->GetInventoryContainer(EItemTypes::TacticalVest))
		{
			for(auto MagazineRow : Item->GetStats().CompatibleItems)
			{
				if(UItemObject* MagazineItem = VestContainer->HasItemOfRowName(MagazineRow.RowName))
				{
					InItem = Cast<UMagazineItem>(MagazineItem);
					break;
				}
			}
		}
	}

	if(!InItem)
	{
		return false;
	}

	bool bCanSupportIncrease = Item->GetOwnerCell()->CanFitIncreasedDimensions(Item,
		Item->GetIsRotated() ? 1 : 0, Item->GetIsRotated() ? 0 : 1);

	if(bCanSupportIncrease)
	{
		SetCurrentMagazine(InItem);
		OnMagazineLoaded();
		Item->OnDimensionsChanged.Broadcast();
		return true;
	}

	return false;
}

void AWeapon::UnloadMagazine()
{
	if(Character)
	{
		if(HasMagazine())
		{
			Character->GetInventoryComponent()->TryAddItem(Magazine);
		}
	
	
		SetCurrentMagazine(nullptr);
		Item->OnDimensionsChanged.Broadcast();
	}
}
/*/

void AWeapon::InitializeWeapon(AFPSCharacter* InCharacter)
{
	Character = InCharacter;

	if(GetWeaponItem())
	{
		GetWeaponItem()->GenerateIcon();
	}
}

bool AWeapon::LoadMagazine(UItemObject* InMagazineItem)
{
	return false;
}

void AWeapon::UnloadMagazine()
{
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::Attack()
{
	LastAttackTime = GetWorld()->GetTimeSeconds();
}
