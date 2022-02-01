// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class WarlocksTarget : TargetRules
{
	public WarlocksTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Warlocks" } );
		
		// Ensure that we compile with the Steam OSS
		//UEBuildConfiguration.bCompileSteamOSS = true;
	}
}
