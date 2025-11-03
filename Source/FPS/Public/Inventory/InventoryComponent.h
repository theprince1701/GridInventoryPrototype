// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, UItemObject*, Item);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryToggled, bool, IsOpen);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TileSize = 75.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnItemAdded OnItemAdded;
	UPROPERTY(BlueprintAssignable)
	FOnInventoryToggled OnInventoryToggled;
	UFUNCTION(BlueprintCallable)
	bool IsInventoryOpen() const { return bIsInventoryOpen; }
	
	UFUNCTION(BlueprintCallable)
	TArray<UItemObject*> GetItems() const { return Items; }

	UFUNCTION(BlueprintCallable)
	virtual bool TryAddItem(UItemObject* InItem);

	UFUNCTION(BlueprintCallable)
	UInventoryContainer* GetInventoryContainer(EItemTypes ItemType);
	
	UFUNCTION(BlueprintCallable)
	void RemoveItem(UItemObject* InItem);

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetInventoryWidget() const { return InventoryWidget; }

	UFUNCTION(BlueprintCallable)
	float GetTotalWeight();
	
	
protected:
	virtual void BeginPlay() override;
	
	bool HasItemOfType(EItemTypes ItemType);
	
	
private:
	UPROPERTY()
	TArray<TObjectPtr<UItemObject>> Items;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> InventoryWidget;

	bool bIsInventoryOpen;
};
