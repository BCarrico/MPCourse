// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class ACasing;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
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
	void ShowPickupWidget(bool bShowWidget);
	virtual void Fire(const FVector& HitTarget);
	void SetWeaponState(EWeaponState State);
	void Dropped();
	FORCEINLINE USphereComponent* GetAreaSphere() const {return AreaSphere;}
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const {return WeaponMesh;}
	FORCEINLINE float GetZoomedFOV() const{return ZoomedFOV;}
	FORCEINLINE float GetZoomInterpSpeed() const{return ZoomInterpSpeed;}
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
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
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

	
	// Zoomed FOV while aiming

	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;

};
