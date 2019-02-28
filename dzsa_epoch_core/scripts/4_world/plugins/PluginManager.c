/*
	Epoch Mod for DayZ Standalone

	FileName: PluginManager.c

	Authors: DayZ SA Epoch Dev Team and Community Contributors
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/README.md

	Licensing:
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/LICENSE.md


	Changelog:

	[2/26/2019] Cleetus / DirtySanchez
		CL/DS(1)- add Plugins to Plugin Manager
	[NewDate] Author(s)
		Initials(Change#)-Description
*/

modded class PluginManager
{
	override void Init()
	{
		/* Register your plugin here in the same fashion */
		
		//----------------------------------------------------------------------
		// Register modules
		//----------------------------------------------------------------------
		//				Module Class Name 						Client	Server
		//----------------------------------------------------------------------	
		RegisterPlugin( "PluginProcessLogs", 					true, 	true);
		RegisterPlugin( "PluginExample", 						false, 	true);
		
		// Call super to allow for other mods to utilize the init to register their plugins
		super.Init();
	}
}