// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BlasterPlayerController.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "BlasterComponents/CombatComponent.h"
#include "Character/BlasterCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUD/BlasterHUD.h"
#include "HUD/CharacterOverlay.h"

ABlasterPlayerController::ABlasterPlayerController()
{
	bReplicates = true;
}

void ABlasterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(BlasterContext, 0);
	}
}

void ABlasterPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD && BlasterHUD->CharacterOverlay && BlasterHUD->CharacterOverlay->HealthBar && BlasterHUD->CharacterOverlay->HealthText)
	{
		const float HealthPercent = Health / MaxHealth;
		BlasterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		BlasterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}

void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	BlasterHUD = Cast<ABlasterHUD>(GetHUD());
	check(BlasterContext);
	
	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);

	if (!HasAuthority())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BlasterContext, 0);
		}
	}
}

void ABlasterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABlasterPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABlasterPlayerController::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABlasterPlayerController::JumpButtonPressed);
	EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ABlasterPlayerController::EquipButtonPressed);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ABlasterPlayerController::CrouchButtonPressed); // Also set as triggered in IA_Crouch due to stuttering in game. Fixed it
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &ABlasterPlayerController::Aiming); // See IA_Aiming for Pressed & Released
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ABlasterPlayerController::FireButtonPressed);
}

void ABlasterPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	
}

void ABlasterPlayerController::Look(const FInputActionValue& InputActionValue)
{
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// add yaw and pitch input to controller
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABlasterPlayerController::JumpButtonPressed()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ACharacter* BlasterCharacter = Cast<ACharacter>(ControlledPawn))
		{
			BlasterCharacter->Jump();
		}
	}
}

void ABlasterPlayerController::EquipButtonPressed()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(ControlledPawn))
		{
			BlasterCharacter->EquipWeapon();
		}
	}
}

void ABlasterPlayerController::CrouchButtonPressed()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ACharacter* BlasterCharacter = Cast<ACharacter>(ControlledPawn))
		{
			if (BlasterCharacter->bIsCrouched)
			{
				BlasterCharacter->UnCrouch();
			}
			else
			{
				BlasterCharacter->Crouch();
			}
		}
	}
}

void ABlasterPlayerController::Aiming(const FInputActionValue& InputActionValue)
{
	bool IsAiming = InputActionValue.Get<bool>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(ControlledPawn))
		{
			if (UCombatComponent* CombatComponent = BlasterCharacter->GetCombatComponent())
			{
				CombatComponent->SetAiming(IsAiming);
			}
		}
	}
}

void ABlasterPlayerController::FireButtonPressed(const FInputActionValue& InputActionValue)
{
	bool IsFiring = InputActionValue.Get<bool>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(ControlledPawn))
		{
			if (UCombatComponent* CombatComponent = BlasterCharacter->GetCombatComponent())
			{
				CombatComponent->FireButtonPressed(IsFiring);
			}
		}
	}
}
