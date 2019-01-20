/*
	Epoch Mod for DayZ Standalone
	
	FileName: missionGameplay.c

	Authors: DayZ SA Epoch Dev Team and Community Contributors
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/README.md	
	
	Licensing:
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/LICENSE.md

	Changelog:

	[1/14/2019] DirtySanchez
		DS5 - Epoch Logo during experimental development
		DS6	- Logo Toggle HotKey(Temp set to U) / Toggle logo upon hitting Escape. 
			- BUG: Escape Menu Continue click does not toggle it back
			- TODO: Settings[WIP]
	[1/17/2019] DirtySanchez
		DS9 - Stamina Bar width increase to match action panel width
	[NewDate] Author(s)
		Initials(Change#)-Description
*/

modded class MissionGameplay
{
	// DS5 Epoch Logo on screen
	Widget 					dze_ServerLogoWidget; 
	ImageWidget 			dze_Logo;
	override void OnMissionStart()
	{
		// Call super.Method(); for compatibility with original dz code and other mods
		super.OnMissionStart();

		// Init new logo and toggle it on
		dze_ServerLogoWidget = GetGame().GetWorkspace().CreateWidgets("emt/dzsa_epoch_assets/DS/UI/Logos/EpochLogo.layout");
		dze_Logo = ImageWidget.Cast(dze_ServerLogoWidget.FindAnyWidget("logo") );
		dze_Logo.LoadImageFile(0, "emt/dzsa_epoch_assets/DS/UI/Logos/EpochLogo.paa" );
		LogoToggle();
	}// End DS5
	
	// DS6 Epoch Logo Toggle method and hotkey
	bool toggle = false;
	void LogoToggle()
	{
		if(toggle)
			toggle = false;
		else
			toggle = true;
		
		dze_ServerLogoWidget.Show(toggle);
	}
	
	override void OnKeyPress(int key)
	{
		// Call super.Method(); for compatibility with original dz code and other mods
		super.OnKeyPress(key);
		// Temporary use of U for toggle
		if ( key == KeyCode.KC_U )
			LogoToggle();
		
		//TODO : Toggle back when clicking continue
		if ( key == KeyCode.KC_ESCAPE )
			//TODO : Block toggle if Logo is already off
			//LogoToggle();
	}
	// End DS6
	
	// DS9 Stamina Bar length changes
	ref WidgetCache 		cccp_WidgetsCache;
	override void OnInit()
	{
		if ( m_Initialized )
		{
			return;
		}
			
		PPEffects.Init();
		
		m_UIManager = GetGame().GetUIManager();
			
		g_Game.m_IsPlayerSpawning	= true;
		m_Initialized				= true;

		// init hud ui
		if ( !m_HudRootWidget )
		{
			// DS9 override current layout file for modded
			m_HudRootWidget			= GetGame().GetWorkspace().CreateWidgets("emt/dzsa_epoch_assets/DS/UI/Hud/day_z_hud.layout");
			
			m_HudRootWidget.Show(false);
			
			m_Chat.Init(m_HudRootWidget.FindAnyWidget("ChatFrameWidget"));
			
			m_ActionMenu.Init( m_HudRootWidget.FindAnyWidget("ActionsPanel"), TextWidget.Cast( m_HudRootWidget.FindAnyWidget("DefaultActionWidget") ) );
			
			m_Hud.Init( m_HudRootWidget.FindAnyWidget("HudPanel") );
			
			m_MicrophoneIcon		= ImageWidget.Cast( m_HudRootWidget.FindAnyWidget("mic") );

			// notification window
			m_NotificationWidget	= new NotificationMessage( m_HudRootWidget );
			
			// chat channel
			m_ChatChannelArea		= m_HudRootWidget.FindAnyWidget("ChatChannelPanel");
			m_ChatChannelText		= TextWidget.Cast( m_HudRootWidget.FindAnyWidget("ChatChannelText") );
		}
		
		// init hud ui
		if ( GetGame().IsDebug() )
		{
			m_HudDebug				= new HudDebug;
			
			if ( !m_HudDebug.IsInitialized() )
			{
				m_HudDebug.Init( GetGame().GetWorkspace().CreateWidgets("gui/layouts/debug/day_z_hud_debug.layout") );
				
				Debug.SetEnabledLogs(PluginConfigDebugProfile.GetInstance().GetLogsEnabled());
			}
		}

		//AIBehaviourHL.RegAIBehaviour("zombie2",AIBehaviourHLZombie2,AIBehaviourHLDataZombie2);
		//RegBehaviour("zombie2",AIBehaviourHLZombie2,AIBehaviourHLDataZombie2);
		
		cccp_WidgetsCache				= new WidgetCache;
		
		if( GetGame().IsMultiplayer() )
		{
			OnlineServices.m_MuteUpdateAsyncInvoker.Insert( SendMuteListToServer );
		}
		super.OnInit();
	}
	
	// Allow return of new hud widgets
	WidgetCache GetWidgetCache()
	{
		return cccp_WidgetsCache;
	}
	// End DS9
}
