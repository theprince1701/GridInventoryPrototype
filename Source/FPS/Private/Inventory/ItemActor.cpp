// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemActor.h"



void AItemActor::InitializeItemActor()
{
}

void AItemActor::OverrideItemObject(UItemObject* InItemObject)
{
	ItemObject = InItemObject;
}


void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	if(!ItemObject)
	{
		ConstructDefaultItem();
		InitializeItemActor();
	}
}

UItemObject* AItemActor::ConstructDefaultItem()
{
	const FString ContextString(TEXT("GetItemData"));

	FItemStats* ItemStats = ItemRowName.DataTable->FindRow<FItemStats>(ItemRowName.RowName, ContextString);
	FItemObjectClasses* ItemObjectClass = ItemObjectRowName.DataTable->FindRow<FItemObjectClasses>(ItemObjectRowName.RowName, ContextString);

	check(ItemStats);
	check(ItemObjectClass);

	ItemObject = NewObject<UItemObject>(this, ItemObjectClass->ItemObjectClass);

	if(ItemObject)
	{
		ItemObject->InitializeItem(*ItemStats, ItemRowName.RowName);
		ItemObject->SetItemActor(this);
		if(ItemObject->IsStackable())
		{
			int32 OutRemainder;
			ItemObject->SetStackSize(FMath::RandRange(0, ItemStats->MaxStackSize), OutRemainder);
		}

		return ItemObject;
	}

	return nullptr;
}
