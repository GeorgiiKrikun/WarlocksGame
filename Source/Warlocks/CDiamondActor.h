// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SphereComponent.h>
#include "CDiamondActor.generated.h"

UCLASS()
class WARLOCKS_API ACDiamondActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACDiamondActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void whenOverlapped(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* _collisionSphere;
};
