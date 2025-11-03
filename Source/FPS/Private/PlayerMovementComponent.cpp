// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"

bool UPlayerMovementComponent::FSavedMove_Player::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter,
	float MaxDelta) const
{
	FSavedMove_Player* NewPlayerMove = static_cast<FSavedMove_Player*>(NewMove.Get());

	if(Saved_bWantsToSprint != NewPlayerMove->Saved_bWantsToSprint)
	{
		return false;
	}

	
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UPlayerMovementComponent::FSavedMove_Player::Clear()
{
	FSavedMove_Character::Clear();
	Saved_bWantsToSprint = 0;
}

uint8 UPlayerMovementComponent::FSavedMove_Player::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if(Saved_bWantsToSprint) Result |= FLAG_Custom_0;
	
	return Result;
}

void UPlayerMovementComponent::FSavedMove_Player::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel,
	class FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UPlayerMovementComponent* PlayerMovement = Cast<UPlayerMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint = PlayerMovement->bSafeWantsToSprint;
}

void UPlayerMovementComponent::FSavedMove_Player::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UPlayerMovementComponent* PlayerMovement = Cast<UPlayerMovementComponent>(C->GetCharacterMovement());

	PlayerMovement->bSafeWantsToSprint = Saved_bWantsToSprint;
	
}

UPlayerMovementComponent::FNetworkPredictionData_Client_Player::FNetworkPredictionData_Client_Player(
	const UCharacterMovementComponent& InMovement) : Super(InMovement)
{
}

FSavedMovePtr UPlayerMovementComponent::FNetworkPredictionData_Client_Player::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Player());
}

UPlayerMovementComponent::UPlayerMovementComponent()
{
}

FNetworkPredictionData_Client* UPlayerMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);

	if(ClientPredictionData == nullptr)
	{
		UPlayerMovementComponent* MutableThis = const_cast<UPlayerMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Player(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
	
	//return Super::GetPredictionData_Client();
}

void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AFPSCharacter>(GetOwner());
	bBlockStaminaRegen = false;
	Stamina = MaxStamina;
}

void UPlayerMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bSafeWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UPlayerMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	
	if(bSafeWantsToSprint)
	{
		Stamina -= StaminaDrainRate * DeltaSeconds;
		Stamina = FMath::Clamp(Stamina, 0, MaxStamina);

		if(Stamina <= 0)
		{
			bBlockStaminaRegen = true;
			GetWorld()->GetTimerManager().SetTimer(StaminaBlockTimerHandle, this,
				&UPlayerMovementComponent::ResetBlockStamina, BlockStaminaRegenTime, false);
		}
	}
	else
	{
		if(!bBlockStaminaRegen)
		{
			Stamina += StaminaDrainRate * DeltaSeconds;
			Stamina = FMath::Clamp(Stamina, 0, MaxStamina);
		}
	}
	
	if(bSafeWantsToSprint && ((Velocity.Size() <= 0) ||
		(Character->GetWalkHorizontal() != 0.0f && Character->GetWalkVertical() == 0) || Character->GetWalkVertical() < 0.0f || Stamina <= 0.f))
	{
		bSafeWantsToSprint = false;
	}
	
	
	/*/
	if(MovementMode == MOVE_Walking)
	{
		if(bSafeWantsToSprint)
		{
			MaxWalkSpeed = SprintMaxSpeed;
		}
		else
		{
			MaxWalkSpeed = WalkMaxSpeed;
		}
	}
	/*/
}

float UPlayerMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if(Character->GetInventoryComponent()->GetTotalWeight() > 1)
	{
		MaxSpeed /= Character->GetInventoryComponent()->GetTotalWeight() * WeightMaxSpeedMultiplier;
	}
	
	
	if(bSafeWantsToSprint)
	{
		MaxSpeed = SprintMaxSpeed;
	}

	if(Character->GetWalkHorizontal() != 0.0f)
	{
		MaxSpeed *= HorizontalMaxSpeedMultiplier;
	}

	if(Character->GetWalkVertical() < 0.0f)
	{
		MaxSpeed *= BackwardsMaxSpeedMultiplier;
	}

	if(Character->GetIsAiming())
	{
		MaxSpeed *= AimMaxSpeedMultiplier;
	}
	
	if(!bSafeWantsToSprint)
	{
		MaxSpeed = FMath::Clamp(MaxSpeed, 0, WalkMaxSpeed);
	}
	
	return MaxSpeed;
}

void UPlayerMovementComponent::SprintPressed()
{
	if(Stamina <= MinStaminaBeforeCanSprint)
	{
		bSafeWantsToSprint = false;
		return;
	}
	
	bSafeWantsToSprint = true;
}

void UPlayerMovementComponent::SprintReleased()
{
	bSafeWantsToSprint = false;
}

void UPlayerMovementComponent::CrouchPressed()
{
	bWantsToCrouch = !bWantsToCrouch;
}

void UPlayerMovementComponent::ResetBlockStamina()
{
	UE_LOG(LogTemp, Warning, TEXT("reset changed"));
	bBlockStaminaRegen = false;
}
