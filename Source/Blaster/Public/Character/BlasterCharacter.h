// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterTypes/TurningInPlace.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

enum ETurningInPlace : uint8;
class UCombatComponent;
class AWeapon;
class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // Must be overriden in any class where you plan on replicating variables. It is where you register variables to be replicated.
	virtual void PostInitializeComponents() override;
	UCombatComponent* GetCombatComponent() const;
	
	void SetOverlappingWeapon(AWeapon* Weapon);
	void EquipWeapon();
	bool IsWeaponEquipped() const;
	bool IsAiming() const;
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch;}
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace;}
	AWeapon* GetEquippedWeapon();
protected:
	
	virtual void BeginPlay() override;
	void AimOffset(float DeltaTime);
private:
	UPROPERTY(VisibleAnywhere, Category="Camera")
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, Category="Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon) // Creates a replicated variable, calls OnRep_OverlappingWeapon when changed.
	AWeapon* OverlappingWeapon;

	UPROPERTY(VisibleAnywhere)
	UCombatComponent* Combat;

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon); // Rep notifies can only have input parameter of the type of variable being replicated

	void TurnInPlace(float DeltaTime);
	
	TEnumAsByte<ETurningInPlace> TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	
	float AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
};
