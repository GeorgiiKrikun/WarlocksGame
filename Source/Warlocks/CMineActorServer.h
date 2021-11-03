// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMineActorServer.generated.h"

class USphereComponent;

UCLASS()
class WARLOCKS_API ACMineActorServer : public AActor
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

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* _collisionSphere;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _damage = 20.0f;
};
