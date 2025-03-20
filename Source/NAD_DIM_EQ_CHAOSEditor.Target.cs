// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class NAD_DIM_EQ_CHAOSEditorTarget : TargetRules
{
	public NAD_DIM_EQ_CHAOSEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "NAD_DIM_EQ_CHAOS" } );
	}
}
