// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticlesBase;

	UPROPERTY(EditAnywhere)
	USoundCue* ImpactSound;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticlesEnemy;
	
	bool bIsEnemy;
private:
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystem* Tracer;

	UPROPERTY()
	UParticleSystemComponent* TracerComponent;



};
