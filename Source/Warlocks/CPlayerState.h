// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

#include "CPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class WARLOCKS_API ACPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangeDeath, int, ID);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	void setDead(bool dead);

	FOnPlayerStateChangeDeath Del;

	UFUNCTION(BlueprintCallable)
	int Coins() const;

	void SetCoins(int val);

	UFUNCTION(BlueprintCallable)
		float DamageDone() const;
	
	UFUNCTION(BlueprintCallable)
		void SetDamageDone(float val);

	UFUNCTION(BlueprintCallable)
		float DamageReceived() const;
	
	UFUNCTION(BlueprintCallable)
		void SetDamageReceived(float val);

	UFUNCTION(BlueprintCallable)
		int Kills() const;
	
	UFUNCTION(BlueprintCallable)
		void SetKills(int val);

	UFUNCTION(BlueprintCallable)
		int Deaths() const;
	
	UFUNCTION(BlueprintCallable)
		void SetDeaths(int val);
	
	UFUNCTION(BlueprintCallable)
	int Diamonds() const { return _diamonds; }

	void SetDiamonds(int val) { _diamonds = val; }
protected:
	//properties
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	bool _isDead = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	int _coins = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	int _diamonds = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _damageDone = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _damageReceived = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	int _kills = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	int _deaths = 0;



};
