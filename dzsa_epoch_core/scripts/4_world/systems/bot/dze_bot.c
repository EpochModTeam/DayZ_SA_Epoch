/**@class		BotTrigger
 **/
class DZE_BotTrigger extends BotTrigger
{
	bool CheckTrigger () { return true; }
};

class DZE_MyBotTrigger extends DZE_BotTrigger
{
	//PlayerBase m_Owner;
	void DZE_MyBotTrigger (PlayerBase p) { m_Owner = p; }
	//override bool CheckTrigger () { return null != m_Owner.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("Headgear")); }
	override bool CheckTrigger () { return true; }
};


/**@class		DZE_Bot
 **/
class DZE_Bot : Bot
{
	//PlayerBase m_Owner = null;
	//protected ref Timer m_Timer = new Timer;
	//protected const float c_TriggerTimeoutMS = 1000.0;
	//protected const float c_UpdateMS = 2000.0;
	//protected const float c_UpdateMS = 16.0;
	//protected ref BotFSM m_FSM = null;
	//protected bool m_UseTrigger = false;
	//protected bool m_Triggered = false;
	//protected DayZPlayerInstanceType m_InstanceType = DayZPlayerInstanceType.INSTANCETYPE_CLIENT;
	//protected ref BotTrigger m_BotTrigger = null;
	//protected ref BotStateBase m_BotTest = null;

	
	
	void DZE_Bot (PlayerBase ow)
	{
		//Message("DZE Bot Constructed");
		m_Owner = ow;
		
	}
	
	override void SetInstanceType (DayZPlayerInstanceType t) { m_InstanceType = t; }

	override void Start (bool use_trigger, BotTrigger trigger = null)
	{
		//Message("DZE Bot Started.");
		
		m_UseTrigger = use_trigger;

		InitFSM();

		if (m_UseTrigger)
		{
			if (m_UseTrigger && trigger == null)
				Error("use trigger, but trigger null");

			m_Triggered = false;
			m_BotTrigger = trigger;
			botDebugPrint("[bot] + " + m_Owner + " Epoch Modded Bot waiting for trigger for "+m_Owner.ToString());
			//m_Timer.Run(c_TriggerTimeoutMS / 1000.0, this, "OnTrigger", null, true);
		}
		else
		{
			botDebugPrint("[bot] + " + m_Owner + " Epoch Modded Bot Started.");
			//m_Timer.Run(c_UpdateMS / 1000.0, this, "OnTimer", null, true);
		}
		
		
		//super.Start(use_trigger,trigger);
	}

	override void DelayedStart (float ms)
	{
		//m_Timer.Run(ms / 1000.0, this, "OnDelayedStart", null, false);
	}

	protected void OnDelayedStart ()
	{
		Start(false, null);

		ProcessEvent(new BotEventStart(m_Owner));
		m_UseTrigger = false;
	}

	override void Stop ()
	{
		m_Triggered = false;
		//m_Timer.Stop();
		m_FSM.Terminate();
	}

	protected void OnTrigger ()
	{
		//Print("Trigger..");
		bool triggered = m_BotTrigger.CheckTrigger();

		if (!m_Triggered)
		{
			if (triggered)
			{
				//m_Timer.Stop(); // stop trigger timer
				m_Triggered = true;
				ProcessEvent(new BotEventStart(m_Owner));
				//m_Timer.Run(c_UpdateMS / 1000.0, this, "OnUpdate", null, true);
				botDebugPrint("[bot] + " + m_Owner + " Started test!");
			}else{
				botDebugPrint("[bot] + " + m_Owner + " No Trigger!");
			}
		}
		else
		{
			if (!triggered)
			{
				//m_Timer.Stop(); // stop update timer
				m_Triggered = false;
				ProcessEvent(new BotEventStop(m_Owner));
				//m_Timer.Run(c_TriggerTimeoutMS / 1000.0, this, "OnTrigger", null, true);

				botDebugPrint("[bot] + " + m_Owner + " Stopped test!");
			}
		}
	}

	override void OnUpdate (float dt)
	{
		//Message("BOT FSM Update");
		if(m_FSM){
			m_FSM.GetCurrentState().OnUpdate(dt);
		}

		//OnTrigger(); // to detect trigger stop
	}

	override void OnTimer ()
	{
		//m_FSM.GetCurrentState().OnUpdate(c_UpdateMS / 1000.0);

		//OnTrigger(); // to detect trigger stop
	}

	override void InitFSM ()
	{
		//Message("Init Bot FSM.");
		
		m_FSM = new BotFSM();

		// basic states
		BotStateBase BotIdle = new BotStateIdle(this, NULL);
		// unstable (intermediate) states
		//m_BotTest = new BotTestAttachAndDropCycle(this, NULL);
		//m_BotTest = new Bot_TestSpawnOpen(this, NULL);
		//m_BotTest = new Bot_TestSpawnOpenDestroy(this, NULL);
		//m_BotTest = new Bot_TestSpawnOpenEat(this, NULL);
		//m_BotTest = new BotTestSwapG2H(this, NULL);
		//m_BotTest = new BotTestSwapC2H(this, NULL);
		//m_BotTest = new BotTestSwapInternal(this, NULL);
		//m_BotTest = new DZE_BotHunt(this, NULL);
		//m_BotTest = new DZE_BotHuntVehicle(this, NULL);
		//m_BotTest = new Bot_TestSpawnDeadSelectBury(this, NULL);
		m_BotTest = new DZE_Bot_TrackTarget(this, NULL, m_Owner);
		//m_BotTest = new DZE_NavMeshTest(this, NULL, m_Owner);
		
		// events
		BotEventBase ___Bgn__ = new BotEventStart;
		BotEventBase __Stop__ = new BotEventStop;
		BotEventBase ___OK___ = new BotEventEndOK;
		BotEventBase __Fail__ = new BotEventEndFail;
		BotEventBase __Tout__ = new BotEventEndTimeout;
		BotEventBase __IChg__ = new BotEventOnItemInHandsChanged;

		///@{ transition table
		m_FSM.AddTransition(new BotTransition(  BotIdle     , ___Bgn__,   m_BotTest));
		m_FSM.AddTransition(new BotTransition(  BotIdle     , __Stop__,        NULL));

		m_FSM.AddTransition(new BotTransition(m_BotTest     , __IChg__,   m_BotTest));

		m_FSM.AddTransition(new BotTransition(m_BotTest     , ___OK___,     BotIdle));
		m_FSM.AddTransition(new BotTransition(m_BotTest     , __Fail__,     BotIdle));
		m_FSM.AddTransition(new BotTransition(m_BotTest     , __Tout__,     BotIdle));
		///@} transition table

		m_FSM.SetInitialState(BotIdle);
		m_FSM.Start();
		
		
	}
	
	override bool ProcessEvent (BotEventBase e)
	{
		if (m_FSM.ProcessEvent(e) == ProcessEventResult.FSM_OK)
		{
			botDebugSpam("[botfsm] Processed event e=" + e.ToString());
			return true;
		}
		else
		{
			botDebugSpam("[botfsm] FSM refused to process event (no transition): src=" + m_FSM.GetCurrentState().ToString() + " event=" + e.ToString());
			return false;
		}
	}
};
/*
void botDebugPrint (string s)
{
	Print("" + s); // comment/uncomment to hide/see debug logs
}

void botDebugSpam (string s)
{
	Print("" + s); // comment/uncomment to hide/see debug logs
}
*/