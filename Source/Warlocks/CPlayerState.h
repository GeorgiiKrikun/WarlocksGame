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

protected:
	//properites
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	bool _isDead = false;
	
	UPROPERTY(Replicated)
	int _coins = 0;


};
