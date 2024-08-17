// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

class UCharacterOverlay;
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void ShowEliminatedMessage(bool bShow);
	void HideEliminatedMessage();
	void SetHUDMatchCountdown(float CountdownTime);
	void OnMatchStateSet(FName State);
protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	void CheckTimeSync(float DeltaSeconds);
	virtual void SetupInputComponent() override;
	void SetHUDTime();
	void PollInit();
	void HandleMatchHasStarted();
	// Sync Time between Client and Server

	UFUNCTION(Server, Reliable) // Requests the current Server time, passing in the Client's time when the request was sent
	void ServerRequestServerTime(float TimeOffClientRequest);

	UFUNCTION(Client, Reliable) // Reports the current server time to the client in response to ServerRequestServerTime
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f; // Difference between Client and Server time
	
	UPROPERTY(EditAnywhere, Category="Time")
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;
	
	virtual float GetServerTime(); // Synced with server world clock
	virtual void ReceivedPlayer() override; // Sync with server clock as soon as possible
	

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

	float MatchTime = 120.f;
	uint32 CountdownInt = 0;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	UCharacterOverlay* CharacterOverlay;
	
	bool bInitializeCharacterOverlay = false;
	float HUDHealth;
	float HUDMaxHealth;
	float HUDScore;
	int32 HUDDefeats;

};
