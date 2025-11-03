// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS/FPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"


UCLASS()
class FPS_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	class FSavedMove_Player : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		uint8 Saved_bWantsToSprint:1;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Player : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Player(const UCharacterMovementComponent& InMovement);
		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

	//flag that gets set on client when input is pressed
	bool bSafeWantsToSprint;

	UPROPERTY(EditDefaultsOnly, Category=Speeds)
	float SprintMaxSpeed;
	UPROPERTY(EditDefaultsOnly, Category=Speeds)
	float WalkMaxSpeed;
	UPROPERTY(EditDefaultsOnly, Category=Speeds)
	float HorizontalMaxSpeedMultiplier = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category=Speeds)
	float BackwardsMaxSpeedMultiplier = 0.45f;
	UPROPERTY(EditDefaultsOnly, Category=Speeds)
	float AimMaxSpeedMultiplier = 0.75f;
	UPROPERTY(EditDefaultsOnly, Category=Speeds)
	float WeightMaxSpeedMultiplier = .2f;

	UPROPERTY(EditDefaultsOnly, Category=Stamina)
	float StaminaDrainRateWeightMultiplier = 1.2f;
	UPROPERTY(EditDefaultsOnly, Category=Stamina)
	float MaxStamina = 1.f;
	UPROPERTY(EditDefaultsOnly, Category=Stamina)
	float StaminaDrainRate = .1f;
	UPROPERTY(EditDefaultsOnly, Category=Stamina)
	float MinStaminaBeforeCanSprint = .1f;
	UPROPERTY(EditDefaultsOnly, Category=Stamina)
	float BlockStaminaRegenTime = 0.25f;


	
	UPROPERTY()
	TObjectPtr<AFPSCharacter> Character;
	
public:
	UPlayerMovementComponent();
	
public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	UFUNCTION(BlueprintCallable)
	float GetCurrentStamina() const { return Stamina; }
protected:
	virtual void BeginPlay() override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual float GetMaxSpeed() const override;
	
public:
	void SprintPressed();
	void SprintReleased();

	void CrouchPressed();

	UFUNCTION(BlueprintCallable)
	bool IsSprinting() const { return bSafeWantsToSprint; }

private:
	float Stamina;
	bool bBlockStaminaRegen;
	bool bStartedStaminaBlock;

	FTimerHandle StaminaBlockTimerHandle;
	void ResetBlockStamina();
};
