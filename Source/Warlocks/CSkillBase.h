// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSkillBase.generated.h"


UCLASS(Blueprintable, BlueprintType)
class WARLOCKS_API UCSkillBase : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCSkillBase();

	/// Called from blueprint on client machine to start cooldown PREPARE PHASE
	UFUNCTION(BlueprintCallable)
		void BeginOnPrepare();

	UFUNCTION(server, reliable)
		virtual void onPrepareServer();

	UFUNCTION(client, reliable)
		void prepareClient();

	UFUNCTION(BlueprintNativeEvent)
		void onPrepareClient();

	/// <summary>
	///  END PREPARE PHASE	
	/// </summary>
	UFUNCTION(BlueprintCallable)
		void BeginOnEndPrepare();

	UFUNCTION(server, reliable)
		virtual void onEndPrepareServer();

	UFUNCTION(client, reliable)
		void endPrepareClient();

	UFUNCTION(BlueprintNativeEvent)
		void onEndPrepareClient();


/// <summary>
///  BEGIN CAST
/// </summary>
	UFUNCTION(BlueprintCallable)
	void BeginBeginCast();

	UFUNCTION(server, reliable)
	virtual void onBeginCastServer();

	UFUNCTION(client, reliable)
	void BeginCastClient();

	UFUNCTION(BlueprintNativeEvent)
	void onBeginCastClient();

/// <summary>
///  AFTER CAST
/// </summary>
	UFUNCTION(BlueprintCallable)
	void BeginAfterCast();

	UFUNCTION(server, reliable)
		virtual void onAfterCastServer();

	UFUNCTION(client, reliable)
		void AfterCastClient();

	UFUNCTION(BlueprintNativeEvent)
		void onAfterCastClient();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(server, unreliable, BlueprintCallable)
		void writeToLogServer();


	UFUNCTION(client, unreliable, BlueprintCallable)
		void writeToLogClient();

	//properites
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		float _cooldown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		float _currentCooldown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		bool _isPrepared;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};