// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "FPS/FPSCharacter.h"
#include "Inventory/WeaponItem.h"


bool UInventoryComponent::TryAddItem(UItemObject* InItem)
{
	if(!HasItemOfType(InItem->GetItemType()) && InItem->IsContainer())
	{
		Items.Add(InItem);
		OnItemAdded.Broadcast(InItem);
		InItem->GetInventoryContainer()->SetInventoryComponent(this);
		return true;
	}

	if(InItem->GetItemType() == EItemTypes::Magazine || InItem->GetItemType() == EItemTypes::Ammo)
	{
		if(UInventoryContainer* VestContainer = GetInventoryContainer(EItemTypes::TacticalVest))
		{
			if(VestContainer->TryAddItemToCell(InItem))
			{
				return true;
			}
		}
	}
	
	for(auto Item : Items)
	{
		if(Item->IsContainer())
		{
			if(Item->GetInventoryContainer()->TryAddItemToCell(InItem))
			{
				if(InItem->IsContainer())
				{
					InItem->GetInventoryContainer()->SetInventoryComponent(this);
				}

				if(UWeaponItem* WeaponItem = Cast<UWeaponItem>(InItem))
				{
					if(WeaponItem->GetWeapon())
					{
						WeaponItem->GetWeapon()->InitializeWeapon(Cast<AFPSCharacter>(GetOwner()));
						WeaponItem->GetWeapon()->SetWeaponState(InInventory);
					}
				}

				return true;
			}
		}
	}

	return false;
}

UInventoryContainer* UInventoryComponent::GetInventoryContainer(EItemTypes ItemType)
{
	for(auto Item : Items)
	{
		if(Item->IsContainer() && Item->GetItemType() == ItemType)
		{
			return Item->GetInventoryContainer();
		}
	}

	return nullptr;
}

void UInventoryComponent::RemoveItem(UItemObject* InItem)
{
	if(Items.Contains(InItem))
	{
		Items.Remove(InItem);
	}
}

void UInventoryComponent::ToggleInventory()
{
	bIsInventoryOpen = !bIsInventoryOpen;
	if(bIsInventoryOpen)
	{
		InventoryWidget->AddToViewport();
	}
	else
	{
		InventoryWidget->RemoveFromParent();
	}

	OnInventoryToggled.Broadcast(bIsInventoryOpen);
}

float UInventoryComponent::GetTotalWeight()
{
	float Weight = 0.f;
	for(auto Item : Items)
	{
		if(Item)
		{
			Weight += Item->GetWeight();
		}

		if(Item->IsContainer())
		{
			for(auto ItemInContainer : Item->GetInventoryContainer()->GetItems())
			{
				if(Item->IsStackable())
				{
					Weight += ItemInContainer->GetStackSize() * ItemInContainer->GetWeight();
				}
				else
				{
					Weight += ItemInContainer->GetWeight();
				}
			}
		}
	}

	return Weight;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);
}

bool UInventoryComponent::HasItemOfType(EItemTypes ItemType)
{
	for(auto Item : Items)
	{
		if(Item && ItemType == Item->GetItemType())
		{
			return true;
		}
	}
	
	return false;
}
