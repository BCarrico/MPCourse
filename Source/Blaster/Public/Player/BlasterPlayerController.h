// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

class ABlasterHUD;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */



UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABlasterPlayerController();
	virtual void OnPossess(APawn* InPawn) override;

	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void ShowEliminatedMessage(bool bShow);
	void HideEliminatedMessage();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	FTimerHandle ElimMessageTimer;

	UPROPERTY()
	ABlasterHUD* BlasterHUD;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> BlasterContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> EquipAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ReloadAction;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void JumpButtonPressed();
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void Aiming(const FInputActionValue& InputActionValue);
	void FireButtonPressed(const FInputActionValue& InputActionValue);
	void ReloadButtonPressed();
};
