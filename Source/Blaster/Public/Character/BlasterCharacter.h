// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterTypes/TurningInPlace.h"
#include "GameFramework/Character.h"
#include "Blaster/Public/Interfaces/InteractWithCrosshairsInterface.h"
#include "Components/TimelineComponent.h"
#include "BlasterCharacter.generated.h"


enum class ECombatState : uint8;
class ABlasterPlayerState;
class UTimelineComponent;
class ABlasterPlayerController;
enum ETurningInPlace : uint8;
class UCombatComponent;
class AWeapon;
class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // Must be overriden in any class where you plan on replicating variables. It is where you register variables to be replicated.
	virtual void PostInitializeComponents() override;
	virtual void OnRep_ReplicatedMovement() override;
	virtual void Destroyed() override;
	
	void SetOverlappingWeapon(AWeapon* Weapon);
	void EquipWeapon();
	bool IsWeaponEquipped() const;
	bool IsAiming() const;
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch;}
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace;}
	FORCEINLINE UCameraComponent* GetFollowCamera() const{return FollowCamera;}
	FORCEINLINE bool ShouldRotateRootBone() const {return bRotateRootBone;}
	FORCEINLINE bool IsEliminated() const {return bEliminated;}
	FORCEINLINE float GetHealth() const {return Health;}
	FORCEINLINE float GetMaxHealth() const {return MaxHealth;}
	FORCEINLINE UCombatComponent* GetCombatComponent() const {return Combat;};
	FORCEINLINE bool GetDisableGameplay() const {return bDisableGameplay;}
	FORCEINLINE UAnimMontage* GetReloadMontage() const {return ReloadMontage;}
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const {return AttachedGrenade;}
	ECombatState GetCombatState() const;
	AWeapon* GetEquippedWeapon();
	void PlayFireMontage(bool bAiming);
	void PlayElimMontage();
	void PlayReloadMontage();
	void PlayThrowGrenadeMontage();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();

	void Elim();
	
	FVector GetHitTarget() const;

	UPROPERTY(Replicated)
	bool bDisableGameplay = false;
protected:
	
	virtual void BeginPlay() override;
	void CalculateAO_Pitch();
	float CalculateSpeed();
	void AimOffset(float DeltaTime);
	void SimProxiesTurn();
	virtual void Jump() override;
	void PlayHitReactMontage();
	void UpdateHudHealth();
	
	// Poll for any relevant classes and initialize our HUD
	void PollInit();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCombatComponent* Combat;
	
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
private:
	UPROPERTY(VisibleAnywhere, Category="Camera")
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, Category="Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon) // Creates a replicated variable, calls OnRep_OverlappingWeapon when changed.
	AWeapon* OverlappingWeapon;

	

	// Animation Montages
	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* ElimMontage;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* ThrowGrenadeMontage;

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon); // Rep notifies can only have input parameter of the type of variable being replicated
	void RotateInPlace(float DeltaTime);

	void TurnInPlace(float DeltaTime);

	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;
	
	TEnumAsByte<ETurningInPlace> TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;

	// Player Health

	UPROPERTY(EditAnywhere, Category="Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing= OnRep_Health, VisibleAnywhere, Category="Player Stats")
	float Health = 100.f;

	UPROPERTY()
	ABlasterPlayerController* BlasterPlayerController;

	UFUNCTION()
	void OnRep_Health();

	bool bEliminated = false;

	FTimerHandle ElimTimer;
	
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;
	
	void ElimTimerFinished();

	// Dissolve Effect
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;
	
	FOnTimelineFloat DissolveTrack;
	
	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();

	// Dynamic Instance that we can change at runtime
	UPROPERTY(VisibleAnywhere, Category="Elim")
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;
	
	// Material instance set on the Blueprint, used with dynamic material instance
	UPROPERTY(EditAnywhere, Category="Elim")
	UMaterialInstance* DissolveMaterialInstance;

	// Elim bot

	UPROPERTY(EditAnywhere)
	UParticleSystem* ElimBotEffect;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ElimBotComponent;
	
	UPROPERTY(EditAnywhere)
	USoundCue* ElimBotSound;

	UPROPERTY()
	ABlasterPlayerState* BlasterPlayerState;

	// Grenade

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AttachedGrenade;
};
