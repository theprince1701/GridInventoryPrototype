// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ContainerCell.h"

#include "Inventory/ItemObject.h"
#include "Inventory/WeaponItem.h"

void UContainerCell::InitializeCell(UInventoryContainer* InInventoryContainer, FVector2D InDimensions)
{
	int32 InventorySize = InDimensions.X * InDimensions.Y;
	Items.SetNum(InventorySize);

	InventoryContainer = InInventoryContainer;
	Dimensions = InDimensions;
}

FTile UContainerCell::IndexToTile(int32 InIndex)
{
	FTile Tile;

	Tile.X = InIndex % static_cast<int32>(Dimensions.X);
	Tile.Y = InIndex / static_cast<int32>(Dimensions.X);
	

	return Tile;
}

int32 UContainerCell::TileToIndex(FTile InTile)
{
	float X = static_cast<float>(InTile.X);
	float Y = static_cast<float>(InTile.Y);

	return (Dimensions.X * Y) + X;
}

FItemIndex UContainerCell::GetItemAtIndex(int32 InIndex)
{
	if(!Items.IsValidIndex(InIndex))
	{
		return FItemIndex();
	}

	return FItemIndex{true, Items[InIndex] };
}

bool UContainerCell::IsRoomAvaliable(UItemObject* InItem, int32 TopLeftIndex)
{
	FTile TopLeftTile = IndexToTile(TopLeftIndex);

	for(int32 x = TopLeftTile.X; x <= TopLeftTile.X + InItem->GetDimensions().X - 1; x++)
	{
		for(int32 y = TopLeftTile.Y; y <= TopLeftTile.Y + InItem->GetDimensions().Y - 1; y++)
		{
			FTile CurrentTile = { x, y};

			if(!IsTileValid(CurrentTile))
			{
				return false;
			}
			
			const int32 TileIndex = TileToIndex(CurrentTile);
			const FItemIndex ItemIndex = GetItemAtIndex(TileIndex);

			if(!ItemIndex.bValid)
			{
				return false;
			}

			if(ItemIndex.Item)
			{
				return false;
			}
		}
	}

	return true;
}

bool UContainerCell::IsTileValid(FTile Tile)
{
	bool bValid = false;
	
	if(Tile.X >= 0 && Tile.Y >= 0 && Tile.X < Dimensions.X && Tile.Y < Dimensions.Y)
	{
		bValid = true;
	}

	return bValid;
}

void UContainerCell::AddItemAt(UItemObject* InItem, int32 InTopLeftIndex)
{
	FTile TopLeftTile = IndexToTile(InTopLeftIndex);

	for(int32 x = TopLeftTile.X; x <= TopLeftTile.X + InItem->GetDimensions().X - 1; x++)
	{
		for(int32 y = TopLeftTile.Y; y <= TopLeftTile.Y + InItem->GetDimensions().Y - 1; y++)
		{
			FTile CurrentTile = { x, y };
			int32 Index = TileToIndex(CurrentTile);

			Items[Index] = InItem;
		}
	}

	if(UWeaponItem* WeaponItem = Cast<UWeaponItem>(InItem))
	{
		if(WeaponItem->GetWeapon())
		{
			WeaponItem->GetWeapon()->SetWeaponState(InInventory);
		}
	}

	InItem->SetContainerCell(this);
	OnInventoryChanged.Broadcast();
}

void UContainerCell::RemoveItem(UItemObject* InItem)
{
	if(!InItem)
	{
		return;
	}

	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(InItem == Items[i])
		{
			Items[i] = nullptr;
		}
	}

	OnInventoryChanged.Broadcast();
}

bool UContainerCell::TryAddItem(UItemObject* InItem)
{
	if(!InItem)
	{
		return false;
	}

	for(auto Item : Items)
	{
		if(Item && Item->IsStackable())
		{
			for(auto CompatibleItem : Item->GetStats().CompatibleItems)
			{
				if(CompatibleItem.RowName == InItem->GetRowName() && Item->CanAddToStack())
				{
					int32 OutRemainder;
					Item->SetStackSize(InItem->GetStackSize(), OutRemainder);

					if(OutRemainder > 0)
					{
						InItem->SetStackSizeNoIncrement(OutRemainder);
					}
					
					if(OutRemainder <= 0)
					{
						return true;
					}

					break;
				}
			}
		}
	}
	

	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(IsRoomAvaliable(InItem, i))
		{
			AddItemAt(InItem, i);
			bHasTriedToAddRotated = false;
			return true;
		}
	}

	//if failed to find a spot, try to re-add the item when its rotated, if that fails the item will rotate itself back

	return TryAddItemRotated(InItem);
}

bool UContainerCell::CanFitItem(UItemObject* InItem)
{
	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(IsRoomAvaliable(InItem, i))
		{
			return true;
		}
	}

	return false;
}

bool UContainerCell::TryAddItemRotated(UItemObject* InItem)
{
	InItem->RotateItem();
	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(IsRoomAvaliable(InItem, i))
		{
			UE_LOG(LogTemp, Warning, TEXT("has room at: %d"), i);
			AddItemAt(InItem, i);
			bHasTriedToAddRotated = false;
			return true;
		}
	}

	InItem->RotateItem();
	return false;
}

TMap<UItemObject*, FTile> UContainerCell::GetAllItems()
{
	TMap<UItemObject*, FTile> ItemMap;
	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(!Items[i])
		{
			continue;
		}

		if(ItemMap.Contains(Items[i]))
		{
			continue;
		}
		
		ItemMap.Add(Items[i], IndexToTile(i));
	}

	return ItemMap;
}

bool UContainerCell::CanFitIncreasedDimensions(UItemObject* InItem, int32 DimensionXIncrease, int32 DimensionYIncrease)
{
	FVector2D OriginalDimensions = InItem->GetDimensions();
	FVector2D NewDimensions = FVector2D(OriginalDimensions.X + DimensionXIncrease, OriginalDimensions.Y + DimensionYIncrease);

	for (int32 i = 0; i < Items.Num(); i++)
	{
		// Check if there’s room for the item with the new dimensions
		if (IsRoomAvailableForDimensions(i, NewDimensions))
		{
			return true;
		}
	}
    
	return false;
}

bool UContainerCell::IsRoomAvailableForDimensions(int32 TopLeftIndex, FVector2D NewDimensions)
{
	FTile TopLeftTile = IndexToTile(TopLeftIndex);

	for (int32 x = TopLeftTile.X; x < TopLeftTile.X + NewDimensions.X; x++)
	{
		for (int32 y = TopLeftTile.Y; y < TopLeftTile.Y + NewDimensions.Y; y++)
		{
			FTile CurrentTile = { x, y };

			if (!IsTileValid(CurrentTile))
			{
				return false;
			}

			const int32 TileIndex = TileToIndex(CurrentTile);
			const FItemIndex ItemIndex = GetItemAtIndex(TileIndex);

			if (!ItemIndex.bValid || ItemIndex.Item)
			{
				return false;
			}
		}
	}

	return true;
}

int32 UContainerCell::GetItemIndex(UItemObject* InItem)
{
	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(!Items[i])
		{
			continue;
		}

		if(InItem == Items[i])
		{
			return i;
		}
	}

	return -1;
}

