// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//TEXT("Text, %d %f %s"), intVar, floatVar, *fstringVar)

DECLARE_LOG_CATEGORY_EXTERN(Goglike, Log, All);
#define GW(X, ...) UE_LOG(Goglike, Warning, TEXT(X), __VA_ARGS__);
#define GL(X, ...) UE_LOG(Goglike, Log, TEXT(X), __VA_ARGS__);
#define GF(X, ...) UE_LOG(Goglike, Fatal, TEXT(X), __VA_ARGS__);
#define GE(X, ...) UE_LOG(Goglike, Error, TEXT(X), __VA_ARGS__);
