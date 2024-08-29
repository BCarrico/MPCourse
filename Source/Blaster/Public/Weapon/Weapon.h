// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponType.h"
#include "Sound/SoundCue.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class ABlasterPlayerController;
class ABlasterCharacter;
class ACasing;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EFireType : uint8
{
	EFT_HitScan UMETA(DisplayName = "Hit Scan Weapon"),
	EFT_Projectile UMETA(DisplayName = "Projectile Weapon"),
	EFT_Shotgun UMETA(DisplayName = "Shotgun Weapon"),
	EFT_MAX UMETA(DisplayName = "DefaultMAX"),
};
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_EquippedSecondary UMETA(DisplayName = "Equipped Secondary"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};
class USphereComponent;

UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	void ShowPickupWidget(bool bShowWidget);
	virtual void Fire(const FVector& HitTarget);
	void SetWeaponState(EWeaponState State);
	void Dropped();
	FORCEINLINE USphereComponent* GetAreaSphere() const {return AreaSphere;}
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const {return WeaponMesh;}
	FORCEINLINE float GetZoomedFOV() const{return ZoomedFOV;}
	FORCEINLINE float GetZoomInterpSpeed() const{return ZoomInterpSpeed;}
	FORCEINLINE EWeaponType GetWeaponType() const {return WeaponType;}
	FORCEINLINE int32 GetAmmo() const {return Ammo;}
	FORCEINLINE int32 GetMagCapacity() const {return MagCapacity;}
	FORCEINLINE float GetDamage() const {return Damage;}
	void AddAmmo(int32 AmmoToAdd);
	FVector TraceEndWithScatter(const FVector& HitTarget);
	// Textures for the weapon crosshairs

	UPROPERTY(EditAnywhere, Category="Crosshairs")
	UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category="Crosshairs")
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category="Crosshairs")
	UTexture2D* CrosshairsRight;
	
	UPROPERTY(EditAnywhere, Category="Crosshairs")
	UTexture2D* CrosshairsTop;
	
	UPROPERTY(EditAnywhere, Category="Crosshairs")
	UTexture2D* CrosshairsBottom;

	// Automatic Fire

	UPROPERTY(EditAnywhere, Category="Combat")
	bool bAutomatic = true;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere)
	USoundCue* EquipSound;
	
	void SetHUDAmmo();
	bool IsEmpty();
	bool IsFull();

	// Enable or disable custom depth

	void EnableCustomDepth(bool bEnable);

	bool bDestroyWeapon = false;
	
	UPROPERTY(EditAnywhere)
	EFireType FireType;

	UPROPERTY(EditAnywhere, Category="Weapon Scatter")
	bool bUseScatter = false;
protected:
	virtual void BeginPlay() override;
	virtual void OnWeaponStateSet();
	virtual void OnEquipped();
	virtual void OnDropped();
	virtual void OnEquippedSecondary();
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Trace end with scatter

	UPROPERTY(EditAnywhere, Category="Weapon Scatter")
	float DistanceToSphere = 800.f;

	UPROPERTY(EditAnywhere, Category="Weapon Scatter")
	float SphereRadius = 75.f;

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere)
	bool bUserServerSideRewind = false;

	UPROPERTY()
	ABlasterCharacter* BlasterOwnerCharacter;
	
	UPROPERTY()
	ABlasterPlayerController* BlasterOwnerController;
	
private:
	
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	USphereComponent* AreaSphere;

	UPROPERTY(ReplicatedUsing=OnRep_WeaponState, VisibleAnywhere, Category="Weapon Properties")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();


	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACasing> CasingClass;

	UPROPERTY(EditAnywhere)
	int32 Ammo;

	// The number of unprocessed server requests for Ammo. Incremented in SpendRound, Decremented in ClientUpdateAmmo.
	int32 Sequence = 0;
	
	void SpendRound();
	
	UFUNCTION(Client, Reliable)
	void ClientUpdateAmmo(int32 ServerAmmo);

	UFUNCTION(Client, Reliable)
	void ClientAddAmmo(int32 AmmoToAdd);
	
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
	
	UPROPERTY(EditAnywhere)
	int32 MagCapacity;
	
	// Zoomed FOV while aiming

	
	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;



};
