modded class ActionCloseBarrel: ActionInteractBase
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		if ( target_object.IsItemBase() )
		{
			Barrel_ColorBase ntarget = Barrel_ColorBase.Cast( target_object );
			if( ntarget )
			{
				if ( !ntarget.IsLocked() && ntarget.IsOpen() )
				{
					return true;
				}
			}
		}
		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		Object target_object = action_data.m_Target.GetObject();
		Barrel_ColorBase ntarget = Barrel_ColorBase.Cast( target_object );
		
		if( ntarget )
		{
			ntarget.DetermineAction(action_data.m_Player);
		}
	}
	
	override void OnEndServer( ActionData action_data )
	{
		Object target_object = action_data.m_Target.GetObject();
		Barrel_ColorBase ntarget = Barrel_ColorBase.Cast( target_object );
		if( ntarget )
		{
			ntarget.SoundSynchRemoteReset();
		}
	}
}