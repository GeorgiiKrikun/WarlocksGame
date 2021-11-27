// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTileSpawner.generated.h"

UCLASS()
class WARLOCKS_API ACTileSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACTileSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<USceneComponent> _actorToSpawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _horizontalSize;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _verticalSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int _tilesNumberHorizontal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int _tilesNumberVertical;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _zOffset;

};
