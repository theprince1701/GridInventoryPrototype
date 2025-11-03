// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentSlotUpdated, EItemTypes, ItemType);

USTRUCT(BlueprintType)
struct FCharacterEquipmentSlot
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EItemTypes> ItemType;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UItemObject> Item;

	bool HasItemInSlot() const;
};

UCLASS()
class FPS_API UPlayerInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<FCharacterEquipmentSlot> CharacterEquipmentSlots;

public:
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotUpdated OnEquipmentSlotUpdated;

	virtual bool TryAddItem(UItemObject* InItem) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnItemRemoved(UItemObject* InItem);
	
	UFUNCTION(BlueprintCallable)
	int32 GetEquipmentSlotIndex(EItemTypes InItemType);
};
