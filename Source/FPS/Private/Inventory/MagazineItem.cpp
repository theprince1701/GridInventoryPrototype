// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/MagazineItem.h"

#include "Inventory/ContainerCell.h"

int32 UMagazineItem::GetStackSize() const
{
	return Super::GetStackSize();
}

TArray<TEnumAsByte<EItemActions>> UMagazineItem::GetItemActions() const
{
	TArray<TEnumAsByte<EItemActions>> ItemActions = Super::GetItemActions();

	if(GetStackSize() > 0)
	{
		ItemActions.Add(Unload);
	}
	
	return ItemActions;
}

void UMagazineItem::UnloadMagazine()
{
	UInventoryComponent* InventoryComponent = GetOwnerCell()->GetInventoryContainer()->GetInventoryComponent();

	if(InventoryComponent)
	{
		
	}
}
