// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletItem.h"
#include "Inventory/ItemObject.h"
#include "MagazineItem.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UMagazineItem : public UItemObject
{
	GENERATED_BODY()

public:
	virtual int32 GetStackSize() const override;
	virtual TArray<TEnumAsByte<EItemActions>> GetItemActions() const override;

	UFUNCTION(BlueprintCallable)
	void UnloadMagazine();
private:
	UPROPERTY()
	TArray<FBulletStats> Bullets;
};
