class DZE_BotEventInVehicle : BotEventBase { };

Man m_Target;
PlayerBase m_Owner;
protected ref BotFSM m_FSM;	
float m_dtAccumulator = 0.0;
float m_dtAccumulatorCheck = 0.0;
float m_dtAccumulatorDuration = 1.0;
BotEventBase dze_e;
bool hasCalled = false;
EntityAI debug_follow_ent;
EntityAI debug_follow_ent2;
class DZE_Bot_TrackTarget : BotStateBase
{
	string m_Type;
	ref DZE_BotFindTarget m_findtarget;
	ref DZE_BotFollowTarget m_followtarget;
	ref DZE_BotRoam m_roam;
	ref BotPerformAction m_botaction;
	

	void DZE_Bot_TrackTarget (DZE_Bot bot = NULL, BotStateBase parent = NULL,PlayerBase m_Owner = NULL)
	{

		//dze_e = this;
//Setup nested state machine Important  to use m_FSM to nest to parent FSM
		//BotFSM().m_InitialState
		m_FSM = new BotFSM(this); // @NOTE: set owner of the submachine fsm
		
//Actions		
		m_findtarget = new DZE_BotFindTarget(m_Bot,this);
		m_followtarget = new DZE_BotFollowTarget(m_Bot, this);
		m_roam = new DZE_BotRoam(m_Bot, this);
		
//Events
		BotEventBase __NotInSight__ = new BotEventHuntedTargetLost;//Lost or No Target
		BotEventBase __InSight__ = new BotEventHuntedTargetInSight;//Can see target
		BotEventBase __EndOk__ = new BotEventEndOK;//End Ok to manually drop out of transition
		
//Transitions
		m_FSM.AddTransition(new BotTransition(   m_findtarget, __InSight__, m_followtarget));
		m_FSM.AddTransition(new BotTransition(   m_findtarget, __NotInSight__, m_roam));
		m_FSM.AddTransition(new BotTransition(   m_roam, __InSight__, m_followtarget));
		m_FSM.SetInitialState(m_findtarget);
		
		//Message("Track Player Called : "+m_FSM.ToString()+" For "+bot.ToString()+" | " + this.ToString());		

	}

	override void OnEntry (BotEventBase e) { super.OnEntry(e); }
	override void OnExit (BotEventBase e) { super.OnExit(e); }
	
	
	override void OnUpdate (float dt) {
		m_dtAccumulator += 0.1;
		int acc = m_dtAccumulator;
			if (acc % 12 == 0)
			{
				//Message("Main Count : "+m_dtAccumulator.ToString());
			}		
		super.OnUpdate(dt); 
	}
	
	
};

class DZE_BotRoam extends BotStateBase
{
	float m_dtAccumulator = 0.0;
	override void OnEntry (BotEventBase e)
	{
		m_dtAccumulator = 0.0;
		super.OnEntry(e);
	}

	override void OnExit (BotEventBase e) {
		m_dtAccumulator = 0.0;
		m_Target = null;
		super.OnExit(e);
	}
	
	override void OnUpdate (float dt) {
		m_dtAccumulator += dt;
		int acc = m_dtAccumulator;
			if (acc % 12 == 0)
			{
				//Message("Roam Count : "+m_dtAccumulator.ToString());
			}	
		super.OnUpdate(dt);	
	}
};

class DZE_BotFollowTarget extends BotStateBase
{
	float m_dtAccumulator = 0.0;
	override void OnEntry (BotEventBase e)
	{
		if(!debug_follow_ent){
			debug_follow_ent = GetGame().CreateObject( "CarBattery", GetGame().GetPlayer().GetPosition() + "10 0 0", false, false );
		}
		m_Owner.GetInputController().OverrideMovementAngle(true, 1);
		//Message("Follow Target Entered.");
		m_dtAccumulator = 0.0;
		super.OnEntry(e);
	}

	override void OnExit (BotEventBase e) {
		m_dtAccumulator = 0.0;
		m_Target = null;
		super.OnExit(e);
	}
	
	override void OnUpdate (float dt) {
		m_dtAccumulator += 0.1;
		
			//if (m_dtAccumulatorCheck < m_dtAccumulator)
			//{
				
				DZE_FollowTarget();
				m_dtAccumulatorCheck = m_dtAccumulator + m_dtAccumulatorDuration;
			//}
		super.OnUpdate(dt);		
	}
	
	void DZE_FollowTarget(){
		
		if(m_Target && m_Owner){
			
			vector bot_pos = m_Owner.GetPosition();
			vector leaderpos = m_Target.GetPosition();
			float disttotrgt = vector.Distance(bot_pos,leaderpos);
			if(disttotrgt>3){
				//Message("Do Follow Target "+m_Bot.ToString()+" | "+m_Owner.ToString());
				
				vector tv = (GetVelocity(m_Target) * -4.0);
				
				vector tv2 = tv.Normalized() * 2.0;
				vector behind = leaderpos + tv2;
				debug_follow_ent.SetPosition(behind);
				//Message(behind.ToString()+" | "+leaderpos.ToString());
												
				

//
//ADD NAVMESH CHECK
//					
					
				vector bot_ahead = bot_pos + (GetVelocity(m_Owner).Normalized() * 0.6);						
				vector samplemesh;
				bool meshtest = GetGame().GetWorld().GetAIWorld().SampleNavmeshPosition(bot_ahead,0.4,samplemesh);
				
				if(!meshtest){
					GetGame().GetWorld().GetAIWorld().SampleNavmeshPosition(bot_ahead,3,samplemesh);
					behind = samplemesh;
				}
				
				if(!debug_follow_ent2){
					debug_follow_ent2 = GetGame().CreateObject( "CarBattery", GetGame().GetPlayer().GetPosition() + "10 0 0", false, false );
				}
				debug_follow_ent2.SetPosition(behind);
				
//
//APPLY FINAL DIRECTION
//															
//
				vector avoid_dir = vector.Direction(bot_pos,behind);
				m_Owner.SetDirection(avoid_dir);
				
//SPEED - TODO ADJUST BASED ON PLAYER SPEED
//
			
				//Message(disttotrgt.ToString());
				if(disttotrgt<3.1){
					m_Owner.GetInputController().OverrideMovementSpeed(true, 0);
				}else if(disttotrgt>14){
					m_Owner.GetInputController().OverrideMovementSpeed(true, 3);
				}
				else if(disttotrgt>8){
					m_Owner.GetInputController().OverrideMovementSpeed(true, 2);
				}
				else if(disttotrgt>3){
					m_Owner.GetInputController().OverrideMovementSpeed(true, 1);
				}
			}
			
			
		}
	}
};



class DZE_BotFindTarget extends BotStateBase
{
	float m_dtAccumulator = 0.0;
	
	bool m_TargetInSight = false;
	bool m_TargetLost = false;
	bool m_Tracking = true;
	
	/*
	void DZE_BotFindTarget(DZE_Bot m_bot,BotEventBase e){
		dze_e = e;
	}
	*/
	
	override void OnEntry (BotEventBase e)
	{
		
		m_dtAccumulator = 0.0;
		//dze_e = e;
		//Message("Track Target Entered from FSM "+m_Owner.ToString()+" | "+dze_e.ToString());
		DZE_SelectTarget(m_Owner,e);
		
		super.OnEntry(e);
	}

	override void OnExit (BotEventBase e) {
		
		m_dtAccumulator = 0.0;
		//Message("Track Target Exited.");
		
		super.OnExit(e);
	}
//
//TODO - ONLY CALL ONCE PER acc % 12
//	
	override void OnUpdate (float dt) {
		
		m_dtAccumulator += 0.1;
		int acc = m_dtAccumulator;
		
			if (acc % 12 == 0)
			{
				
				if(m_dtAccumulatorCheck < m_dtAccumulator){
					//Message("Find Count : "+m_dtAccumulator.ToString()+" | " + dt.ToString());
					if(DZE_SelectTarget(m_Owner,dze_e)){
						m_Bot.ProcessEvent(new BotEventHuntedTargetInSight(m_Owner));
					}
				}
			}
		
		
		super.OnUpdate(dt);
	}
};

bool DZE_SelectTarget (EntityAI bot, BotEventBase e)
{
	
	ref array<Man> players = new array<Man>;

	GetGame().GetPlayers(players);	
	//Message("Looking for Target "+m_dtAccumulator.ToString()+" | "+players.Count().ToString());
	
	bool minimal_distance_ok = true;

	float min_dist = 100.0;
	int min_index = -1;
	for ( int i = 0; i < players.Count(); i++ )
	{
		float d = vector.Distance(players.Get(i).GetPosition(), bot.GetPosition());
		//Message(players.Get(i).ToString());
		if ( d < min_dist && players.Get(i)!=bot)
		{
			min_dist = d;
			min_index = i;
		}
	}

	if (min_index != -1){
		//Message("Target Found");
		m_Target = players.Get(min_index);
		return true;
	}
	
	m_dtAccumulatorCheck = m_dtAccumulator;
	return false;
}



class BotPerformAction extends BotStateBase
{
	EntityAI m_Entity;
	int m_ActionID;

	void BotPerformAction (Bot bot = NULL, BotStateBase parent = NULL, int action = AT_EAT)
	{
		m_ActionID = action;
	}

	override void OnEntry (BotEventBase e)
	{
		super.OnEntry(e);

		if (e.m_Entity)
		{
			botDebugPrint("[bot] + " + m_Owner + " performing action on item=" + e.m_Entity);

			if (m_Owner.GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT)
			{
#ifdef BOT
				ActionManagerClient cli_mgr = ActionManagerClient.Cast(m_Owner.GetActionManager());
				if (cli_mgr)
					cli_mgr.PerformAction(m_ActionID, new ActionTarget(e.m_Entity, null, -1, vector.Zero, -1), e.m_Entity);
#endif
			}
		}
	}

	override void OnAbort (BotEventBase e) { super.OnAbort(e); }
	override void OnExit (BotEventBase e) { super.OnExit(e); }
	override void OnUpdate (float dt) { super.OnUpdate(dt); }
};





