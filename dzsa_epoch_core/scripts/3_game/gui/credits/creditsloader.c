/*
	Epoch Mod for DayZ Standalone
	
	FileName: CreditsLoader.c

	Authors: DayZ SA Epoch Dev Team and Community Contributors
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/README.md	
	
	Licensing:
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/LICENSE.md

	Changelog:

	[1/16/2019] AWOL / DirtySanchez
		AWOL1 - DZSA Epoch Dev Team and Community Credits
	[1/20/2019] AWOL
		AWOL2 - Made credits dynamically support other mods via 'creditsJson' var in CfgMods. Also appended DayZ Game header and credits
		
	[NewDate] Author(s)
		Initials(Change#)-Description
*/
modded class CreditsLoader
{
	protected static const string JSON_FILE_PATH_EPOCH = "Scripts/Data/credits.json";
	
	override static JsonDataCredits GetData()
	{

		// get all mods
		int modsCount = GetGame().ConfigGetChildrenCount("CfgMods");

		JsonDataCredits data = new JsonDataCredits;
		data.Departments = new array<ref JsonDataCreditsDepartment>;

		// Add Mod header
		ref JsonDataCreditsDepartment data_department_modheader = new JsonDataCreditsDepartment;
		data_department_modheader.Sections = new array<ref JsonDataCreditsSection>;
		data_department_modheader.DepartmentName = ("");
		data.Departments.Insert(data_department_modheader);
		ref JsonDataCreditsSection data_section_modheader = new JsonDataCreditsSection;
		data_section_modheader.SectionLines = new array<string>;
		data_section_modheader.SectionName = ("");
		data_section_modheader.SectionLines.Insert("Mod Credits");
		data_department_modheader.Sections.Insert(data_section_modheader);

		// Append credits for each mod that has a creditsJson defined.
		for (int i = 0; i < modsCount; i++)
		{
			string modName = "";
			GetGame().ConfigGetChildName("CfgMods", i, modName);
			string jsonCredits = "";
			GetGame().ConfigGetText("CfgMods " + modName + " creditsJson", jsonCredits);
			
			if (jsonCredits != "") {
				ref JsonDataCredits jsondata;
				JsonFileLoader<ref JsonDataCredits>.JsonLoadFile(jsonCredits, jsondata);
				foreach(auto d: jsondata.Departments) {
					data.Departments.Insert(d);
				};
			}
		}

		// Append DayZ Game Credits Header
		ref JsonDataCreditsDepartment data_department_header = new JsonDataCreditsDepartment;
		data_department_header.Sections = new array<ref JsonDataCreditsSection>;
		data_department_header.DepartmentName = ("				DayZ Game Credits");
		data.Departments.Insert(data_department_header);
		// Append DayZ Game Credits
		ref JsonDataCredits existingjsondata;
		JsonFileLoader<ref JsonDataCredits>.JsonLoadFile(JSON_FILE_PATH_EPOCH, existingjsondata);
		foreach(auto b: existingjsondata.Departments) {
			data.Departments.Insert(b);
		};
		
		return data;
	}
}
