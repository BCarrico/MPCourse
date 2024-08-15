// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BlasterGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Character/BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* EliminatedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	if (EliminatedCharacter)
	{
		EliminatedCharacter->Elim();
	}
}

void ABlasterGameMode::RequestRespawn(ACharacter* ElimintedCharacter, AController* EliminatedController)
{
	if (ElimintedCharacter)
	{
		ElimintedCharacter->Reset(); // Stops possessing character
		ElimintedCharacter->Destroy();
	}
	if (EliminatedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num()-1);
		RestartPlayerAtPlayerStart(EliminatedController, PlayerStarts[Selection]);
	}
}
