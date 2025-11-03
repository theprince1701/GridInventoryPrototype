// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/PlayerInventoryComponent.h"

bool FCharacterEquipmentSlot::HasItemInSlot() const
{
	return IsValid(Item);
}

bool UPlayerInventoryComponent::TryAddItem(UItemObject* InItem)
{
	if(!InItem)
	{
		return false;
	}
	
	const int32 EquipmentSlotID = GetEquipmentSlotIndex(InItem->GetItemType());

	if(CharacterEquipmentSlots.IsValidIndex(EquipmentSlotID))
	{
		if(!CharacterEquipmentSlots[EquipmentSlotID].HasItemInSlot())
		{
			CharacterEquipmentSlots[EquipmentSlotID].Item = InItem;
			OnEquipmentSlotUpdated.Broadcast(InItem->GetItemType());
			return true;
		}
	}
	
	return Super::TryAddItem(InItem);
}

void UPlayerInventoryComponent::OnItemRemoved(UItemObject* InItem)
{
	for(int32 i = 0; i < CharacterEquipmentSlots.Num(); i++)
	{
		if(CharacterEquipmentSlots[i].Item && CharacterEquipmentSlots[i].Item == InItem)
		{
			UE_LOG(LogTemp, Warning, TEXT("AHHHH"));
			CharacterEquipmentSlots[i].Item = nullptr;
		}
	}
}

int32 UPlayerInventoryComponent::GetEquipmentSlotIndex(EItemTypes InItemType)
{
	for(int32 i = 0; i < CharacterEquipmentSlots.Num(); i++)
	{
		if(InItemType == CharacterEquipmentSlots[i].ItemType)
		{
			return i;
		}
	}

	return -1;
}
