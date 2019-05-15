/*
class BotEventHuntedTargetInSight : BotEventBase
{
	//EntityAI m_Entity;
	//void BotEventEntityAttached (PlayerBase p = NULL, EntityAI att = NULL) { m_Entity = att; }
};

class BotEventHuntedTargetLost : BotEventBase
{
	//EntityAI m_Entity;
	//void BotEventEntityDetached (PlayerBase p = NULL, EntityAI att = NULL) { m_Entity = att; }
};


DZE_Bot bot_ower;
*/


class DZE_BotEventNextToVehicle : BotEventBase
{
	//EntityAI m_Entity;
	/*
	void DZE_BotEventNextToVehicle (PlayerBase p = NULL, EntityAI att = NULL) 
	{
		m_Entity = att;
	}
	*/
};
EntityAI debug_veh_ent;
EntityAI debug_veh_ent2;
/*

enum MeleeCombatHit
{
	NONE = -1,

	LIGHT,
	HEAVY,
	SPRINT,
	KICK,

	WPN_HIT,
	WPN_STAB,
}
*/
Car m_TargetVehicie;
ItemBase dze_item;
DZE_ActionData dze_action_data;
bool dze_getinStarted = false;
bool dze_getinFinished = false;
HumanCommandVehicle dze_vehCommand;

class DZE_BotNextToVehicle extends BotStateBase
{
	Car m_Target;
	Bot m_owner;
	float m_dtAccumulator = 0.0;

	void DZE_BotNextToVehicle (Bot bot = NULL, BotStateBase parent = NULL, int action = AT_EAT)
	{
		if(bot){
			m_owner = bot;
		}
	}
	
	void SelectTarget()
	{
		//BotSelectNearestTarget
		m_Target = DZE_BotSelectNearestTargetVehicle(m_Owner);
		botDebugPrint("[bot] + " + m_Owner + " DZE_BotNextToVehicle target=" + m_Target);
	}

	void OnEntry (BotEventBase e)
	{
		Message("Bot Next To Vehicle On Entry.");
		m_dtAccumulator = 0.0;
		SelectTarget();
	}

	void OnExit (BotEventBase e)
	{
		Message("Bot Next To Vehicle On Exit.");
		m_dtAccumulator = 0.0;
		m_Target = null;
	}

	void OnUpdate (float dt)
	{
		m_dtAccumulator += dt;
		
		if (m_Target == null)
		{
			if (m_dtAccumulator > 2.0)
			{
				
				botDebugPrint("[bot] + " + m_Owner + " searching...");
				DZE_BotNextToVehicle();
				if (m_Target)
				{
					Error("TODO");
					//m_Bot.PostEvent(new BotEventSelectedTarget(m_Owner, m_Target));
				}

				m_dtAccumulator = 0.0;
			}
		}
		else
		{
					Error("TODO");
			//m_Bot.PostEvent(new BotEventSelectedTarget(m_Owner, m_Target));
		}
	}
};

class DZE_BotPerformAction extends BotStateBase
{
	EntityAI m_Entity;
	int m_ActionID;

	void DZE_BotPerformAction (Bot bot = NULL, BotStateBase parent = NULL, int action = AT_EAT)
	{
		//Message("Bot Action Called");
		m_ActionID = action;
	}

	void OnEntry (BotEventBase e)
	{
		if(m_Owner){
		//PlayerBase.GetCommand_Vehicle(
		PlayerBase dze_player = PlayerBase.Cast(GetGame().GetPlayer());
		//Create Action Target
		ActionTarget dze_actiontrgt = new ActionTarget(m_TargetVehicie, null, -1, vector.Zero, -1);
		//Get Seat
		int dze_componentIndex = dze_actiontrgt.GetComponentIndex();
		int dze_crewIdx = m_TargetVehicie.CrewPositionIndex(dze_componentIndex);
		//Create Action Base
		DZE_ActionBase dze_action_base = new DZE_ActionBase;
		//Create Action Data
		dze_action_base.DZE_SetupAction(m_Owner,dze_actiontrgt,dze_item,dze_action_data);
		
		int dze_seat = m_TargetVehicie.GetSeatAnimationType(dze_crewIdx);
		
		
			//if( !m_Owner.GetCommand_Vehicle().IsGettingIn() && !m_Owner.IsInVehicle() )
			//{
				//m_Owner.GetItemAccessor().HideItemInHands(true);
				Message("Get In "+dze_action_data.m_Player.ToString());
				dze_vehCommand = dze_action_data.m_Player.StartCommand_Vehicle(m_TargetVehicie, dze_crewIdx, dze_seat);
				if( dze_vehCommand )
				{
					dze_vehCommand.SetVehicleType(m_TargetVehicie.GetAnimInstance());
					dze_action_data.m_Player.GetItemAccessor().HideItemInHands(true);
				}
			//}else{
			//	Message("Am In. Drive it like a bot stole it..");
			//}
		}
	}

	override void OnAbort (BotEventBase e) { super.OnAbort(e); }
	override void OnExit (BotEventBase e) { super.OnExit(e); }
	override void OnUpdate (float dt) { super.OnUpdate(dt); }
};

class DZE_BotHuntVehicle extends BotStateBase
{
	
	
	Man m_Target;
	float m_dtAccumulator = 0.0;

	ref DZE_BotHunt_Tracking_Vehicle m_TrackingVehcile;
	ref DZE_BotHunt_Hunting_Vehicle m_HuntingVehcile;
	ref DZE_BotPerformAction m_GetInVehicle;
	ref DZE_BotNextToVehicle m_NextToVehicle;

	void DZE_BotHuntVehicle (DZE_Bot bot = NULL, BotStateBase parent = NULL)
	{
// setup nested state machine
		m_FSM = new BotFSM(this); // @NOTE: set owner of the submachine fsm
		//DZE_BotHunt_Tracking_Vehicle.
		m_TrackingVehcile = new DZE_BotHunt_Tracking_Vehicle(m_Bot, this);
		m_HuntingVehcile = new DZE_BotHunt_Hunting_Vehicle(m_Bot, this);
		m_NextToVehicle = new DZE_BotNextToVehicle(m_Bot,this);
		m_GetInVehicle = new DZE_BotPerformAction(m_Bot, this, AT_BURY_BODY);
		
// events
		
		BotEventBase __Lost__ = new BotEventHuntedTargetLost;
		BotEventBase __NearVehicle__ = new DZE_BotEventNextToVehicle;
		BotEventBase __InSight__ = new BotEventHuntedTargetInSight;
		BotEventBase __EndOk__ = new BotEventEndOK;
		
		Message("InSight : "+__InSight__.ToString());


// transitions
		//m_FSM.AddTransition(new BotTransition(   m_TrackingVehcile, __InSight__, m_HuntingVehcile));

		m_FSM.AddTransition(new BotTransition(   null,  __Lost__   , m_TrackingVehcile));
		m_FSM.AddTransition(new BotTransition(   m_TrackingVehcile,   __EndOk__, m_GetInVehicle));		
		

		m_FSM.SetInitialState(m_TrackingVehcile);
		debug_veh_ent = GetGame().CreateObject( "TruckBattery", GetGame().GetPlayer().GetPosition() + "10 0 0", false, false );//debug entity
		debug_veh_ent2 = GetGame().CreateObject( "CarBattery", GetGame().GetPlayer().GetPosition() + "10 0 0", false, false );
	}

	void SelectTarget ()
	{
		m_TargetVehicie = DZE_BotSelectNearestTargetVehicle(GetPlayerOwner());
		//m_Target = Man.Cast(GetGame().GetPlayer());
		m_TrackingVehcile.m_Target = m_TargetVehicie;
		m_HuntingVehcile.m_Target = m_TargetVehicie;
		//botDebugPrint("[bot] + " + m_Owner + " hunt SelectTarget Vehicle target=" + m_TargetVehicie);
		/*
		if(m_TargetVehicie){
			m_Owner.SetDirection(m_TargetVehicie.GetDirection());
			m_Owner.GetInputController().OverrideMovementAngle(true,0);
			m_Owner.GetInputController().OverrideMovementSpeed(true, 2);
		}
		*/
	}

	override void OnEntry (BotEventBase e)
	{
		Message("Bot Hunt Vehicle Entered");
		m_dtAccumulator = 0.0;
		SelectTarget();

		super.OnEntry(e);
	}

	override void OnExit (BotEventBase e)
	{
		Message("Bot Hunt Vehicle Exited");
		m_dtAccumulator = 0.0;
		m_TargetVehicie = null;

		super.OnExit(e);
		
	}

	override void OnUpdate (float dt)
	{
		super.OnUpdate(dt);

		m_dtAccumulator += dt;
		
		/*float rescanTime = 3.0;
		if (m_dtAccumulator >= rescanTime)
			if (m_weapon.CanProcessWeaponEvents())
				m_Bot.ProcessEvent(new WeaponEventReloadTimeout(p));*/

		if (m_TargetVehicie == null)
		{
			int acc = m_dtAccumulator;
			if (acc % 5 == 0)
			{
				//Print("Searching...");
				SelectTarget();
			}
		}
	}
};

class DZE_BotHunt_Tracking_Vehicle extends BotStateBase
{

	EntityAI m_Target;
	bool m_TargetInSight = false;
	bool m_TargetLost = false;
	bool m_TrackingVehcile = true;
	vector dirTo;
	int BOTLOOKAHEADDIST = 20;
	AnimalBase a_owner ;
	protected ref DayZPlayerImplementMeleeCombat m_MeleeCombat;
	MeleeCombatHit m_HitMask;
	ActionData action_data;
	PlayerBase m_Player;
	
	override void OnEntry (BotEventBase e)
	{
		super.OnEntry(e);

		m_TargetLost = false;
		m_TargetInSight = false;
		m_TrackingVehcile = false;
		
		
	}

	override void OnAbort (BotEventBase e)
	{
		m_TargetLost = false;
		m_TargetInSight = false;
		m_TrackingVehcile = false;

		super.OnAbort(e);
	}

	override void OnExit (BotEventBase e)
	{
		m_TargetLost = false;
		m_TargetInSight = false;
		m_TrackingVehcile = false;

		super.OnExit(e);
	}

	override void OnUpdate (float dt)
	{
		if (m_TargetVehicie)
		{
			//Print("Updating Vehicle Track");
			m_TrackingVehcile = true;
			vector targetPos = m_TargetVehicie.GetPosition();
			bool targetisalive = m_TargetVehicie.IsAlive();

			// tmp dist check
			float d = vector.Distance(m_TargetVehicie.GetPosition(), GetPlayerOwner().GetPosition());
			if (d < 1.8)
			{
				m_TargetInSight = true;
			}
			else
			{
				m_TargetInSight = false;
			}
			
			//m_TargetInSight = false;//Override to force avoidance
			
			//botDebugPrint("[bot] + " + m_Owner + " hunt Tracking target=" + m_TargetVehicie + " pos=" + targetPos + " TargetInSight="+m_TargetInSight+" Distance="+d);

			if (!m_TargetInSight)//Further than xm away
			{
				
				
				if(m_Owner.IsAnimal()){
					
					a_owner = DayZAnimal.Cast(GetPlayerOwner());

					dirTo = vector.Direction(a_owner.GetPosition(),m_TargetVehicie.GetPosition());
										
					//DayZAnimalInputController.
					DayZAnimalInputController aic = a_owner.GetInputController();
					
					//Message(a_owner.ToString()+" | "+dirTo.ToString()+" | Jump : "+aic.IsJump().ToString()+" | Attack : "+aic.IsAttack().ToString());

					//a_owner.StartCommand_Move();
					
				}else if(m_Owner.IsMan()){
					
//
//TRACK VEHICLE
//						
									
					dirTo = vector.Direction(m_Owner.GetPosition(),m_TargetVehicie.GetPosition());
					m_Owner.SetDirection(dirTo);
					m_Owner.GetInputController().OverrideMovementSpeed(true, 1);
					m_Owner.GetInputController().OverrideMovementAngle(true,1);

				}
				
				
			}
			else if(!dze_getinStarted)
			{//Stop
				dze_getinStarted = true;
				if(m_Owner){
					if( !m_Owner.IsInVehicle() ){
						Message("At Vehcile - Exit Transition.");
						Print("At Vehcile - Exit Transition.");
						m_Owner.GetInputController().OverrideMovementSpeed(true, 0);
						BotEventBase __EndOk__ = new BotEventEndOK;
						OnAbort(__EndOk__);
						m_Bot.ProcessEvent(new BotEventEndOK(m_Owner));
						return;
					}
				}
				
				
				
				//PlayerBase.GetItemAccessor().items
				//m_Player = PlayerBase.Cast(m_Owner);
				//Car.getAnim
				
				
				
//
//USING PURE COMMANDS, CRASHES SERVER ON ANIMATION FINISH
//	
					/*										
					if(m_Owner){
					//PlayerBase.GetCommand_Vehicle(
					PlayerBase dze_player = PlayerBase.Cast(GetGame().GetPlayer());
					//Create Action Target
					ActionTarget dze_actiontrgt = new ActionTarget(m_TargetVehicie, null, -1, vector.Zero, -1);
					//Get Seat
					int dze_componentIndex = dze_actiontrgt.GetComponentIndex();
					int dze_crewIdx = m_TargetVehicie.CrewPositionIndex(dze_componentIndex);
					//Create Action Base
					DZE_ActionBase dze_action_base = new DZE_ActionBase;
					//Create Action Data
					dze_action_base.DZE_SetupAction(m_Owner,dze_actiontrgt,dze_item,dze_action_data);
					
					int dze_seat = m_TargetVehicie.GetSeatAnimationType(dze_crewIdx);
					
					
						//if( !m_Owner.GetCommand_Vehicle().IsGettingIn() && !m_Owner.IsInVehicle() )
						//{
							//m_Owner.GetItemAccessor().HideItemInHands(true);
							Message("Get In "+dze_action_data.m_Player.ToString());
							dze_vehCommand = dze_action_data.m_Player.StartCommand_Vehicle(m_TargetVehicie, dze_crewIdx, dze_seat);
							if( dze_vehCommand )
							{
								dze_vehCommand.SetVehicleType(m_TargetVehicie.GetAnimInstance());
								dze_action_data.m_Player.GetItemAccessor().HideItemInHands(true);
							}
						//}else{
						//	Message("Am In. Drive it like a bot stole it..");
						//}
					}
				*/
					
//
//TRYING TO USE USERINTERACTIONS - GETTING STUCK AT m_state = 25 ??
//												
				//ActionBase.SetupAction(
				//ActionTarget.
				//ActionGetInTransport.St(
				/*
				if( !dze_action_data && !m_Owner.IsInVehicle() )
				{
					PlayerBase dze_player = PlayerBase.Cast(GetGame().GetPlayer());
					ActionTarget dze_actiontrgt = new ActionTarget(m_TargetVehicie, null, -1, vector.Zero, -1);
					DZE_ActionBase dze_action_base = new DZE_ActionBase;
					
					dze_action_base.DZE_SetupAction(m_Owner,dze_actiontrgt,dze_item,dze_action_data);
					
					DZE_ActionGetInTransport dze_abtransport = new DZE_ActionGetInTransport;
					//Message("Setup - Test Conditions.");
					//Message(dze_abtransport.DZE_ActionCondition(m_Owner,dze_actiontrgt,dze_item).ToString());
					if(dze_abtransport.DZE_ActionCondition(m_Owner,dze_actiontrgt,dze_item)){
						dze_action_base.DZE_setState(UA_START,dze_action_data);
						dze_abtransport.DZE_Start(dze_action_data);
						Message("Start Get In | State : "+dze_action_data.m_State.ToString());
					}
				}else{
					if( !dze_action_data.m_Player.GetCommand_Vehicle().IsGettingIn() )
					{
						//dze_action_data.m_Player.GetActionManager().OnActionEnd();
						Message("End Action");
						DZE_End(dze_action_data);
					}
				}
				*/
				
				
			}
			/*
			if(dze_getinStarted && !dze_getinFinished){
				if( !dze_action_data.m_Player.GetCommand_Vehicle().IsGettingIn() )
				{
					dze_getinFinished = true;
					DZE_End(dze_action_data);
				}
			}
			*/



		}
		else
		{
			if (m_TrackingVehcile)
			{
				m_TargetLost = true;
				m_TargetInSight = false;
				m_TrackingVehcile = false;

				GetPlayerOwner().GetInputController().OverrideMovementSpeed(false, 0);
				GetPlayerOwner().GetInputController().OverrideMovementAngle(false, 0);
			}
		}
	}
	
	void DZE_End( DZE_ActionData action_data )
	{
		if( action_data.m_Player )
			action_data.m_Player.GetActionManager().OnActionEnd();
	}
		
	bool lineIntersectsBoundingBox(vector ahead, vector ahead2, Object obj_target, out vector intersectingPos) {
		vector bounds_min_max;
		bool intersects_return = false;
    	obj_target.GetCollisionBox(bounds_min_max);
		vector box_size = GetCollisionBoxSize(bounds_min_max);
		float x_size = box_size[0];
		float y_size = box_size[2];
		vector obj_pos = obj_target.GetPosition();
		
		vector ahead_pos_x = (ahead[0].ToString()+" 0 0").ToVector();
		vector ahead2_pos_x = (ahead2[0].ToString()+" 0 0").ToVector();
		vector obj_pos_x = (obj_pos[0].ToString()+" 0 0").ToVector();
		
		vector ahead_pos_y = ("0 0 "+ahead[2].ToString()).ToVector();
		vector ahead2_pos_y = ("0 0 "+ahead2[2].ToString()).ToVector();
		vector obj_pos_y = ("0 0 "+obj_pos[2].ToString()).ToVector();
		
		
		float ahead_dist_x = vector.Distance(ahead_pos_x,obj_pos_x);
		float ahead2_dist_x = vector.Distance(ahead2_pos_x,obj_pos_x);
		
		float ahead_dist_y = vector.Distance(ahead_pos_y,obj_pos_y);
		float ahead2_dist_y = vector.Distance(ahead2_pos_y,obj_pos_y);
		
		if(ahead_dist_x < (x_size / 2) || ahead_dist_y < (y_size / 2)){
			intersects_return=true;
			intersectingPos = ahead;
		}
		
		if(ahead2_dist_x < (x_size / 2) || ahead2_dist_y < (y_size / 2)){
			intersects_return=true;
			intersectingPos = ahead2;
		}
		//Message ("Intersects : "+intersects_return.ToString());
		return intersects_return;
	}
	
	protected vector GetCollisionBoxSize( vector min_max[2] )
	{
		vector box_size = Vector(1,1,1);
		
		box_size[0] = min_max[1][0] - min_max[0][0];
		box_size[2] = min_max[1][2] - min_max[0][2];
		box_size[1] = min_max[1][1] - min_max[0][1];
		
		return box_size;
	}
};

class DZE_BotHunt_Hunting_Vehicle extends BotStateBase
{
	EntityAI m_Target;

	override void OnEntry (BotEventBase e)
	{
		super.OnEntry(e);
	}

	override void OnAbort (BotEventBase e) { super.OnAbort(e); }

	override void OnExit (BotEventBase e)
	{
		super.OnExit(e);
	}

	override void OnUpdate (float dt)
	{
	}
};


Car DZE_BotSelectNearestTargetVehicle (EntityAI bot)
{
	/*ref array<Man> players = new array<Man>;
	g_Game.GetWorld().GetPlayerList( players );
		
	bool minimal_distance_ok = true;

	float min_dist = 1234567.0;
	int min_index = -1;
	for ( int i = 0; i < players.Count(); i++ )
	{
		float d = vector.Distance(players.Get(i).GetPosition(), bot.GetPosition());
		if ( d < min_dist )
		{
			min_dist = d;
			min_index = i;
		}
	}

	if (min_index != -1)
		return players.Get(min_index);

	return null;*/
	
		vector pos = bot.GetPosition();
		//vector dir = player.GetDirection();

	array<Object> objects = new array<Object>;
	array<CargoBase> proxyCargos = new array<CargoBase>;
	
	GetGame().GetObjectsAtPosition(pos, 100.0, objects, proxyCargos);

	float min_dist = 1234567.0;
	int min_index = -1;
	int c = objects.Count();
	//Message("Nr Bot Objects : "+objects.Count().ToString());
	for (int i = 0; i < c; i++)
	{
		Object o = objects[i];
		if (o == bot || o==debug_veh_ent || o==debug_veh_ent2)
			continue;
		//Print(o.GetParent().ClassName());
		//Object.GetParent().ClassName()
		//if(o.Type().ToString()=="OffroadHatchback"){Message("Car Found : "+o.ToString());}
		float d = vector.Distance(o.GetPosition(), bot.GetPosition());
		if(o.Type().ToString()=="OffroadHatchback"){
			min_dist = d;
			min_index = i;
		}
		/*
		
		if ( d < min_dist )
		{
			min_dist = d;
			min_index = i;
		}
		*/
	}

	if (min_index != -1)
	{
		//Message("Bot Target Found.");
		//botDebugPrint("[bot] + " + bot + " DZE_BotSelectNearestTargetVehicle idx=" + min_index + " dist=" + min_dist + " obj=" + o);
		return Car.Cast( objects.Get(min_index) );//Comment out and return player
	}
	//return Man.Cast(GetGame().GetPlayer());
	return null;
}