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
*/

DZE_Bot bot_ower;
EntityAI debug_ent;
EntityAI debug_ent2;

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

class DZE_BotHunt extends BotStateBase
{
	
	Man m_Target;
	float m_dtAccumulator = 0.0;

	ref DZE_BotHunt_Tracking m_Tracking;
	ref DZE_BotHunt_Hunting m_Hunting;

	void DZE_BotHunt (DZE_Bot bot = NULL, BotStateBase parent = NULL)
	{
// setup nested state machine
		m_FSM = new BotFSM(this); // @NOTE: set owner of the submachine fsm

		m_Tracking = new DZE_BotHunt_Tracking(m_Bot, this);
		m_Hunting = new DZE_BotHunt_Hunting(m_Bot, this);
		
// events
		BotEventBase __InSight__ = new BotEventHuntedTargetInSight;
		BotEventBase __Lost__ = new BotEventHuntedTargetLost;

// transitions
		m_FSM.AddTransition(new BotTransition(   m_Tracking, __InSight__, m_Hunting));
		m_FSM.AddTransition(new BotTransition(   m_Hunting,  __Lost__   , m_Tracking));

		m_FSM.SetInitialState(m_Tracking);
		debug_ent = GetGame().CreateObject( "TruckBattery", GetGame().GetPlayer().GetPosition() + "10 0 0", false, false );//debug entity
		debug_ent2 = GetGame().CreateObject( "CarBattery", GetGame().GetPlayer().GetPosition() + "10 0 0", false, false );
	}

	void SelectTarget ()
	{
		m_Target = DZE_BotSelectNearestTarget(GetPlayerOwner());
		//m_Target = Man.Cast(GetGame().GetPlayer());
		m_Tracking.m_Target = m_Target;
		m_Hunting.m_Target = m_Target;
		botDebugPrint("[bot] + " + m_Owner + " hunt SelectTarget target=" + m_Target);
		m_Owner.SetDirection(m_Target.GetDirection());
		m_Owner.GetInputController().OverrideMovementAngle(true,0);
		m_Owner.GetInputController().OverrideMovementSpeed(true, 2);
	}

	void OnEntry (BotEventBase e)
	{
		m_dtAccumulator = 0.0;
		SelectTarget();

		super.OnEntry(e);
	}

	void OnExit (BotEventBase e)
	{
		m_dtAccumulator = 0.0;
		m_Target = null;

		super.OnExit(e);
	}

	void OnUpdate (float dt)
	{
		//super.OnUpdate(dt);

		m_dtAccumulator += dt;
		
		/*float rescanTime = 3.0;
		if (m_dtAccumulator >= rescanTime)
			if (m_weapon.CanProcessWeaponEvents())
				m_Bot.ProcessEvent(new WeaponEventReloadTimeout(p));*/

		if (m_Target == null)
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

class DZE_BotHunt_Tracking extends BotHunt_Tracking
{

	//EntityAI m_Target;
	//bool m_TargetInSight = false;
	//bool m_TargetLost = false;
	//bool m_Tracking = true;
	vector dirTo;
	int BOTLOOKAHEADDIST = 20;
	AnimalBase a_owner ;
	protected ref DayZPlayerImplementMeleeCombat	m_MeleeCombat;
	MeleeCombatHit						m_HitMask;
	
	override void OnEntry (BotEventBase e)
	{
		super.OnEntry(e);

		m_TargetLost = false;
		m_TargetInSight = false;
		m_Tracking = false;
		
		
	}

	override void OnAbort (BotEventBase e)
	{
		m_TargetLost = false;
		m_TargetInSight = false;
		m_Tracking = false;

		super.OnAbort(e);
	}

	override void OnExit (BotEventBase e)
	{
		m_TargetLost = false;
		m_TargetInSight = false;
		m_Tracking = false;

		super.OnExit(e);
	}

	override void OnUpdate (float dt)
	{
		if (m_Target)
		{
			m_Tracking = true;
			vector targetPos = m_Target.GetPosition();
			bool targetisalive = m_Target.IsAlive();

			// tmp dist check
			float d = vector.Distance(m_Target.GetPosition(), GetPlayerOwner().GetPosition());
			if (d < 1.0)
			{
				m_TargetInSight = true;
			}
			else
			{
				m_TargetInSight = false;
			}
			
			m_TargetInSight = false;//Override to force avoidance
			
			botDebugPrint("[bot] + " + m_Owner + " hunt Tracking target=" + m_Target + " pos=" + targetPos + " TargetInSight="+m_TargetInSight+" Distance="+d);

			if (!m_TargetInSight)//Further than 2m away
			{
				
				
				if(m_Owner.IsAnimal()){
					
					a_owner = DayZAnimal.Cast(GetPlayerOwner());

					dirTo = vector.Direction(a_owner.GetPosition(),m_Target.GetPosition());
					
					
					
					//a_owner.SetInvisible(true);//Works but useless..
					
					//Doesn't work on type
					//a_owner.SetYawPitchRoll(dirTo);
					//a_owner.SetAngles(dirTo);
					//a_owner.SetOrientation(dirTo);
					//a_owner.GetLocalPos(dirTo);
					//Doesn't work on type
					
					//DayZAnimalInputController.
					DayZAnimalInputController aic = a_owner.GetInputController();
					
					//Message(a_owner.ToString()+" | "+dirTo.ToString()+" | Jump : "+aic.IsJump().ToString()+" | Attack : "+aic.IsAttack().ToString());

					//a_owner.StartCommand_Move();
					
				}else if(m_Owner.IsMan()){
					
//
//TRACK PLAYER
//						
					//Use manula whilst testing avoidance									
					//dirTo = vector.Direction(m_Owner.GetPosition(),m_Target.GetPosition());
					//m_Owner.SetDirection(dirTo);
					//GetPlayerOwner().SetDirection(dirTo);
				
					//GetPlayerOwner().GetInputController().OverrideMovementSpeed(true, 3);
					//GetPlayerOwner().GetInputController().OverrideMovementAngle(true, 1);
					//GetPlayerOwner().GetInputController().OverrideAimChangeX(true, 1);
					//Message(GetPlayerOwner().GetInputController().GetHeadingAngle().ToString());
					
//
//AVOIDANCE ROUTINE
//
//COLLECT DATA / SETUP
//							
								
					vector bot_velocity = GetVelocity(m_Owner);
					vector bot_dir = GetVelocity(m_Owner);
					//vector bot_dir = m_Owner.GetDirection();
					vector bot_pos = m_Owner.GetPosition();
					
					vector bot_ahead;
					vector bot_ahead2;
					vector avoid_pos;
					vector avoid_force;
					vector avoid_force2;
					bool do_avoid = false;
					float bot_maxseeahead = 4;//See ahead factor
					
					//Message("Bot Velocity : "+bot_velocity.ToString()+" | Dir : "+bot_dir.ToString());
					
					bot_ahead = bot_pos + (bot_velocity.Normalized() * bot_maxseeahead);
					bot_ahead2 = bot_pos + (bot_velocity.Normalized() * bot_maxseeahead) * 0.5;
					debug_ent.SetPosition(bot_ahead);
					debug_ent2.SetPosition(bot_ahead2);
//	
//GET OBJECTS
//															
					vector lookpos = bot_pos + (bot_velocity.Normalized() * bot_maxseeahead) * 0.75;
					float finddist = vector.Distance(bot_ahead,bot_ahead2) * 0.55;
					array<Object> near_objects = new array<Object>;
					array<CargoBase> proxy_cargos = new array<CargoBase>;
					GetGame().GetObjectsAtPosition(lookpos,finddist,near_objects,proxy_cargos);
					
					//Message(near_objects.Count().ToString());
					
					//SetVelocity
					
					float avoid_dist = 9999;
					Object near_obj_out = null;
					float obj_dist = 9999;
					foreach( Object near_obj : near_objects ) 
					{
						if(near_obj != debug_ent && near_obj != debug_ent2 && near_obj.GetType() != "PersonalLight" && near_obj != m_Owner){
							if(vector.Distance(near_obj.GetPosition(),bot_pos) < obj_dist){
								near_obj_out = near_obj;
								obj_dist = vector.Distance(near_obj.GetPosition(),bot_pos);
							}
						}
						
					}
					vector intersectingPos;
					if(near_obj_out != null){
						if(lineIntersectsBoundingBox(bot_ahead, bot_ahead2, near_obj_out, intersectingPos)){
							avoid_pos = near_obj_out.GetPosition();
							do_avoid = true;
						}
					}
					
//
//CALCULATE AVOIDANCE
//															
					if(do_avoid){	
	
						avoid_force = intersectingPos - avoid_pos ;
						avoid_force = avoid_force.Normalized() * 1.1;//Multiply by avoidance force;
						vector avoid_pos_final = avoid_pos + avoid_force;
						debug_ent2.SetPosition(avoid_pos_final);
					
//
//APPLY AVOIDANCE
//				
						float dist_to = vector.Distance(avoid_pos,avoid_pos_final);
													
						vector avoid_dir = vector.Direction(bot_pos,avoid_pos_final);
						m_Owner.SetDirection(avoid_dir);
						SetVelocity(m_Owner,avoid_force);	
						Message(m_Owner.ToString()+" Avoiding : "+avoid_pos_final.ToString()+" | "+near_objects[0].ToString()+" | Dist : "+dist_to.ToString());				
					}
				}
				
				
			}
			else if(targetisalive)
			{
				if(m_Owner.IsMan()){
					
					
//
//ATTACK LOGIC
//
					/*
					HumanCommandMelee hmc = m_Owner.GetCommand_Melee();
					ItemBase pEntityInHands = m_Owner.GetItemInHands();
					InventoryItem itemInHands = InventoryItem.Cast(pEntityInHands);
					EntityAI target;
					target = m_MeleeCombat.GetTargetObject();
					m_MeleeCombat		= m_Owner.GetMeleeCombat();
					m_HitMask = MeleeCombatHit.LIGHT;
					m_MeleeCombat.Update(itemInHands, m_HitMask);
					m_Owner.StartCommand_Melee(target);
					
					if (hmc && hmc.WasHit())
					{
						hmc.ContinueCombo();
						m_Target.ProcessDirectDamage(0,m_Owner,"Head","MeleeDamage","0.5 0.5 0.5",0.1);
					}
						
					//m_MeleeCombat.ProcessHit();
					
					GetPlayerOwner().GetInputController().OverrideMovementSpeed(false, 0);
					GetPlayerOwner().GetInputController().OverrideMovementAngle(false, 0);
					*/
				}else if(m_Owner.IsAnimal()){
					a_owner = DayZAnimal.Cast(GetPlayerOwner());
					dirTo = vector.Direction(a_owner.GetPosition(),m_Target.GetPosition());
					a_owner.SetDirection(dirTo);
				}
				
			}

			/*
			if (GetPlayerOwner().GetInputController().LimitsIsSprintDisabled()){
				
			}else{
				GetPlayerOwner().GetInputController().OverrideMovementSpeed( true, 1 );
			}
			*/

		}
		else
		{
			if (m_Tracking)
			{
				m_TargetLost = true;
				m_TargetInSight = false;
				m_Tracking = false;

				GetPlayerOwner().GetInputController().OverrideMovementSpeed(false, 0);
				GetPlayerOwner().GetInputController().OverrideMovementAngle(false, 0);
			}
		}
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

class DZE_BotHunt_Hunting extends BotHunt_Hunting
{
	//EntityAI m_Target;

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


Man DZE_BotSelectNearestTarget (EntityAI bot)
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
		if (o == bot || o==debug_ent || o==debug_ent2)
			continue;

		float d = vector.Distance(o.GetPosition(), bot.GetPosition());
		if ( d < min_dist )
		{
			min_dist = d;
			min_index = i;
		}
	}

	if (min_index != -1)
	{
		//Message("Bot Target Found.");
		botDebugPrint("[bot] + " + bot + " DZE_BotSelectNearestTarget idx=" + min_index + " dist=" + min_dist + " obj=" + o);
		return Man.Cast( objects.Get(min_index) );//Comment out and return player
	}
	//return Man.Cast(GetGame().GetPlayer());
	return null;
}



