// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

USTRUCT(BlueprintType)
struct FItemObjectClasses : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EItemTypes> ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UItemObject> ItemObjectClass;
};

UCLASS()
class FPS_API AItemActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle ItemRowName;
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle ItemObjectRowName;
	
public:
	virtual void InitializeItemActor();

	UFUNCTION(BlueprintCallable)
	virtual void OverrideItemObject(UItemObject* InItemObject);

	UFUNCTION(BlueprintCallable)
	UItemObject* GetItemObject() const { return ItemObject; }
	
protected:
	virtual void BeginPlay() override;
	virtual UItemObject* ConstructDefaultItem();

protected:
	UPROPERTY()
	TObjectPtr<UItemObject> ItemObject;

};
