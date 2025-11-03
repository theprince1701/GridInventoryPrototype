// Fill out your copyright notice in the Description page of Project Settings.


#include "IconGeneration/IconGenerationSubSystem.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetMathLibrary.h"

UTextureRenderTarget2D* UIconGenerationSubSystem::GenerateIcon(UWeaponItem* InItem)
{
	check(InItem);
	check(InItem->GetWeapon())
	AWeapon* Weapon = InItem->GetWeapon();
	
	UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>(this, UTextureRenderTarget2D::StaticClass(), "DefaultIcon");
	
	RenderTarget->InitAutoFormat(InItem->GetDimensionsNonSwapped().X * 512, InItem->GetDimensionsNonSwapped().Y * 512);
	RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
	RenderTarget->ClearColor = FLinearColor(0, 0, 0, 0);
	
	USceneCaptureComponent2D* SceneCapture = Weapon->DefaultSceneCaptureComponent;

	UE_LOG(LogTemp, Warning, TEXT("scene capture display name: %s"), *SceneCapture->GetName());
	if(SceneCapture)
	{
		SceneCapture->TextureTarget = RenderTarget;
		SceneCapture->CaptureScene();
	}

	return RenderTarget;
}

UTextureRenderTarget2D* UIconGenerationSubSystem::GenerateRotatedIcon(UWeaponItem* InItem)
{
	check(InItem);
	check(InItem->GetWeapon())
	AWeapon* Weapon = InItem->GetWeapon();
	
	UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>(this, UTextureRenderTarget2D::StaticClass(), "RotatedIcon");
	
	RenderTarget->InitAutoFormat(InItem->GetDimensions().X * 512, InItem->GetDimensions().Y * 512);
	RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
	RenderTarget->ClearColor = FLinearColor(0, 0, 0, 0);
	
	USceneCaptureComponent2D* SceneCapture = Weapon->DefaultSceneCaptureComponent;
	
	
	if(SceneCapture)
	{
		/*/
		FVector Origin, BoxExtent;
		Weapon->GetActorBounds(false, Origin, BoxExtent);
		
		float AspectRatio = static_cast<float>(InItem->GetDimensions().X * 512) / InItem->GetDimensions().Y * 512;

		// Determine maximum dimension based on actor bounds
		float MaxDimension = FMath::Max(BoxExtent.X, BoxExtent.Z * AspectRatio);

		// Calculate the distance needed based on FOV and aspect ratio
		float DesiredDistance = MaxDimension / FMath::Tan(FMath::DegreesToRadians(SceneCapture->FOVAngle * 0.5f));

		// Adjust SceneCapture location to fit the entire actor in view
		FVector CaptureOffset(0, 0, DesiredDistance);

		SceneCapture->SetRelativeLocation(SceneCapture->GetRelativeLocation() + CaptureOffset);

		/*/
		SceneCapture->TextureTarget = RenderTarget;
		SceneCapture->CaptureScene();
	}

	return RenderTarget;
}

