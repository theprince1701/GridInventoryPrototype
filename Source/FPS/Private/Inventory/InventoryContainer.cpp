// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryContainer.h"

#include "Inventory/ContainerCell.h"
#include "Inventory/ItemObject.h"

void UInventoryContainer::InitializeContainer(UItemObject* InItem)
{
	UE_LOG(LogTemp, Warning, TEXT("initialzing container2"))

	for(auto Cell : InItem->GetContainerRows())
	{
		TArray<FVector2D> Dimensions = Cell.RowDimensions;
		TArray<UContainerCell*> RowCells;
		
		for(auto RowDimension : Dimensions)
		{
			UContainerCell* NewCell = NewObject<UContainerCell>();
			NewCell->InitializeCell(this, RowDimension);
			RowCells.Add(NewCell);
		}

		FContainerCells ContainerCells;
		ContainerCells.Cells = RowCells;
		Cells.Add(ContainerCells);
	}
}

bool UInventoryContainer::TryAddItemToCell(UItemObject* InItem)
{
	for(auto CellRow : Cells)
	{
		for(auto Cell : CellRow.Cells)
		{
			if(Cell->TryAddItem(InItem))
			{
				return true;
			}
		}
	}

	return false;
}

bool UInventoryContainer::CanFitItem(UItemObject* InItem)
{
	for(auto CellRow : Cells)
	{
		for(auto Cell : CellRow.Cells)
		{
			if(Cell->CanFitItem(InItem))
			{
				return true;
			}
		}
	}

	return false;
}

bool UInventoryContainer::ContainsItem(UItemObject* InItem)
{
	for(auto CellRow : Cells)
	{
		for(auto Cell : CellRow.Cells)
		{
			if(Cell->GetItems().Contains(InItem))
			{
				return true;
			}
		}
	}

	return false;
}

TArray<UItemObject*> UInventoryContainer::GetItems()
{
	TArray<UItemObject*> TempItems;
	for(auto CellRow : Cells)
	{
		for(auto Cell : CellRow.Cells)
		{
			for(auto ItemInCell : Cell->GetItems())
			{
				if(ItemInCell)
				{
					if(!TempItems.Contains(ItemInCell))
					{
						TempItems.Add(ItemInCell);
					}

					if(ItemInCell->IsContainer())
					{
						for(auto NestedItem : ItemInCell->GetInventoryContainer()->GetItems())
						{
							if(!TempItems.Contains(NestedItem))
							{
								TempItems.Add(NestedItem);
							}
						}
					}
				}
			}
		}
	}

	return TempItems;
}

UItemObject* UInventoryContainer::HasItemOfRowName(FName RowName)
{
	for(auto CellRow : Cells)
	{
		for(auto Cell : CellRow.Cells)
		{
			for(auto ItemInCell : Cell->GetItems())
			{
				if(ItemInCell)
				{
					UE_LOG(LogTemp, Warning, TEXT("item name: %s"), *ItemInCell->GetRowName().ToString());
				}
				if(ItemInCell && ItemInCell->GetRowName() == RowName)
				{
					return ItemInCell;
				}
			}
		}
	}

	return nullptr;
}
