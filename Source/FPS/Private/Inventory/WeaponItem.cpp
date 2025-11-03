// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/WeaponItem.h"

#include "IconGeneration/IconGenerationSubSystem.h"
#include "Inventory/ContainerCell.h"

void UWeaponItem::SetWeapon(AWeapon* InWeapon)
{
	Weapon = InWeapon;
	GenerateIcon();
}

TArray<TEnumAsByte<EItemActions>> UWeaponItem::GetItemActions() const
{
	if(Weapon)
	{
		TArray<TEnumAsByte<EItemActions>> ItemActions = Super::GetItemActions();

		if(Weapon->HasMagazine())
		{
			ItemActions.Add(EItemActions::Unload);
		}
		else
		{
			ItemActions.Add(EItemActions::Load);
		}

		return ItemActions;
	}
	
	return Super::GetItemActions();
}

void UWeaponItem::GenerateIcon()
{
	UIconGenerationSubSystem* IconSubsystem = Weapon->GetGameInstance()->GetSubsystem<UIconGenerationSubSystem>();

	if(IconSubsystem)
	{
		if(GetWeapon()->GetWeaponState() == InInventory)
		{
			GetWeapon()->SetActorHiddenInGame(false);
		}
		
		UTextureRenderTarget2D* DefaultIconRenderTarget = IconSubsystem->GenerateIcon(this);

		if(DefaultIconRenderTarget)
		{
			UTexture2D* NewTexture = DefaultIconRenderTarget->ConstructTexture2D(
				Weapon,
				FString(TEXT("ConvertedTexture")),
				RF_NoFlags);
			
			
			if(NewTexture)
			{
				DefaultIcon = NewTexture;
				UE_LOG(LogTemp, Display, TEXT("default icon created"));

			}
		}
	}

	if(GetOwnerCell())
	{
		int32 ItemIndex = GetOwnerCell()->GetItemIndex(this);

		if(ItemIndex != -1)
		{
			GetOwnerCell()->RemoveItem(this);
			GetOwnerCell()->AddItemAt(this, ItemIndex);
		}
	}

	if(GetWeapon()->GetWeaponState() == InInventory)
	{
		GetWeapon()->SetActorHiddenInGame(true);
	}
}

FVector2d UWeaponItem::GetDimensions()
{
	if(GetWeapon())
	{
		if(GetWeapon()->HasMagazine())
		{
			return Super::GetDimensions();
		}
	}
	else
	{
		return Super::GetDimensions();
	}

	FVector2D NewDimensions = GetStats().ItemDimensions;

	if(GetIsRotated())
	{
		return FVector2D(NewDimensions.Y-1, NewDimensions.X);
	}
	
	return FVector2D(NewDimensions.X, NewDimensions.Y-1);

}

UTexture2D* UWeaponItem::GetDefaultIcon() const
{
	if(DefaultIcon)
	{
		return DefaultIcon;
	}
	
	return Super::GetDefaultIcon();
}

UTexture2D* UWeaponItem::GetUpdatedIcon()
{
	return Super::GetUpdatedIcon();
}

UTexture2D* UWeaponItem::GetIconRotated() const
{
	if(DefaultIcon)
	{
		return DefaultIcon;
	}
	
	return Super::GetIconRotated();
}

int32 UWeaponItem::GetStackSize() const
{
	if(Weapon && Weapon->HasMagazine())
	{
		return Weapon->GetMagazineItem()->GetStackSize();
	}

	return 0;
}

int32 UWeaponItem::GetMaxStackSize() const
{
	if(Weapon && Weapon->HasMagazine())
	{
		return Weapon->GetMagazineItem()->GetMaxStackSize();
	}

	return 0;
}

bool UWeaponItem::IsStackable()
{
	if(Weapon && Weapon->HasMagazine())
	{
		return true;
	}
	
	return Super::IsStackable();
}

FVector2D UWeaponItem::GetDimensionsNonSwapped()
{
	FVector2D NewDimensions = GetDimensions();

	if(GetIsRotated())
	{
		NewDimensions = FVector2D(NewDimensions.Y, NewDimensions.X);
	}

	return NewDimensions;
}

bool UWeaponItem::CanItemBeDroppedOn(UItemObject* InItem)
{
	if(InItem->GetItemType() == EItemTypes::Magazine)
	{
		return !Weapon->HasMagazine() && Super::CanItemBeDroppedOn(InItem);
	}

	return Super::CanItemBeDroppedOn(InItem);
}

bool UWeaponItem::DropItem(UItemObject* InItem)
{
	if(InItem->GetItemType() == EItemTypes::Magazine)
	{
		if(UMagazineItem* MagazineItem = Cast<UMagazineItem>(InItem))
		{
			return Weapon->LoadMagazine(MagazineItem);
		}
	}

	return false;
}

bool UWeaponItem::DropItemActorInsteadOfDefaultClass() const
{
	return true;
}
