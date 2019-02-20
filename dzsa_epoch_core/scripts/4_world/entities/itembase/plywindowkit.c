class plywindowKit extends FenceKit
{		
	//================================================================
	// ADVANCED PLACEMENT
	//================================================================			
		
	override void OnPlacementComplete( Man player )
	{
		if ( GetGame().IsServer() )
		{
			//Create object
			PlayerBase player_base = PlayerBase.Cast( player );
			vector position = player_base.GetLocalProjectionPosition();
			vector orientation = player_base.GetLocalProjectionOrientation();
			
			plywindow obj = plywindow.Cast( GetGame().CreateObject( "plywindow", GetPosition() ) );
			obj.SetPosition( position );
			obj.SetOrientation( orientation );
			
			//make the kit invisible, so it can be destroyed from deploy UA when action ends
			HideAllSelections();
		}	
		
		SetIsDeploySound( true );
	}
	

}
