// Fill out your copyright notice in the Description page of Project Settings.


#include "CSessionsSearchDataObject.h"

FString UCSessionsSearchDataObject::GetSessionName()
{
	return FString("SessionName");
}

int UCSessionsSearchDataObject::GetNumPlayers()
{
	return 4;
}

int UCSessionsSearchDataObject::GetNumMaxPlayers()
{
	return 9;
}
