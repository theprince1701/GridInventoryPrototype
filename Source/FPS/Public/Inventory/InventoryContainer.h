// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryContainer.generated.h"

class UInventoryComponent;
class UItemObject;
class UContainerCell;

USTRUCT(BlueprintType)
struct FContainerCells
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UContainerCell>> Cells;
};

UCLASS(BlueprintType, Blueprintable)
class FPS_API UInventoryContainer : public UObject
{
	GENERATED_BODY()

public:
	void InitializeContainer(UItemObject* InItem);

	UFUNCTION(BlueprintCallable)
	bool TryAddItemToCell(UItemObject* InItem);
	
	UFUNCTION(BlueprintCallable)
	bool CanFitItem(UItemObject* InItem);
	
	UFUNCTION(BlueprintCallable)
	bool ContainsItem(UItemObject* InItem);

	UFUNCTION(BlueprintCallable)
	TArray<FContainerCells> GetCells() const { return Cells; }
	
	UFUNCTION(BlueprintCallable)
	UItemObject* GetItem() const { return Item; }
	
	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	
	UFUNCTION(BlueprintCallable)
	TArray<UItemObject*> GetItems();

	UFUNCTION(BlueprintCallable)
	UItemObject* HasItemOfRowName(FName RowName);

	
	void SetInventoryComponent(UInventoryComponent* InInventoryComp) { InventoryComponent = InInventoryComp; }

private:
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY()
	TArray<FContainerCells> Cells;

	UPROPERTY()
	TObjectPtr<UItemObject> Item;
};
