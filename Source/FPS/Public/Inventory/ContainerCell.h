// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryContainer.h"
#include "UObject/NoExportTypes.h"
#include "ContainerCell.generated.h"

USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Y;
};

USTRUCT(BlueprintType)
struct FItemIndex
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool bValid;
	UPROPERTY()
	TObjectPtr<UItemObject> Item;
};

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOInventoryChangedDelegate);

UCLASS(BlueprintType, Blueprintable)
class FPS_API UContainerCell : public UObject
{
	GENERATED_BODY()

public:
	void InitializeCell(UInventoryContainer* InInventoryContainer, FVector2D InDimensions);

	UPROPERTY(BlueprintAssignable)
	FOInventoryChangedDelegate OnInventoryChanged;

public:
	UFUNCTION(BlueprintCallable)
	FTile IndexToTile(int32 InIndex);
	UFUNCTION(BlueprintCallable)
	int32 TileToIndex(FTile InTile);

	UFUNCTION(BlueprintCallable)
	FItemIndex GetItemAtIndex(int32 InIndex);

	UFUNCTION(BlueprintCallable)
	bool IsRoomAvaliable(UItemObject* InItem, int32 TopLeftIndex);
	UFUNCTION(BlueprintCallable)

	bool IsTileValid(FTile Tile);

	UFUNCTION(BlueprintCallable)
	void AddItemAt(UItemObject* InItem, int32 InTopLeftIndex);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(UItemObject* InItem);
	UFUNCTION(BlueprintCallable)
	bool TryAddItem(UItemObject* InItem);
	UFUNCTION(BlueprintCallable)
	bool CanFitItem(UItemObject* InItem);

	bool TryAddItemRotated(UItemObject* InItem);
	
	UFUNCTION(BlueprintCallable)
	TMap<UItemObject*, FTile> GetAllItems();

	UFUNCTION(BlueprintCallable)
	TArray<UItemObject*> GetItems() const { return Items; }

	UFUNCTION(BlueprintCallable)
	FVector2D GetDimensions() const { return Dimensions; }

	UFUNCTION(BlueprintCallable)
	UInventoryContainer* GetInventoryContainer() const { return InventoryContainer; }

	bool CanFitIncreasedDimensions(UItemObject* InItem, int32 DimensionXIncrease, int32 DimensionYIncrease);
	bool IsRoomAvailableForDimensions(int32 TopLeftIndex, FVector2D NewDimensions);

	int32 GetItemIndex(UItemObject* InItem);
	
private:
	UPROPERTY()
	TArray<TObjectPtr<UItemObject>> Items;

	UPROPERTY()
	FVector2D Dimensions;

	UPROPERTY()
	TObjectPtr<UInventoryContainer> InventoryContainer;

	bool bHasTriedToAddRotated;

};


