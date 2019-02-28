/*
	Epoch Mod for DayZ Standalone

	FileName: PluginManager.c

	Authors: DayZ SA Epoch Dev Team and Community Contributors
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/README.md

	Licensing:
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/LICENSE.md

	Usage:
		-Check comment header of each method for more info
		-Check TestLogging() for example code and usage
	
	Changelog:

	[2/26/2019] Cleetus / DirtySanchez
		CL/DS(1)- convert PluginKillFeedLog into universal PluginProcessLogs
	[NewDate] Author(s)
		Initials(Change#)-Description
*/
class PluginProcessLogs extends PluginBase
{
	// debugging
	bool debugPluginProcessLogs = true;

	private FileHandle					emt_LogFile;
	private FileHandle					emt_NewLogFile;
	private FileHandle					emt_ProfileLog;
	private FileHandle					emt_SaveLog;
	private string 						SaveLocation 			= "$saves:";
	private string 						ProfileLocation 		= "$profile:";

	// Time variables for use in timestamping cycled files and inside logs
	int year, month, day, hour, minute, second;
	string sYear, sMonth, sDay, sHour, sMinute, sSecond, currentTime, currentDate, message;
	bool m_LogEnabled, result;
	string new_path, file_path, emt_LogFilePath;

	void PluginProcessLogs()
	{
		Print("Init: PluginProcessLogs and Init EpochServer.log");
		emt_LogFilePath = BuildFilePath("EpochServer", "log", "", "");
		
		// Init epoch server log
		CycleLogFile("EpochServer", "$profile:EpochServer.log");
		
		// testing only, remove when needed
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(TestLogging, 5 * 1000, false);
	}

	void ~PluginProcessLogs()
	{
		Print("~PluginProcessLogs Closed");
	}

	// testing only, remove when needed
	void TestLogging()
	{
		// EXAMPLES
		WriteLog("EpochServer", emt_LogFilePath, "Epoch Mod Server instance initiated. Happy Hosting!");

		WriteProfileLog("EpochServer", "Alternate form of writing to logs without pathing needed");

		//WriteSaveLog("EpochPlayerUID", "joined", GetDateString() + "_" + GetTimeString(false));
	}
	
	// Allows writing to exact log file name and path
	void WriteLog(string file_name, string file_path, string text)
	{
		emt_LogFile = OpenFile(file_path, FileMode.APPEND);
		FPrintln(emt_LogFile, GetTimeString(false) + " | " + text);
		CloseFile(emt_LogFile);
		
		// TODO: add in chat display for in game testing, maybe toggles for debug bools while in game as well
		
		// This will debug output to the scripts.log file
		if (debugPluginProcessLogs) Print(file_name + " logged " + text);
	}
	
	// This will save logs directly to profile without need for path
	void WriteProfileLog(string file_name, string text)
	{
		string path = ProfileLocation + file_name + ".log";
		emt_ProfileLog = OpenFile(path, FileMode.APPEND);
		FPrintln(emt_ProfileLog, GetTimeString(false) + " | " + text);
		CloseFile(emt_ProfileLog);
	}
	
	// This will save files to the server under profilefolder/users/server/file_name.txt
	void WriteSaveLog(string file_name, string text, string data)
	{
		string path = SaveLocation + file_name + ".txt";
		emt_SaveLog = OpenFile(path, FileMode.APPEND);
		FPrintln(emt_SaveLog, text + " " + data);
		CloseFile(emt_SaveLog);
	}
	
	// Checks for existing file, rotates or creates a new file
	void CycleLogFile(string file_name, string file_path)
	{
		message = "Log Created on " + GetDateString() + " at " + GetTimeString(false);
		if (FileExist(file_path))
		{
			if (debugPluginProcessLogs) Print(file_name + " exists, cycling it.");
			
			// Setting String for new file name
			string newFilePath = ProfileLocation + file_name + "_" + GetDateString() + "_" + GetTimeString(true) + ".log";
			
			// Copying Old Log to New Log filename
			CopyFile(file_path, newFilePath);

			// Removing Old Log to ensure no issues.
			DeleteFile(file_path);

			WriteLog(file_name, file_path, message);
		}
		else
		{
			if (debugPluginProcessLogs) Print(file_name + " does not exist, creating new.");
			WriteLog(file_name, file_path, message);
		}
	}
	
	// BuildFilePath
	//	-string file_name				// Exact name of your file, case sensitive
	//	-string file_type				// Exact type ie log or cfg or txt
	//	-string file_folder				// (OPTIONAL) Exact folder name where the file is or where it is to be created
	//	-string file_subfolder			// (OPTIONAL) If you need more storage use a subfolder :P
	// EXAMPLES:
	//	-BuildFilePath("KillFeed", "log", "", ""); <-- USE THIS FOR PROFILE FOLDER
	//	-BuildFilePath("KillFeed", "cfg", "KillFeed", ""); <-- CONFIGS AND SETTINGS GO IN FOLDERS
	//	-BuildFilePath("KillFeed", "cfg", "KillFeed", "PlayerStats"); <-- CONFIGS AND SETTINGS GO IN FOLDERS
	string BuildFilePath(string file_name, string file_type, string file_folder, string file_subfolder)
	{
		file_path = "";
		if(file_name == "" || file_type == "")
		{
			string log = "Failed attempt to build path for " + file_name + "." +  file_type;
			WriteProfileLog("EpochServer", log);
			return file_path;
		}
		
		if(file_folder == "")
		{
			file_path = ProfileLocation + file_name + "." + file_type;
			return file_path;
		}
		
		if(file_folder != "")
		{
			if(file_subfolder == "")
				file_path = ProfileLocation + file_folder + "/" + file_name + "." + file_type;
			
			if(file_subfolder != "")
				file_path = ProfileLocation + file_folder + "/" + file_subfolder + "/" + file_name + "." + file_type;
		}
		return file_path;
	}
	
	// Self explanatory :wink:
	string GetDateString()
	{
		// Getting Current Date
		GetYearMonthDay(year, month, day);
		sYear = year.ToString();
		
		sMonth = month.ToString();
		if (sMonth.Length() == 1)
			sMonth = "0" + sMonth;
		
		sDay = day.ToString();
		if (sDay.Length() == 1)
			sDay = "0" + sDay;

		currentDate = sYear + "-" + sMonth + "-" + sDay;
		return currentDate;
	}
	
	// file_name_friendly(true) adds "-" instead of ":" for cycling log files
	string GetTimeString(bool file_name_friendly)
	{
		// Getting Current Time
		GetHourMinuteSecond(hour, minute, second);
		
		// Setting String for Time Variables for time-stamp
		sHour = hour.ToString();
		if (sHour.Length() == 1)
			sHour = "0" + sHour;
		
		sMinute = minute.ToString();
		if (sMinute.Length() == 1)
			sMinute = "0" + sMinute;
		
		sSecond = second.ToString();
		if (sSecond.Length() == 1)
			sSecond = "0" + sSecond;
		
		currentTime = sHour + ":" + sMinute + ":" + sSecond;
		if(file_name_friendly)
			currentTime = sHour + "-" + sMinute + "-" + sSecond;
		
		return currentTime;
	}
}
