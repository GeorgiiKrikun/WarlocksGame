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
		void BeginPREPARE();

	UFUNCTION(server, reliable)
		virtual void onPREPAREServer();

	UFUNCTION(client, reliable)
		void PREPAREClient();

	UFUNCTION(BlueprintNativeEvent)
		void onPREPAREClient();

	/// <summary>
	///  END PREPARE PHASE	
	/// </summary>
	UFUNCTION(BlueprintCallable)
		void BeginENDPREPARE();

	UFUNCTION(server, reliable)
		virtual void onENDPREPAREServer();

	UFUNCTION(client, reliable)
		void ENDPREPAREClient();

	UFUNCTION(BlueprintNativeEvent)
		void onENDPREPAREClient();


/// <summary>
///  BEGIN CAST
/// </summary>
	UFUNCTION(BlueprintCallable)
	void BeginBEGINCAST();

	UFUNCTION(server, reliable)
	virtual void onBEGINCASTServer();

	UFUNCTION(client, reliable)
	void BEGINCASTClient();

	UFUNCTION(BlueprintNativeEvent)
	void onBEGINCASTClient();

/// <summary>
///  AFTER CAST
/// </summary>
	UFUNCTION(BlueprintCallable)
	void BeginAFTERCAST();

	UFUNCTION(server, reliable)
		virtual void onAFTERCASTServer();

	UFUNCTION(client, reliable)
		void AFTERCASTClient();

	UFUNCTION(BlueprintNativeEvent)
		void onAFTERCASTClient();

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		bool _isPreparing;




public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};