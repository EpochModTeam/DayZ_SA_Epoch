/*
	Epoch Mod for DayZ Standalone
	
	FileName: LogoutMenu.c

	Authors: DayZ SA Epoch Dev Team and Community Contributors
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/README.md	
	
	Licensing:
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/LICENSE.md

	Changelog:

	[1/16/2019] DirtySanchez
		DS7 - Port CCCP Character Data Protection into DZSA Epoch
			- Player Data and Combat Protection Enabled is new message
			- minimum 7s countdown but will allow any setting above 7 to process
			- TODO: OnHit and OnDamage upgrades for true combat logging protection
	[NewDate] Author(s)
		Initials(Change#)-Description
*/

modded class LogoutMenu extends UIScriptedMenu
{
	// DS7 Character Data Protection PortIN
	// private variables cannot be modded, they must be recreated in your modded class
	// Find all instances of original and override the method to gain control of the variable in your work
	private TextWidget cccp_LogoutTimetext;
	private TextWidget cccp_Info;
	private ButtonWidget cccp_bLogoutNow;
	private ButtonWidget cccp_bCancel;
	private int cccp_iTime;

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("emt/dzsa_epoch_core/layouts/day_z_logout_dialog.layout");

		cccp_LogoutTimetext = TextWidget.Cast( layoutRoot.FindAnyWidget("logoutTimeText") );
		cccp_Info = TextWidget.Cast( layoutRoot.FindAnyWidget("txtInfo") );
		cccp_bLogoutNow = ButtonWidget.Cast( layoutRoot.FindAnyWidget("bLogoutNow") );
		cccp_bCancel = ButtonWidget.Cast( layoutRoot.FindAnyWidget("bCancel") );
#ifdef PLATFORM_CONSOLE
		cccp_bCancel.Show( false );
		cccp_bLogoutNow.Show( false );

		cccp_bCancel = ButtonWidget.Cast( layoutRoot.FindAnyWidget("bCancelConsole") );
		cccp_bCancel.Show( true );
#else
		cccp_bCancel.Show( true );
		cccp_bLogoutNow.Show( true );
		layoutRoot.FindAnyWidget("bCancelConsole").Show( false );
#endif
		UpdateInfo();

		// player should sit down if possible
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player.GetEmoteManager() && !player.IsRestrained() && !player.IsUnconscious()) 
		{
			player.GetEmoteManager().CreateEmoteCBFromMenu(ID_EMOTE_SITA);
		}

		return layoutRoot;
	}

	override void SetLogoutTime()
	{
		cccp_LogoutTimetext.SetText(" ");
	}

	override void SetTime(int time)
	{
		// Logout time modification here for in combat or other usage to stop log out
		cccp_iTime = 7;
		if(time > 7) cccp_iTime = time;
		cccp_LogoutTimetext.SetText(cccp_iTime.ToString());
	}

	override void UpdateTime()
	{
		if (cccp_iTime > 0)
		{
			cccp_iTime -= 1;
			cccp_LogoutTimetext.SetText(cccp_iTime.ToString());		
		}
		else
		{
			Exit();
		}
	}

	override void UpdateInfo()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if (player.IsRestrained() || player.IsUnconscious())
		{
			// display killInfo
			cccp_Info.SetText("#layout_logout_dialog_note_killed");
		}
		else
		{
			// Set Button Text
			cccp_bLogoutNow.SetText("Please Wait");
			// Set dialog main text
			cccp_Info.SetText("Player Data & Combat Protection Enabled. Please Wait.");
			//cccp_Info.SetText("#layout_logout_dialog_note");
		}
		//for future combat logging type
		//cccp_bLogoutNow.SetText("In Combat");
		//cccp_Info.SetText("Combat Logging Protection Enabled. Please Wait.");
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		if (w == cccp_bLogoutNow)
		{
			//GetGame().GetMission().AbortMission();

			//return true;
			//check lastHitByTime > 30s and/or lastNearHitTime* > 30s | *needs to be created
			return false;
		}
		else if (w == cccp_bCancel)
		{
			Cancel();
			return true;
		}

		return false;
	}
}
