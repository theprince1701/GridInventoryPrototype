// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryContainer.h"
#include "IconGeneration/IconActor.h"
#include "UObject/NoExportTypes.h"
#include "ItemObject.generated.h"

class AItemActor;

UENUM(BlueprintType)
enum EItemTypes : uint8
{
	Backpack,
	TacticalVest,
	Medical,
	Weapon,
	Tech,
	Edible,
	Helmet,
	Headset,
	BodyArmor,
	FaceMask,
	Eyes,
	Ammo,
	Magazine
};

UENUM(BlueprintType)
enum EItemActions : uint8
{
	Inspect,
	Discard,
	Open,
	Unload,
	Load
};

USTRUCT(BlueprintType)
struct FItemStatsInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName StatName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString StatValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FColor StatValueColor = FColor::White;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> StatIcon;
};


USTRUCT(BlueprintType)
struct FContainerRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector2D> RowDimensions;
};

USTRUCT(BlueprintType)
struct FItemStats : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MultiLine="true"))
	FText ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D ItemDimensions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseMaterialForIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TEnumAsByte<EItemActions>> ItemActions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemStatsInfo> ItemStats;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EItemTypes> ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDataTableRowHandle> CompatibleItems;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxStackSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator DefaultIconRotation = FRotator(0,0,90);
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator RotatedIconRotation = FRotator(90,-90,180);
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D IconResolution = FVector2D(512.f, 512.f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //if this item is a container for items ContainerRows will be > 0
	TArray<FContainerRow> ContainerRows;

	

};

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStackSizeChanged);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDimensionsChanged);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemBusyChanged);

UCLASS(BlueprintType, Blueprintable)
class FPS_API UItemObject : public UObject
{
	GENERATED_BODY()

	
public:
	UPROPERTY(BlueprintAssignable)
	FOnStackSizeChanged OnStackSizeChanged;
	UPROPERTY(BlueprintAssignable)
	FOnDimensionsChanged OnDimensionsChanged;
	UPROPERTY(BlueprintAssignable)
	FOnItemBusyChanged OnItemBusyChanged;
	UFUNCTION(BlueprintCallable)
	UInventoryContainer* GetInventoryContainer() const { return InventoryContainer; }

	UFUNCTION(BlueprintCallable)
	TArray<FContainerRow> GetContainerRows() const { return ContainerRows; }

	UFUNCTION(BlueprintCallable)
	virtual UTexture2D* GetDefaultIcon() const	{ return Icon; }

	UFUNCTION(BlueprintCallable)
	virtual UTexture2D* GetIconRotated() const	{ return IconRotated; }
	
	UFUNCTION(BlueprintCallable)
	virtual FItemStats GetStats() const { return ItemStats; }

	UFUNCTION(BlueprintCallable)
	virtual bool GetIsRotated() const { return bIsRotated; }

	UFUNCTION(BlueprintCallable)
	virtual bool CanAddToStack() const { return StackSize < MaxStackSize; }
	
	UFUNCTION(BlueprintCallable)
	virtual TSubclassOf<AActor> GetItemClass() const { return ItemStats.ItemClass; }
	
	UFUNCTION(BlueprintCallable)
	virtual FName GetItemName() const { return ItemStats.ItemName; }

	UFUNCTION(BlueprintCallable)
	virtual EItemTypes GetItemType() const { return ItemStats.ItemType; }

	UFUNCTION(BlueprintCallable)
	virtual TArray<TEnumAsByte<EItemActions>> GetItemActions() const;
	
	UFUNCTION(BlueprintCallable)
	virtual FName GetRowName() const { return RowName; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsContainer();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FVector2D GetDimensions();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual UTexture2D* GetUpdatedIcon();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetWeight() const { return ItemStats.Weight; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsStackable();

	UFUNCTION(BlueprintCallable)
	virtual UContainerCell* GetOwnerCell() const { return OwnerCell; }

	UFUNCTION(BlueprintCallable)
	virtual void SetContainerCell(UContainerCell* Cell) { OwnerCell = Cell; }
	
	UFUNCTION(BlueprintCallable)
	virtual  int32 GetStackSize() const { return StackSize; }

	UFUNCTION(BlueprintCallable)
	virtual int32 GetMaxStackSize() const { return MaxStackSize; }

	UFUNCTION(BlueprintCallable)
	virtual bool DropItemActorInsteadOfDefaultClass() const { return false; }
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void GenerateIcons();
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void InitializeItem(FItemStats InItem, FName InRowName);

	UFUNCTION(BlueprintCallable)
	virtual void RotateItem() { bIsRotated = !bIsRotated; }

	UFUNCTION(BlueprintCallable)
	virtual void SetDefaultIcon(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable)
	virtual void SetRotatedIcon(UTexture2D* RotatedIcon);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetStackSize(int32 Increase, int& OutRemainder);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetStackSizeNoIncrement(int32 NewStackSize);

	UFUNCTION(BlueprintCallable)
	AItemActor* GetItemActor() const { return ItemActor; }
	
	UFUNCTION(BlueprintCallable)
	void SetItemActor(AItemActor* InItemActor) { ItemActor = InItemActor; }

	UFUNCTION(BlueprintCallable)
	bool GetItemBusy() const { return bItemBusy; }
	UFUNCTION(BlueprintCallable)
	void SetItemBusy(bool bValue);

	
	//dragging
public:
	UFUNCTION(BlueprintCallable)
	virtual bool CanItemBeDroppedOn(UItemObject* InItem);
	UFUNCTION(BlueprintCallable)
	virtual bool DropItem(UItemObject* InItem);
	
private:
	UPROPERTY()
	TObjectPtr<UInventoryContainer> InventoryContainer;

	UPROPERTY()
	TArray<FContainerRow> ContainerRows;

	UPROPERTY()
	FVector2D Dimensions;

	UPROPERTY()
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY()
	TObjectPtr<UTexture2D> IconRotated;

	UPROPERTY()
	TObjectPtr<AItemActor> ItemActor;
	
	UPROPERTY()
	FItemStats ItemStats;

	UPROPERTY()
	bool bIsRotated;

	UPROPERTY()
	TObjectPtr<UContainerCell> OwnerCell;

	int32 StackSize;
	int32 MaxStackSize;

	FName RowName;

	bool bItemBusy;
};
