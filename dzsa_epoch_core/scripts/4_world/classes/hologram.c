/*
	Epoch Mod for DayZ Standalone
	
	FileName: hologram.c

	Authors: DayZ SA Epoch Dev Team and Community Contributors
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/README.md	
	
	Licensing:
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/LICENSE.md

	
	Changelog:

	[2/19/2019] DirtySanchez
		DS1 - Add color to visual feedback during construction placement
	[NewDate] Author(s)
		Initials(Change#)-Description
*/
modded class Hologram
{
	string emt_CantBuild = "#(argb,8,8,3)color(0.81,0,0,0.75,ca)";
	string emt_CanBuild = "#(argb,8,8,3)color(0,0.81,0,0.75,ca)";
	string emt_PoweredDevice = "#(argb,8,8,3)color(1,0.65,0,0.75,ca)";

	// Change coloring of hologram | GREEN = ok to build | RED = no bueno | ORANGE = powered deployable
	override void RefreshVisual()
	{
		if ( m_Projection )
		{
			string config_material = "CfgVehicles" + " " + m_Projection.GetType() + " " + "hologramMaterial";
			string hologram_material = GetGame().ConfigGetTextOut( config_material );
			string config_model = "CfgVehicles" + " " + m_Projection.GetType() + " " + "hologramMaterialPath";
			string hologram_material_path = GetGame().ConfigGetTextOut( config_model );
			string deployable_hologram_path = hologram_material_path + "\\" + hologram_material + "_deployable.rvmat";
			string powered_hologram_path =  hologram_material_path + "\\" + hologram_material + "_powered.rvmat";
			string SEL_CORD_PLUGGED = m_Parent.GetCompEM().SEL_CORD_PLUGGED;
			string SEL_CORD_FOLDED = m_Parent.GetCompEM().SEL_CORD_FOLDED;
			string selection_to_refresh;
			
			const string emt_HGDeployablePath = hologram_material_path + "\\" + hologram_material + "_deployable.rvmat";
			if ( !IsColliding() && !IsFloating() )
			{
				if ( m_Parent.HasEnergyManager() )
				{
					if ( m_Parent.GetCompEM().IsPlugged() )
					{
						if ( m_Parent.GetCompEM().IsEnergySourceAtReach( GetProjectionPosition() ) )
						{
							for( int i = 0; i < m_SelectionsToRefresh.Count(); i++ )
							{
								selection_to_refresh = m_SelectionsToRefresh.Get(i);
								m_Projection.SetObjectTexture( GetHiddenSelection( selection_to_refresh ), emt_CanBuild );
								m_Projection.SetObjectMaterial( GetHiddenSelection( selection_to_refresh ), powered_hologram_path );
							}
							
							m_Projection.SetAnimationPhase( SEL_CORD_PLUGGED, 0);
							m_Projection.SetAnimationPhase( SEL_CORD_FOLDED, 1);	
						}							
						else
						{
							for( int j = 0; j < m_SelectionsToRefresh.Count(); j++ )
							{
								selection_to_refresh = m_SelectionsToRefresh.Get(j);
								m_Projection.SetObjectTexture( GetHiddenSelection( selection_to_refresh ), emt_PoweredDevice );
								m_Projection.SetObjectMaterial( GetHiddenSelection( selection_to_refresh ), deployable_hologram_path );
							}
							
							m_Projection.SetAnimationPhase( SEL_CORD_PLUGGED, 1);
							m_Projection.SetAnimationPhase( SEL_CORD_FOLDED, 0);
						}
					}
					else
					{
						for( int k = 0; k < m_SelectionsToRefresh.Count(); k++ )
						{
							selection_to_refresh = m_SelectionsToRefresh.Get(k);
							m_Projection.SetObjectTexture( GetHiddenSelection( selection_to_refresh ), emt_CanBuild );
							m_Projection.SetObjectMaterial( GetHiddenSelection( selection_to_refresh ), deployable_hologram_path );
						}
							m_Projection.SetAnimationPhase( SEL_CORD_PLUGGED, 1);
							m_Projection.SetAnimationPhase( SEL_CORD_FOLDED, 0);
					}			
				}
				else
				{
					for( int l = 0; l < m_SelectionsToRefresh.Count(); l++ )
					{
						selection_to_refresh = m_SelectionsToRefresh.Get(l);
						m_Projection.SetObjectTexture( GetHiddenSelection( selection_to_refresh ), emt_CantBuild );
						m_Projection.SetObjectMaterial( GetHiddenSelection( selection_to_refresh ), deployable_hologram_path );
					}
				}
			}
			else
			{
				for( int m = 0; m < m_SelectionsToRefresh.Count(); m++ )
				{
					selection_to_refresh = m_SelectionsToRefresh.Get(m);
					m_Projection.SetObjectTexture( GetHiddenSelection( selection_to_refresh ), emt_CantBuild );
					m_Projection.SetObjectMaterial( GetHiddenSelection( selection_to_refresh ), deployable_hologram_path );
				}
			}
		}
	}
}
