// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemObject.h"

#include "Inventory/ContainerCell.h"

TArray<TEnumAsByte<EItemActions>> UItemObject::GetItemActions() const
{
	return ItemStats.ItemActions;
}

bool UItemObject::IsContainer()
{
	const bool bIsContainer = ItemStats.ContainerRows.Num() > 0;

	return bIsContainer;
}

FVector2D UItemObject::GetDimensions()
{
	return bIsRotated ? FVector2D(Dimensions.Y, Dimensions.X) : FVector2D(Dimensions.X, Dimensions.Y);
}

UTexture2D* UItemObject::GetUpdatedIcon()
{
	return bIsRotated ? GetIconRotated() : GetDefaultIcon();
}

bool UItemObject::IsStackable()
{
	return ItemStats.MaxStackSize > 0;
}

void UItemObject::InitializeItem(FItemStats InItem, FName InRowName)
{
	ItemStats = InItem;
	ContainerRows = InItem.ContainerRows;
	Dimensions = InItem.ItemDimensions;
	MaxStackSize = InItem.MaxStackSize;
	RowName = InRowName;

	if(IsContainer())
	{
		InventoryContainer = NewObject<UInventoryContainer>();
		InventoryContainer->InitializeContainer(this);
	}

	GenerateIcons();
}

void UItemObject::SetDefaultIcon(UTexture2D* Texture)
{
	Icon = Texture;
}

void UItemObject::SetRotatedIcon(UTexture2D* RotatedIcon)
{
	IconRotated = RotatedIcon;
}

void UItemObject::SetStackSize(int32 Increase, int& OutRemainder)
{
	int32 NewStackSize = Increase + StackSize;

	if(NewStackSize > MaxStackSize)
	{
		OutRemainder = FMath::Abs(MaxStackSize - NewStackSize);
		StackSize = MaxStackSize;
	}
	else
	{
		OutRemainder = 0;
		StackSize = NewStackSize;
	}
	OnStackSizeChanged.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("new stack size: %d"), GetStackSize());
	UE_LOG(LogTemp, Warning, TEXT("out remainder: %d"), OutRemainder);

}

void UItemObject::SetStackSizeNoIncrement(int32 NewStackSize)
{
	StackSize = NewStackSize;
}

void UItemObject::SetItemBusy(bool bValue)
{
	bItemBusy = bValue;
	OnItemBusyChanged.Broadcast();
}

bool UItemObject::CanItemBeDroppedOn(UItemObject* InItem)
{
	for(auto CompatibleItem : GetStats().CompatibleItems)
	{
		if(InItem->GetRowName() == CompatibleItem.RowName)
		{
			return true;
		}
	}
	
	if(IsContainer())
	{
		if(GetInventoryContainer()->CanFitItem(InItem))
		{
			return true;
		}
	}

	return false;
}

bool UItemObject::DropItem(UItemObject* InItem)
{
	if(!CanAddToStack())
	{
		UE_LOG(LogTemp, Warning, TEXT("cant add to stack"));
		return false;
	}

	if(IsContainer())
	{
		if(GetInventoryContainer()->TryAddItemToCell(InItem))
		{
			UE_LOG(LogTemp, Warning, TEXT("added to cell"));
			return true;
		}
	}

	
	int32 OutRemainder = 0;
	SetStackSize(InItem->GetStackSize(), OutRemainder);

	if(OutRemainder > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("set stack size no increment"));
		InItem->SetStackSizeNoIncrement(OutRemainder);
	}
	else
	{
		InItem->SetStackSizeNoIncrement(0);

		if(InItem->GetOwnerCell())
		{
			InItem->GetOwnerCell()->RemoveItem(InItem);
			UE_LOG(LogTemp, Warning, TEXT("remove item"));
			return true;
		}
	}

	return false;
}

