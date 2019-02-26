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
	string message;
	
	void PluginExample()
	{
		Print("Init: PluginExample");
		// init vars and methods here
		message = "Hello World";
		MyMethod(message);
	}
	
	void ~PluginExample()
	{
		// remove/delete/reset vars here
		Print("~PluginExample Closed");
	}
	
	// create methods for your feature
	void MyMethod(string str)
	{
		Print(str);
	}
}