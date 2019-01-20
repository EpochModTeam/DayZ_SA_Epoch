/*
	Epoch Mod for DayZ Standalone
	
	FileName: missionGameplay.c

	Authors: DayZ SA Epoch Dev Team and Community Contributors
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/README.md	
	
	Licensing:
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/LICENSE.md

	Changelog:

	[1/16/2019] DirtySanchez
		DS8 - Port CCCP Stamina Bar coloring
			- Stamina Bar Coloring
			- Green 60-100 | Yellow 40-60 | Orange 20-40 | Red 0-20
			- Code for increased Stamina above 100(energy drinks, morphine, etc)
	[NewDate] Author(s)
		Initials(Change#)-Description
*/

/*		code							colors
ARGB( alpha * 255, 255, 255, 255 ) );	//white
ARGB( alpha * 255, 255, 255, 0 ) );		//yellow
ARGB( alpha * 255, 255, 0, 0 ) );		//red
ARGB( alpha * 255, 255, 165, 0 ) );		//orange
ARGB( alpha * 255, 0, 206, 209 ) );		//turquoise
ARGB( alpha * 255, 0, 206, 0 ) );		//green
ARGB( alpha * 255, 30, 144, 255 ) );	//blue
ARGB( alpha * 255, 255, 255, 255 ) );	//white
*/

modded class IngameHud extends Hud
{
	
	override void SetStamina( int value , int range )
	{
		bool debugStaminaHud = false;
		int newValue;
		float sx, sy;
		float max = 100;
		range = max;
		
		if (value > 100)
			newValue = value - 100;
		else
			newValue = value;

		float percentage =  range / max;
		m_Stamina.SetCurrent( ( newValue / range ) * max );
		m_Stamina.GetSize( sx, sy );
		m_Stamina.SetSize( percentage, sy );
		m_StaminaBackground.SetSize( 1.2, sy);
		
		// If stamina is over 100, then overlay is blue and background is green(for the original 100 stamina)
		if (value > 100)
		{
			m_StaminaBackground.SetColor( ARGB(255, 0, 255, 0) );
			m_Stamina.SetColor( ARGB( 255, 0, 0, 255 ) );
		}
		else
		{
			m_StaminaBackground.SetColor( ARGB(255, 0, 0, 0) );
			if((value <= 100) && (value >= 60))
				m_Stamina.SetColor( ARGB( 255, 0, 255, 0 ) );
			if((value < 60) && (value >= 40)) 
				m_Stamina.SetColor( ARGB( 255, 255, 255, 0 ) );
			if((value < 40) && (value >= 20)) 
				m_Stamina.SetColor( ARGB( 255, 255, 165, 0 ) );
			if((value < 20) && (value >= 0)) 
				m_Stamina.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
		
		if(debugStaminaHud) PrintFormat("[CCCPDebug]IngameHud.c SetStamina Range:%1 Value:%2 Percent:%3 max:%4 sx:%5 sy:%6", range, value, percentage, max, sx, sy);

	}
}