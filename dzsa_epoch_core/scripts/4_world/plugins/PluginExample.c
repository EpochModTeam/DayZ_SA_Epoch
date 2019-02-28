/*
	Epoch Mod for DayZ Standalone

	FileName: PluginExample.c

	Authors: DayZ SA Epoch Dev Team and Community Contributors
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/README.md

	Licensing:
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/LICENSE.md


	Changelog:

	[2/26/2019] Cleetus / DirtySanchez
		CL/DS(1)- add PluginExample to Plugin Manager
	[NewDate] Author(s)
		Initials(Change#)-Description
*/
class PluginExample extends PluginBase
{
	//DEBUG
	bool debugPluginExample = false; // This will show all widgets upon loading into server

	// Plugins
	ref PluginProcessLogs emt_logging = PluginProcessLogs.Cast(GetPlugin(PluginProcessLogs));

	string message;
	bool result;

	void PluginExample()
	{
		Print("Init: PluginExample");

		// init vars and methods here
		message = "Hello World";

		if(debugPluginExample)
		{
			// CallLater(Method, time, repeat)
			MyMethod(message);
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(RepeatMethod, 60 * 1000, true); // 60 seconds
		}
	}
	
	void ~PluginExample()
	{
		// remove/delete/reset vars here
		message = "";
		
		Print("~PluginExample Closed");
	}

	// create methods for your feature
	void MyMethod(string str)
	{
		//Print(str);
		if(IsDebugOn())
			emt_logging.WriteLog("EpochServer","$profile:EpochServer.log","PluginExample has sent:" + str);
	}
	
	void RepeatMethod()
	{
		//Print(str);
		if(IsDebugOn())
			emt_logging.WriteLog("EpochServer","$profile:EpochServer.log",message);
	}

	// void becomes a bool because we want to return true or false
	bool IsDebugOn()
	{
		result = false;
		if(debugPluginExample)
			result = true;

		return result;
	}
}
