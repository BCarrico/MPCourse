// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BlasterGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Character/BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/BlasterPlayerState.h"

ABlasterGameMode::ABlasterGameMode()
{
	bDelayedStart = true; // Stay in waiting to start state, default pawn spawned and can fly around
}

void ABlasterGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmUpTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
}

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* EliminatedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	ABlasterPlayerState* VictimPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState)
	{
		AttackerPlayerState->AddToScore(1.0f);
	}
	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}
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

void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}
