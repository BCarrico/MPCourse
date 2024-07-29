// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Casing.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>("CasingMesh");
	SetRootComponent(CasingMesh);
	CasingMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();

	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);
	CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
	const FRotator RandomRotation = FRotator(FMath::RandRange(0.f, 360.f),FMath::RandRange(0.f, 360.f),FMath::RandRange(0.f, 360.f));
	CasingMesh->AddRelativeRotation(RandomRotation);
	GetWorld()->GetTimerManager().SetTimer(ShellDestroyTimerHandle, FTimerDelegate::CreateUObject(this, &ACasing::DestroyShell), ShellDestroyDelayTime, false);
}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit)
{
	if (ShellSound && !bShellSoundPlayed)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
		bShellSoundPlayed = true;
	}

}

void ACasing::DestroyShell()
{
	Destroy();
}


