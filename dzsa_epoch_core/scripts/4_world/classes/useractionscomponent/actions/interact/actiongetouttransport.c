/*
	Epoch Mod for DayZ Standalone
	
	FileName: actiongetouttransport.c

	Authors: DayZ SA Epoch Dev Team and Community Contributors
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/README.md	
	
	Licensing:
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/LICENSE.md

	Changelog:

	[1/30/2019] AWOL
		AWOL1 - Added raycast to prevent vehicle based wall glitch.
		
*/

modded class ActionGetOutTransport: ActionInteractBase
{
	private Transport m_emt_transport;
	private int       m_emt_crewIdx;
	
	private const float c_emt_RayDistance = 2.0;
	 
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		vector       emt_CrewGetInPos;
		vector       emt_CrewTransform[4];
		vector       emt_RayStart;
		vector 		 emt_RayEnd;
		vector       emt_Dir;
		
		vector position;
		vector contact_dir;
		int contact_component;
		set<Object> results = new set<Object>;
		
		m_emt_transport = null;

		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
		if ( vehCommand )
		{
			m_emt_transport = vehCommand.GetTransport();
			if ( m_emt_transport )
			{
				m_emt_crewIdx = m_emt_transport.CrewMemberIndex( player );
				if ( m_emt_crewIdx >= 0 && m_emt_transport.CrewCanGetThrough( m_emt_crewIdx ) ) {
				
					m_emt_transport.CrewEntryWS( m_emt_crewIdx, emt_CrewGetInPos, emt_Dir );
					m_emt_transport.CrewTransformWS( m_emt_crewIdx, emt_CrewTransform );
					
					emt_RayStart = Vector(0.0, 0.0, 0.0).Multiply4(emt_CrewTransform);
					emt_Dir = vector.Direction( emt_RayStart, emt_CrewGetInPos );
					emt_Dir.Normalize();
					
					emt_RayEnd = emt_RayStart + emt_Dir * c_emt_RayDistance;
					emt_RayEnd[1] = GetGame().SurfaceY(emt_RayEnd[0], emt_RayEnd[2]);
					emt_RayEnd[1] = emt_RayEnd[1] + 0.5;
					
					if (DayZPhysics.RaycastRV(emt_RayStart, emt_RayEnd, position, contact_dir, contact_component, results, m_emt_transport, null, false, false, ObjIntersectGeom)) {
						if (results.Count() > 0) {
							return false;
						}
					}
					return true;
				}
			}
		}
		return false;
	}
};
