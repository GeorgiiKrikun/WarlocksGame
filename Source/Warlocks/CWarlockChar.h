// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/EngineTypes.h"
#include "CWarlockChar.generated.h"

class UParticleSystemComponent;
class ACWarlockMainPlayerController;
class UCSkillBase;



UCLASS()
class WARLOCKS_API ACWarlockChar : public ACharacter
{
	GENERATED_BODY()


	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRestart);

public:
	// Sets default values for this character's properties
	ACWarlockChar();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;


	float HealthPoints() const;
	float MaxHealthPoints() const;

	FOnDeath onDeathDelegate;
	FOnRestart onRestartDelegate;



	void Restart() override;

	bool Dead() const { return _isDead; }

	UFUNCTION(Server,Reliable)
	void SetDead(bool val);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move_XAxis(float AxisValue);

	void Move_YAxis(float AxisValue);


	// casting stuff

private:

	UPROPERTY(EditAnywhere)
	UCSkillBase* _skillBeingCasted;

public:
	
	void setCasting(UCSkillBase* skill);


	UFUNCTION(BlueprintCallable)
	UCSkillBase* getSkillBeingCasted();

	//__END CASTING

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	//properites
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _HealthPoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _MaxHealthPoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, ReplicatedUsing = OnRep_Dead)
	bool _isDead = false;

	UFUNCTION()
	void OnRep_Dead();

	float InternalTakeRadialDamage(float Damage, struct FRadialDamageEvent const& RadialDamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	float InternalTakePointDamage(float Damage, struct FPointDamageEvent const& PointDamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// To track who dealt damage when there is no correct instigator
	ACWarlockMainPlayerController* _previousDamageInstigator;









public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void stopMovementFor(float seconds);

	UFUNCTION(Server, Reliable)
	void orientDirectionTowards(FVector direction, float seconds);

	// TELEPORT STUFF

	//UFUNCTION(Server, Reliable, BlueprintCallable)
	//void playTeleportAnimation(float time);
	//
	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
	//UParticleSystemComponent* _teleportAnimation;

	//SACRRIFICE
	UFUNCTION(Server,Reliable, BlueprintCallable)
	void startPlaySacrificeAnimation();

	UFUNCTION(NetMulticast, Reliable)
	void playSacrificeAnimation();

	UFUNCTION(BlueprintNativeEvent)
	void ClientImplementationOfSacrificeAnimation();


	//Fireball

	UFUNCTION(NetMulticast, Reliable)
	void playFireballAnimation();

	UFUNCTION(BlueprintNativeEvent)
	void ClientImplementationOfFireballAnimation();



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* _sacrificeAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _sacrificeAnimationPlayTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCollisionResponseContainer _collisionDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCollisionResponseContainer _collisionDead;

	//Parry stuff
	protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool _invulnerable;

	bool _acceptsMovementInput=true;

	public:

	UFUNCTION(Server, Reliable)
	void setInvulnerable(bool invulnerable);

	bool GetInvulnerable();
};
