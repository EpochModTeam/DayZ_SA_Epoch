ref array<ref DZE_DebugObj> dze_debug_objs = new array<ref DZE_DebugObj>;
ref DZE_DebugObj m_dbgobj;

//Debug Object Settings
ref array<string> dze_objtype = {"Apple","Banana","Orange","Tomato","GreenBellPepper","Zucchini","Potato"};
float dze_dbg_numberOfObjects = 64;
float dbg_height = 0.1;//NB. initial spawn only, player / player z pos decides once updating
float dze_dbg_obj_radius = 3;
float dze_dbg_obj_height = 1.2;
protected const float c_UpdateMS = 16.0;

class DZE_NavMeshTest extends DZEStateBase {
	
	ref DZE_CreateRadius m_createdze_dbg_radius;
	ref DZE_UpdateNavmeshPoints m_updatenavmeshpoints;
	
	
	void DZE_NavMeshTest(DZE_Fsm_Init dze_fsm_init = NULL, DZEStateBase parent = NULL,PlayerBase m_Owner = NULL){
		//Message("Constructed Nav Mesh Test");
		//Setup nested state machine Important  to use m_FSM to nest to parent FSM

		m_FSM = new DZEFsm(this); // @NOTE: set owner of the submachine fsm
		
//Actions		
		m_createdze_dbg_radius = new DZE_CreateRadius(dze_fsm_init,this);
		m_updatenavmeshpoints = new DZE_UpdateNavmeshPoints(dze_fsm_init, this);
		
//Events
		DZEEventBase ___Bgn__ = new DZEEventStart;
		DZEEventBase __Stop__ = new DZEEventStop;
		
//Transitions
		m_FSM.AddTransition(new DzeTransition(   m_createdze_dbg_radius, ___Bgn__, m_updatenavmeshpoints));
		//m_FSM.AddTransition(new DzeTransition(   m_updatenavmeshpoints, __Stop__, NULL));
		
		m_FSM.SetInitialState(m_createdze_dbg_radius);
		//m_FSM.Start();//Do not repeat start here, it creates a new unlinked parent fsm with blank state
		
		
		
		
	}
	
	override void OnEntry (DZEEventBase e) {
		//Message("Entered");	
		super.OnEntry(e);
	}
	
	override void OnExit (DZEEventBase e) { super.OnExit(e); }
	
	void OnUpdate () {
		//Message("Update Navmesh Test");
		m_FSM.GetCurrentState().OnUpdate(c_UpdateMS / 1000.0);
		m_dtAccumulator += 0.1;
		int acc = m_dtAccumulator;
			if (acc % 12 == 0)
			{
				//Message("Main Count : "+m_dtAccumulator.ToString());
			}		
 		//super.OnUpdate(0);
		
	}
	
};

class DZE_UpdateNavmeshPoints : DZEStateBase
{
	protected const float c_TriggerTimeoutMS = 1000.0;
	protected ref DZE_Timer dze_Timer2;
	protected const float dze_checksubfsmdur = 0;
	float m_dtAccumulator = 0.0;
	bool m_TargetInSight = false;
	bool m_TargetLost = false;
	bool m_Tracking = true;
	
	
	void DZE_UpdateNavmeshPoints(DZE_Fsm_Init dze_fsm_init = NULL, DZEStateBase parent = NULL){
		dze_Timer2 = new DZE_Timer();
	}
	
	override void OnEntry (DZEEventBase e)
	{
		
		dze_Timer2.Run(dze_checksubfsmdur, this, "OnUpdate", null, true);
		//Message("Update Nav Mesh Entered from FSM "+m_Owner.ToString()+" | Timer : "+dze_Timer2.IsRunning()+" | "+dze_Timer2.GetTime().ToString()+" | Waiting : "+IsWaitingForActionFinish().ToString()+" | Class : "+this.ToString());
		m_dtAccumulator = 0.0;
		super.OnEntry(e);
	}

	override void OnExit (DZEEventBase e) {
		m_dtAccumulator = 0.0;
		//Message("Update Nav Mesh Exited.");
		super.OnExit(e);
	}
	
	override void OnUpdate(float dt) {//N.B. must call override otherwise CallFunction calls the parent method by default.
		m_dtAccumulator += 0.1;
		int acc = m_dtAccumulator;

		//if(m_dtAccumulatorCheck < m_dtAccumulator){
			//Message("Update Nav Mesh Count : "+m_dtAccumulator.ToString());
			
			DZE_Adjust_DBG_Radius();
			m_dtAccumulatorCheck = m_dtAccumulator + m_dtAccumulatorDuration;
		//}
			
		super.OnUpdate(0);
	}
	
	void DZE_Adjust_DBG_Radius(){

		PlayerBase dze_Player = PlayerBase.Cast(GetGame().GetPlayer());
		private vector plypos = dze_Player.GetPosition();
		private vector posin;
		private vector posradin;
		private float anglein;
		DZE_DebugObj dze_adjustobj;
		vector samplemesh;
		
		//vector.Normalized()
		
		for (int i = 0; i < dze_debug_objs.Count(); i++)      
	    {
			dze_adjustobj = DZE_DebugObj.Cast(dze_debug_objs.Get(i));
			//Message(dze_adjustobj.ToString()+" | "+dze_debug_objs.Count().ToString());
			anglein = i * Math.PI * 2 / dze_dbg_numberOfObjects;
		    posradin = Vector(Math.Cos(anglein), 0, Math.Sin(anglein)) * dze_dbg_obj_radius;
			posin = posradin + plypos;
			bool meshtest = GetGame().GetWorld().GetAIWorld().SampleNavmeshPosition(posin,0.2,samplemesh);
			//Message(meshtest.ToString()+" | "+samplemesh.ToString()+" | "+plypos[1].ToString());
			//Object.SetPosition
			if(dze_adjustobj){
				//Object dze_mveobj = Object.Cast(dze_debug_objs[i]);
				if(meshtest){
					samplemesh[1]=plypos[1]+dze_dbg_obj_height;
					dze_adjustobj.DZE_SetPosition(samplemesh);
				}else{
					posin[1]=plypos[1]+dze_dbg_obj_height;
					dze_adjustobj.DZE_SetPosition(posin);	
				}
			}
			
		}
		
		m_dtAccumulatorCheck = m_dtAccumulator;
	}
};

class DZE_CreateRadius extends DZEStateBase
{
	float m_dtAccumulator = 0.0;
	
	bool m_TargetInSight = false;
	bool m_TargetLost = false;
	bool m_Tracking = true;

	override void OnEntry (DZEEventBase e)
	{
		//Message("Create Radius Entered from FSM "+m_Owner.ToString()+" | "+e.ToString());
		if(createDebugObjs()){
			Print("Processing End Event from Create Radius.");
			m_fsm_init.ProcessEvent(new DZEEventStart(m_Owner));
		}
		m_dtAccumulator = 0.0;
		super.OnEntry(e);
	}

	override void OnExit (DZEEventBase e) {
		
		m_dtAccumulator = 0.0;
		//Message("Create dze_dbg_radius Exited.");
		
		super.OnExit(e);
	}
	
	override void OnUpdate (float dt) {
		super.OnUpdate(dt);
	}
	
	bool createDebugObjs(){
		
		PlayerBase dze_Player = PlayerBase.Cast(GetGame().GetPlayer());
		vector plypos = dze_Player.GetPosition();
		private vector posin;
		private vector posradin;
		private float anglein;
		for (int i = 0; i < dze_dbg_numberOfObjects; i++)      
    	{
			anglein = i * Math.PI * 2 / dze_dbg_numberOfObjects;
		    posradin = Vector(Math.Cos(anglein), dbg_height, Math.Sin(anglein)) * dze_dbg_obj_radius;
			posin = posradin + plypos;	
			DZE_DebugObj dze_dbgins = new DZE_DebugObj(posin,i);
			int dbgins = dze_debug_objs.Insert(dze_dbgins);
			Print(i.ToString()+" | Obj : "+dze_dbgins.ToString()+" | Inserted at : "+dbgins.ToString()+" | From Arr : "+dze_debug_objs[i].ToString());
		}
		//Message("Created "+dze_debug_objs.Count().ToString()+" Objects.");
		return true;
	}
};

class DZE_DebugObj{
	private EntityAI dze_m_debugobj;
	private int dze_obj_index = -1;
	private string dze_obj_type="";
	void DZE_DebugObj(vector pos, int idx){
		//Print("Creating : "+dze_obj_type);
		dze_m_debugobj = GetGame().CreateObject( dze_objtype.Get(0), pos, false, false );
		dze_obj_index = idx;
		Print("DBG OBJ : "+this.ToString());
	}
	void DZE_SetPosition(vector pos){
		dze_m_debugobj.SetPosition(pos);
	}
	void DZE_setObjIdx(int idx){
		dze_obj_index = idx;
	}
	int DZE_getObjIdx(){
		return dze_obj_index;
	}
};



