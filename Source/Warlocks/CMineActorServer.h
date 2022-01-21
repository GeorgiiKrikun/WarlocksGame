// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorSpawnedBySkill.h"
#include "Templates/SubclassOf.h"
#include "CMineActorServer.generated.h"

class USphereComponent;
class UCMine;

UCLASS()
class WARLOCKS_API ACMineActorServer : public AActorSpawnedBySkill
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACMineActorServer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void whenOverlapped(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void whenNotOverlapped(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool _visibleOnThisInstance = false;

	UFUNCTION(BlueprintImplementableEvent)
	void armVisibilityTimer();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* _collisionSphere = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _damage = 20.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _suckTime = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _suckFallOff = 200.0f;
	
	float _currentSuckTime = -1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _force = 200.0f;

	TArray<AActor*> _actorsCaught;

};
