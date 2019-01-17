/*
	Epoch Mod for DayZ Standalone
	
	FileName: DayZGame.c

	Authors: DayZ SA Epoch Dev Team and Community Contributors
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/README.md	
	
	Licensing:
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/LICENSE.md

	Changelog:

	[1/14/2019] DirtySanchez
		DS1-Customized Loading Screen
		DS2-Turn all messaging(admin/system/chat) ON by default
		DS3-Remove spawning in countdown screen to show Custom Loading Screen for set time
		DS4-Override "Loading..." to "Epoch Mod Loading..." while keeping language translations
	[NewDate] Author(s)
		Initials(Change#)-Description
*/

modded class LoadingScreen
{
	// DS1 DZSA Epoch Custom Loading Screen
	bool LSCheck = true;
	override void Show()
	{
		if(LSCheck)
		{
			m_ImageWidgetBackground.LoadImageFile( 0, LoadingScreenTextures().GetRandomElement());
			LSCheck = false;
		}
		// Call super.MethodName();
		// It can be used before your work, after or even in between your changes.
		// This will allow other mods and the DayZ SA game to process their code as well.
		// super.Show() is called afterwards here so that we can establish a new loading screen texture before their code processes
		super.Show();
	}

	// Paths for loading screens - Keep the comma at the end of the last path
	TStringArray LoadingScreenTextures()
    {
        return {
			"emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-1.paa", "emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-2.paa", "emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-3.paa", "emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-4.paa", "emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-5.paa", "emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-6.paa", "emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-7.paa", "emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-8.paa", "emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-9.paa",
			"emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-10.paa",
			"emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-11.paa",
			"emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-12.paa",
			"emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-13.paa",
			"emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-14.paa",
			"emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-15.paa", "emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-16.paa", "emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-17.paa", "emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-18.paa", "emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-19.paa",
			"emt/dzsa_epoch_assets/DS/UI/LoadingScreen/LoadingScrTexture-20.paa",
		};
	}
	// End DS1 DZSA Epoch Custom Loading Screen 
}

modded class DayZGame extends CGame
{
	// DS2 DZSA Epoch Turn All Messaging On When Enter Server(DZ SA is OFF by default)
	// will not change them from disabled to enabled until joining into a server
	ref DayZProfilesOptions dze_DayZProfileOptions;

	override void RegisterProfilesOptions()
	{
		// This super call is before our work so our work overrides dayz original while allowing compatibility for other mods
		super.RegisterProfilesOptions();
		
		dze_DayZProfileOptions = new DayZProfilesOptions;
		dze_DayZProfileOptions.RegisterProfileOption(EDayZProfilesOptions.GAME_MESSAGES, GAME_CHAT_MSG, true);
		dze_DayZProfileOptions.RegisterProfileOption(EDayZProfilesOptions.ADMIN_MESSAGES, ADMIN_CHAT_MSG, true);
		dze_DayZProfileOptions.RegisterProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES, PLAYER_CHAT_MSG, true);
		// End DS2
	}
	
	// DS3 DZSA Epoch Override "Spawning In" Countdown Screen for longer viewing of Custom Loading Screen Textures
	// <var name="QueueTimeSameServer" type="0" value="20"/> in GLOBALS.XML is how long custom loading screen texture shows
	// DZE_queueTime was m_queueTime, but this variable is "private" and cannot be used in modded classes
	// With new variable established all cases of m_queueTime need to be overrode
	private int 	DZE_queueTime; 
	override void OnEvent(EventType eventTypeId, Param params)
	{
		string address;
		int port;
		int high, low;
		
		switch(eventTypeId)
		{
			case ClientSpawningEventTypeID:
				ClientSpawningEventParams spawningParams;
				if (Class.CastTo(spawningParams, params))
				{
					DZE_queueTime = spawningParams.param1;
					OnClientSpawningEvent(DZE_queueTime, spawningParams.param2);
				}
			break;
		}
		super.OnEvent(eventTypeId, params);
	}

	override void ClientSpawning(bool newChar)
	{
		if (DZE_queueTime >= 0) // count all the way to zero
		{
			// Remove countdown timer so loading screen will display for the entire countdown timer
			/*#ifndef NO_GUI	
			string text = "#dayz_game_spawning_in" + " " + DZE_queueTime.ToString() + " " + "#dayz_game_seconds";
			GetUIManager().ScreenFadeIn(0, text, FadeColors.BLACK, FadeColors.WHITE);
			#endif*/
			DZE_queueTime--;// End DS3
		}
		else
		{
			// hide spawning text
			CancelQueueTime();
			ClientSpawningFinished(newChar);
		}
	}// End DS3
	
	// DS4 DZSA Epoch Welcome Message
	string msg = "";
	override void ClientSpawningFinished(bool newChar)
	{		
		// tell game to continue
		StoreLoginData();
		
		//Toggles logo (loading screen)
		Mission mission = GetMission();
		
		if ( mission && m_IsPlayerSpawning )
		{
			m_loading = new LoadingScreen(this);
			m_loading.Show();
			
			// Start DS4
			//m_loading.SetTitle("#dayz_game_loading");
			// Keep stringtable call for Loading.. - This will keep all translations active
			msg = "Epoch Mod " + "#dayz_game_loading";
			m_loading.SetTitle(msg);
			// End DS4
			
			m_IsPlayerSpawning = false;
		}
	}	
}
