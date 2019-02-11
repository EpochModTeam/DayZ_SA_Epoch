modded class ActionConstructor
{
	override void ConstructActions(out array<ref ActionBase> actions, out TIntArray suactions, out TIntArray cactions, out TIntArray iactions)
	{
		super.ConstructActions(actions, suactions, cactions, iactions);
		
		// Custom Actions	
		// actions.Insert(new ActionCustom);
	}
};