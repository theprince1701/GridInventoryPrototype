// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IconActor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Inventory/ItemObject.h"
#include "Inventory/WeaponItem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IconGenerationSubSystem.generated.h"

UCLASS()
class FPS_API UIconGenerationSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UTextureRenderTarget2D* GenerateIcon(UWeaponItem* InItem);
	UTextureRenderTarget2D* GenerateRotatedIcon(UWeaponItem* InItem);

private:
};
