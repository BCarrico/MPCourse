// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/AmmoPickup.h"

#include "BlasterComponents/CombatComponent.h"
#include "Character/BlasterCharacter.h"

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor))
	{
		if (UCombatComponent* Combat = BlasterCharacter->GetCombatComponent())
		{
			Combat->PickupAmmo(WeaponType, AmmoAmount);
		}
	}
	Destroy();
}
