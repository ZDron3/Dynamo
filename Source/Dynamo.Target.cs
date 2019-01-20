// ‎Copyright© 2016 UE4Arch.com

using UnrealBuildTool;
using System.Collections.Generic;

public class DynamoTarget : TargetRules
{
	public DynamoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "Dynamo" } );
	}
}
