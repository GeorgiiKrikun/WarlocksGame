// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorSpawnedBySkill.h"
#include "CFireballActorServer.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UCFireball;

UCLASS()
class WARLOCKS_API ACFireballActorServer : public AActorSpawnedBySkill
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACFireballActorServer();

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

	UMovementComponent* ProjectileMovement() const override;


protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UProjectileMovementComponent* _movement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* _collisionSphere;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _damage = 20.0f;

};
