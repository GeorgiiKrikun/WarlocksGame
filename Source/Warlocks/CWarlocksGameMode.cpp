// Fill out your copyright notice in the Description page of Project Settings.

#include "CWarlocksGameMode.h"
#include "CGameStateBase.h"
#include "CPlayerState.h"
#include "GoglikeLogging.h"
#include "CWarlockChar.h"
#include "CWarlockMainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"


void ACWarlocksGameMode::StartPlay() {
	Super::StartPlay();
}

void ACWarlocksGameMode::addBot()
{
	/*if (_botControllerClass) {
		GE("No bot controller class to spawn, aborting");
		return;
	} 
	if (!_actualClassToSpawn) {
		GE("No character class to spawn, aborting");
		return;
	}*/
	
	FVector spawnLocation;
	FRotator spawnRotation;
	ACWarlockChar* spawnedChar = Cast<ACWarlockChar>(GetWorld()->SpawnActor(_actualClassToSpawn, &spawnLocation, &spawnRotation));
	ACAIController* spawnedController = Cast<ACAIController>(GetWorld()->SpawnActor(_botControllerClass, &spawnLocation, &spawnRotation));
	
	if (!spawnedChar) {
		GE("Could not spawn char");
		return;
	}
	if (!spawnedController) {
		GE("Could not spawn controller");
		return;
	}

	spawnedController->Possess(spawnedChar);

}

void ACWarlocksGameMode::addDamageStatisticsEntry(AController* from, float damage)
{
	GW("ADD DAMAGE STATISTICS ENTRY %d, %f", from, damage);
	if (!_damageStatistics._damageDoneMap.Contains(from)) {
		GE("Cannot add damage statistic entry, because controller was not registered in game mode");
		return;     
	}               
	_damageStatistics._damageDoneMap[from] += damage;
}

void ACWarlocksGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (_currentLengthOfInterlude >= 0.0f && ((_currentLengthOfInterlude - DeltaSeconds) <= 0.0f)) {
		GW("Exiting Interlude");
		ReactOnExitInterlude();
	}

	if (_currentTreshold > _minThreshold) {
		_currentTreshold -= _thresholdVelocityDecrease * DeltaSeconds;
	}

	_currentLengthOfInterlude -= DeltaSeconds;

	ACGameStateBase* gamestate = Cast<ACGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if (!gamestate) {
		GE("Could not find gamestate");
		return;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ACWarlockMainPlayerController* PlayerController = Cast<ACWarlockMainPlayerController>(Iterator->Get());
		APawn* pawn = Cast<APawn>(PlayerController->GetPawn());
		FVector Location2D = pawn->GetActorLocation();
		Location2D.Z = 0;

		float distance = FVector::Dist(Location2D, FVector(0, 0, 0));
		if (pawn && (distance > _currentTreshold)) pawn->TakeDamage(10.0f*DeltaSeconds, FPointDamageEvent(), nullptr, nullptr);
	}

	gamestate->_thresholdValue = _currentTreshold;
	

	
}


void ACWarlocksGameMode::BeginPlay()
{
	Super::BeginPlay();
	_currentTreshold = _beginThreshold;

}

ACWarlocksGameMode::ACWarlocksGameMode() :
	_actualClassToSpawn(ACWarlockChar::StaticClass()),
	_botControllerClass(ACAIController::StaticClass())
{

}

void ACWarlocksGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	ACWarlockMainPlayerController* myController = Cast<ACWarlockMainPlayerController>(NewPlayer);
	if (!myController) {
		GE("Login failed, controller is empty");
		return;
	}

	myController->OnPawnDeath.AddDynamic(this, &ACWarlocksGameMode::ReactOnDeath);
	if (!_damageStatistics._damageDoneMap.Contains(myController)) {
		_damageStatistics._damageDoneMap.Add(myController, 0.0f);
	}
}

void ACWarlocksGameMode::ReactOnDeath()
{

	if (_currentLengthOfInterlude <= 0.0f) { // we are in the battle
		ReactOnDeathBattle();
	}
	else { // interlude, immediate respawn
		ReactOnDeathInterlude();
	}

}

void ACWarlocksGameMode::ReactOnDeathBattle()
{
	if (_bRespawnGuard) return;
    int numberPlayersLeftAlive = CheckNumberOfPlayersAlive();
    
    GL("Number of players still alive = %d", numberPlayersLeftAlive);
    
	// respawn everyone
    if (numberPlayersLeftAlive > 1) return;
	ReactOnEnterInterlude();

}

void ACWarlocksGameMode::ReactOnDeathInterlude()
{
	if (_bRespawnGuard) return;
	_bRespawnGuard = true;

	GL("Reacting on death in interlude")
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ACWarlockMainPlayerController* PlayerController = Cast<ACWarlockMainPlayerController>(Iterator->Get());
		RespawnPlayer(PlayerController);
	}
	_bRespawnGuard = false;
}


void ACWarlocksGameMode::ReactOnEnterInterlude()
{

	_currentTreshold = _beginThreshold;

	_bRespawnGuard = true;
	GW("This game has ended");
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ACWarlockMainPlayerController* PlayerController = Cast<ACWarlockMainPlayerController>(Iterator->Get());
		RespawnPlayer(PlayerController);
	}
	_bRespawnGuard = false;

	// begin interlude
	_currentLengthOfInterlude = _lengthOfInterlude;
	AwardCoinsAccordingToStats();
	FString damageStats = _damageStatistics.pop();

	GW("damage stats %s", *damageStats);

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ACWarlockMainPlayerController* PlayerController = Cast<ACWarlockMainPlayerController>(Iterator->Get());
		PlayerController->callOnInterludeBegin(damageStats);
	}
}

void ACWarlocksGameMode::ReactOnExitInterlude()
{
	_currentTreshold = _beginThreshold;

	
	if (_bRespawnGuard) return;
	_bRespawnGuard = true;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ACWarlockMainPlayerController* PlayerController = Cast<ACWarlockMainPlayerController>(Iterator->Get());
		AActor* playerStart = FindPlayerStart(PlayerController);

		if (playerStart) RespawnPlayer(PlayerController, false, true, playerStart->GetActorLocation(), playerStart->GetActorRotation());
		else RespawnPlayer(PlayerController);

		PlayerController->callOnInterludeEnd();
	}
	_bRespawnGuard = false;
}

void ACWarlocksGameMode::AwardCoinsAccordingToStats()
{
	for (const TPair<AController*, float>& pair : _damageStatistics._damageDoneMap)
	{
		ACPlayerState* playerState = pair.Key->GetPlayerState<ACPlayerState>();
		if (!playerState) continue;
		playerState->SetCoins(playerState->Coins() + int(pair.Value / _DamageForCoin));
	}
}

int ACWarlocksGameMode::CheckNumberOfPlayersAlive()
{
	int numberPlayersLeftAlive = 0;
	UWorld* World = GetWorld();
	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ACWarlockMainPlayerController* PlayerController = Cast<ACWarlockMainPlayerController>(Iterator->Get());
		if (PlayerController) {
			ACWarlockChar* pawn = PlayerController->GetPawn<ACWarlockChar>();
			if (pawn && !pawn->Dead()) numberPlayersLeftAlive++;
		}
	}
	return numberPlayersLeftAlive;
}

void ACWarlocksGameMode::RespawnPlayer(ACWarlockMainPlayerController* controller, bool respawnEvenIfAlive /*= true*/, bool atSpecifiedLocation /*= false*/, FVector location /*= FVector(0,0,0)*/, FRotator rotation/* = FRotator(0, 0, 0)*/)
{

	GL("RESPAWN CALLED")
	if (!controller) return;
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this,controller, atSpecifiedLocation, location, rotation]() {
		ACWarlockChar* character = controller->GetPawn<ACWarlockChar>();
		if (character) character->SetDead(false);
		else {GE("Could not respawn character, which is empty!");}

		if (atSpecifiedLocation) {
			if (character) character->TeleportTo(location, rotation);
		}

	}, _respawnDelay, false);
}

bool ACWarlocksGameMode::AreWeInMatch()
{
	
	return (_currentLengthOfInterlude <= 0.0f);
}

FString FDamageStatistics::pop()
{
	//FString res = "Statistics:\n\n";
	//for (const TPair<APlayerController*, float>& pair : _damageDoneMap)
	//{
	//	res += "ID: ";
	//	res.AppendInt(pair.Key->PlayerState->GetPlayerId());

	//	//res += " NAME: ";
	//	//res += pair.Key->PlayerState->GetPlayerName();
	//	res += " DAMAGE DONE: ";
	//	res.AppendInt(int( pair.Value));
	//	res += " Coins: ";
	//	res.AppendInt(pair.Key->GetPlayerState<ACPlayerState>()->Coins());
	//	res += "\n";

	//	_damageDoneMap[pair.Key] = 0.0f;
	//}

	//return res;
	return FString();

}
