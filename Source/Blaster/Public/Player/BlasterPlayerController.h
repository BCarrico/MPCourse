// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

class UReturnToMainMenu;
class ABlasterCharacter;
class ABlasterGameMode;
class UCharacterOverlay;
class ABlasterHUD;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool, bPingTooHigh);
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
	void SetHUDShield(float Shield, float MaxShield);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDGrenades(int32 Grenades);
	void ShowEliminatedMessage(bool bShow);
	void HideEliminatedMessage();
	void SetHUDMatchCountdown(float CountdownTime);
	void SetHUDAnnouncementCountdown(float CountdownTime);
	void OnMatchStateSet(FName State);

	float SingleTripTime = 0.f;
	virtual float GetServerTime(); // Synced with server world clock

	FHighPingDelegate HighPingDelegate;
protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	void CheckTimeSync(float DeltaSeconds);
	void CheckPing(float DeltaSeconds);
	virtual void SetupInputComponent() override;
	void SetHUDTime();
	void PollInit();
	void HandleMatchHasStarted();
	void HandleCooldown();
	void HighPingWarning();
	void StopHighPingWarning();
	// Sync Time between Client and Server

	UFUNCTION(Server, Reliable) // Requests the current Server time, passing in the Client's time when the request was sent
	void ServerRequestServerTime(float TimeOffClientRequest);

	UFUNCTION(Client, Reliable) // Reports the current server time to the client in response to ServerRequestServerTime
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f; // Difference between Client and Server time
	
	UPROPERTY(EditAnywhere, Category="Time")
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;
	
	
	virtual void ReceivedPlayer() override; // Sync with server clock as soon as possible

	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();

	UFUNCTION(Client, Reliable)
	void ClientJoinMidGame(FName StateOfMatch, float Warmup, float Match, float StartingTime, float Cooldown);

	void ShowReturnToMainMenu();
private:
	// Return To Main Menu
	UPROPERTY(EditAnywhere, Category= HUD)
	TSubclassOf<UUserWidget> ReturnToMainMenuWidget;
	
	UPROPERTY()
	UReturnToMainMenu* ReturnToMainMenu;

	bool bReturnToMainMenuOpen = false;
	
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

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ThrowGrenadeAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> QuitAction;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void JumpButtonPressed();
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void Aiming(const FInputActionValue& InputActionValue);
	void FireButtonPressed(const FInputActionValue& InputActionValue);
	void ReloadButtonPressed();
	void ThrowGrenadeButtonPressed();
	void QuitActionPressed();

	float LevelStartingTime = 0.f;
	float MatchTime = 0.f;
	float WarmupTime = 0.f;
	float CooldownTime = 0.f;
	uint32 CountdownInt = 0;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	UCharacterOverlay* CharacterOverlay;

	UPROPERTY()
	ABlasterGameMode* BlasterGameMode;
	
	
	bool bInitializeHealth = false;
	float HUDHealth;
	float HUDMaxHealth;
	bool bInitializeShield = false;
	float HUDShield;
	float HUDMaxShield;
	bool bInitializeScore = false;
	float HUDScore;
	bool bInitializeDefeats = false;
	int32 HUDDefeats;
	bool bInitializeGrenades = false;
	int32 HUDGrenades;
	bool bInitializeCarriedAmmo = false;
	float HUDCarriedAmmo;
	bool bInitializeWeaponAmmo = false;
	float HUDWeaponAmmo;

	float HighPingRunningTime = 0.f;
	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5.f;
	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 20.f;

	UFUNCTION(Server, Reliable)
	void ServerReportPingStatus(bool bHighPing);
	
	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 50.f;
	float PingAnimationRunningTime = 0.f;

};
