// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CWarlockChar.generated.h"

class UParticleSystemComponent;

UCLASS()
class WARLOCKS_API ACWarlockChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACWarlockChar();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;


	float HealthPoints() const;
	float MaxHealthPoints() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	//properites
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _HealthPoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _MaxHealthPoints;



	float InternalTakeRadialDamage(float Damage, struct FRadialDamageEvent const& RadialDamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	float InternalTakePointDamage(float Damage, struct FPointDamageEvent const& PointDamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	// TELEPORT STUFF

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void playTeleportAnimation(float time);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UParticleSystemComponent* _teleportAnimation;

};
