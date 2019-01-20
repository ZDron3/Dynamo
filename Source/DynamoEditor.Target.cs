// ‎Copyright© 2016 UE4Arch.com

using UnrealBuildTool;
using System.Collections.Generic;

public class DynamoEditorTarget : TargetRules
{
	public DynamoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Dynamo" } );
	}
}
