// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Player/BlasterPlayerController.h"
#include "BlasterGameMode.generated.h"

class ABlasterPlayerState;
class ABlasterPlayerController;
class ABlasterCharacter;

namespace MatchState
{
	extern BLASTER_API const FName Cooldown; // Match duration has been reached, display winner and begin cooldown timer
}
/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABlasterGameMode();
	virtual void Tick(float DeltaSeconds) override;
	virtual void PlayerEliminated(ABlasterCharacter* EliminatedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* ElimintedCharacter, AController* EliminatedController);
	void PlayerLeftGame(ABlasterPlayerState* PlayerLeaving);
	FORCEINLINE float GetLevelStartingTime() const {return LevelStartingTime;}
	FORCEINLINE float GetCountdownTime() const {return CountdownTime;}
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);
	UPROPERTY(EditDefaultsOnly)
	float WarmUpTime = 10.f;
	
	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;
	
	float LevelStartingTime = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.f;
protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;
private:
	float CountdownTime = 0.f;

};
