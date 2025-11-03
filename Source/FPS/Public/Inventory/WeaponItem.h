// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/ItemObject.h"
#include "Weapons/Weapon.h"
#include "WeaponItem.generated.h"


UCLASS()
class FPS_API UWeaponItem : public UItemObject
{
	GENERATED_BODY()


public:
	void SetWeapon(AWeapon* InWeapon);
	
	virtual TArray<TEnumAsByte<EItemActions>> GetItemActions() const override;

	UFUNCTION(BlueprintCallable)
	AWeapon* GetWeapon() const { return Weapon; }

	void GenerateIcon();

	virtual FVector2d GetDimensions() override;
	virtual UTexture2D* GetDefaultIcon() const override;
	virtual UTexture2D* GetUpdatedIcon() override;
	virtual UTexture2D* GetIconRotated() const override;

	virtual int32 GetStackSize() const override;
	virtual int32 GetMaxStackSize() const override;
	virtual bool IsStackable() override;

	virtual FVector2D GetDimensionsNonSwapped();
	
	virtual bool CanItemBeDroppedOn(UItemObject* InItem) override;
	virtual bool DropItem(UItemObject* InItem) override;

	virtual bool DropItemActorInsteadOfDefaultClass() const override;

private:
	UPROPERTY()
	TObjectPtr<AWeapon> Weapon;

	UPROPERTY()
	TObjectPtr<UTexture2D> DefaultIcon;
	UPROPERTY()
	TObjectPtr<UTexture2D> RotatedIcon;
};
