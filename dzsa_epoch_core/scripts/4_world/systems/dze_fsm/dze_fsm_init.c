/**@class		FsmTrigger
 **/
class DZE_FsmTrigger
{
	bool CheckTrigger () { return true; }
};

class DZE_MyFsmTrigger : DZE_FsmTrigger
{
	PlayerBase m_Owner;
	void DZE_MyFsmTrigger (PlayerBase p) { m_Owner = p; }
	//override bool CheckTrigger () { return null != m_Owner.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("Headgear")); }
};

//m_Timer = new DZE_TimerTest;

/*
Usage:
dze_Player = PlayerBase.Cast(GetGame().GetPlayer());
DZE_Fsm_Init dze_fsm_init = new DZE_Fsm_Init(dze_Player);
dze_fsm_init.Start(false);


*/
class DZE_Fsm_Init
{
	PlayerBase m_Owner = null;//Owner of FSM (Player / Bot)
	protected ref DZE_Timer dze_Timer;
	protected ref Timer m_Timer;
	protected const float c_TriggerTimeoutMS = 1000.0;
	//protected const float c_UpdateMS = 2000.0;
	protected const float c_UpdateMS = 16.0;
	protected ref DZEFsm m_FSM = null;
	protected bool m_UseTrigger = false;
	protected bool m_Triggered = false;
	protected DayZPlayerInstanceType m_InstanceType = DayZPlayerInstanceType.INSTANCETYPE_CLIENT;
	protected ref DZE_FsmTrigger m_FsmTrigger = null;
	protected ref DZEStateBase m_DZEFsmTest = null;
	protected const float dze_checkmainfsmdur = 1.0;

	void DZE_Fsm_Init (PlayerBase ow)
	{
		m_Owner = ow;
		//dze_Timer = new DZE_Timer();
	}
	
	void SetInstanceType (DayZPlayerInstanceType t) { m_InstanceType = t; }

	void Start (bool use_trigger, DZE_FsmTrigger trigger = null)
	{
		m_UseTrigger = use_trigger;

		InitFSM();
		//m_FSM.GetCurrentState();

		if (m_UseTrigger)
		{
			if (m_UseTrigger && trigger == null)
				Error("use trigger, but trigger null");

			//m_Triggered = false;
			//m_FsmTrigger = trigger;
			//DZEFsmDebugPrint("[fsminit] + " + m_Owner + " DZE FSM waiting for trigger...");
			//m_Timer.Run(c_TriggerTimeoutMS / 1000.0, this, "OnTrigger", null, true);

		}
		else
		{
			//dze_Timer.Run(dze_checkmainfsmdur, this, "OnUpdate", null, true);
			//Message("Main FSM for " + m_Owner + " Started with timer : "+dze_Timer.ToString()+" Running : "+m_Timer.IsRunning().ToString()+" | Duration : "+m_Timer.GetDuration().ToString());
		}
	}

	void DelayedStart (float ms)
	{
		m_Timer.Run(0.01, this, "OnDelayedStart", null, false);
	}

	protected void OnDelayedStart ()
	{
		Start(false, null);

		ProcessEvent(new DZEEventStart(m_Owner));
		m_UseTrigger = false;
	}

	void Stop ()
	{
		//Message("FSM Stop");
		m_Triggered = false;
		m_Timer.Stop();
		m_FSM.Terminate();
	}

	void OnTrigger()
	{
		//Message("Trigger FSM Init");
		//Print("Trigger FSM Init");
		
		
		if (!m_Triggered)
		{
			
			//m_Timer. (); // stop trigger timer
			m_Triggered = true;
			ProcessEvent(new DZEEventStart(m_Owner));
			m_Timer.Run(0.01, this, "OnUpdate",  new Param1<int>(0), true);

			DZEFsmDebugPrint("[fsminit] + " + m_Owner + " Started test with timer : "+m_Timer.ToString());
			
		}
		else
		{
			
			//m_Timer.Stop(); // stop update timer
			m_Triggered = false;
			ProcessEvent(new DZEEventStop(m_Owner));
			m_Timer.Run(0.01, this, "OnTrigger", null, true);
			DZEFsmDebugPrint("[fsminit] + " + m_Owner + " Stopped test!");
			
		}
	}

	void OnUpdate (float dt)
	{
		//Message("Main FSM : On Update");
		m_FSM.GetCurrentState().OnUpdate(dt);

		//OnTrigger(); // to detect trigger stop
	}

	void OnTimer ()
	{
		//Message("Timer : ");
		m_FSM.GetCurrentState().OnUpdate(c_UpdateMS / 1000.0);

		//OnTrigger(); // to detect trigger stop
	}

	void InitFSM ()
	{
		//Message("Init FSM");
		m_FSM = new DZEFsm();

		// basic states
		DZEStateBase DZEIdle = new DZEStateIdle(this, NULL);

		m_DZEFsmTest = new DZE_NavMeshTest(this, NULL, m_Owner);

		// events
		DZEEventBase ___Bgn__ = new DZEEventStart;
		DZEEventBase __Stop__ = new DZEEventStop;
		DZEEventBase ___OK___ = new DZEEventEndOK;
		DZEEventBase __Fail__ = new DZEEventEndFail;
		DZEEventBase __Tout__ = new DZEEventEndTimeout;
		DZEEventBase __IChg__ = new DZEEventOnItemInHandsChanged;

		///@{ transition table
		m_FSM.AddTransition(new DzeTransition(  DZEIdle     , null,   m_DZEFsmTest));
		m_FSM.AddTransition(new DzeTransition(  m_DZEFsmTest     , __Stop__,        NULL));

		m_FSM.SetInitialState(DZEIdle);
		m_FSM.Start();
		//Message("InitFSM : "+m_FSM.GetCurrentState().ToString());
	}
	
	bool ProcessEvent (DZEEventBase e)
	{
		DZEFsmDebugSpam("FSM : "+m_FSM.ToString() + " | Event Base : "+ e.ToString()+" | Process Result : ");
		
		if (m_FSM.ProcessEvent(e) == ProcessEventResult.FSM_OK)
		{
			DZEFsmDebugSpam("[fsminit] Processed event e=" + e.ToString());
			return true;
		}
		else
		{
			DZEFsmDebugSpam("[fsminit] FSM refused to process event (no transition): src=" + m_FSM.GetCurrentState().ToString() + " event=" + e.ToString());
			return false;
		}
		
	}
};

void DZEFsmDebugPrint (string s)
{
	Print("" + s); // comment/uncomment to hide/see debug logs
}

void DZEFsmDebugSpam (string s)
{
	Print("" + s); // comment/uncomment to hide/see debug logs
}

