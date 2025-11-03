// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/ItemObject.h"
#include "BulletItem.generated.h"

USTRUCT(BlueprintType)
struct FBulletStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MuzzleVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> DamageCurve;
};

UCLASS(Blueprintable, BlueprintType)
class FPS_API UBulletItem : public UItemObject
{
	GENERATED_BODY()
	
};
