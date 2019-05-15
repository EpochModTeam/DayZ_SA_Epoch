/**@class	DZEStateBase
 * @brief	represent weapon state base
 *
 * Class comes with entry/update/exit hooks that can be overriden in custom states
 *
 * Class is ready for hierarchic composition, i.e. this state having a sub-machine running
 * under hood. If no m_FSM member is configured, class acts as ordinary plain
 * finite machine state.
 **/
class DZEStateBase
{
	PlayerBase m_Owner; /// man that this state belongs to
	DZE_Bot m_Bot; /// bot that this state belongs to
	DZE_Fsm_Init m_fsm_init;//Pointer to init class
	DZEStateBase m_ParentState; /// hierarchical parent state of this state (or null)
	ref DZEFsm m_FSM; /// nested state machine (or null)

	void DZEStateBase (DZE_Fsm_Init dze_fsm_init = NULL, DZEStateBase parent = NULL) { 
		
		m_fsm_init = dze_fsm_init;
		m_Owner = dze_fsm_init.m_Owner;
		m_ParentState = parent;
		//Print("DZE FSM Base State Constrcuted : "+m_fsm_init.ToString());
	}
	
	PlayerBase GetPlayerOwner() { return m_Owner; }

	/**@fn		SetParentState
	 * @brief	allows construction of hierarchical state machine
	 **/
	void SetParentState (DZEStateBase parent) { m_ParentState = parent; }
	/**@fn		GetParentState
	 * @return	state that owns this sub-state (or null if plain state)
	 **/
	DZEStateBase GetParentState () { return m_ParentState; }

	bool HasFSM () { return m_FSM != NULL; }
	DZEFsm GetFSM () { return m_FSM; }

	bool ProcessEvent (DZEEventBase e)
	{
		if (HasFSM())
			return m_FSM.ProcessEvent(e);
		return false;
	}

	/**@fn		AddTransition
	 * @brief	adds transition into m_FSM transition table
	 **/
	void AddTransition (DZETransition<DZEStateBase, DZEEventBase, DZEActionBase, DZEGuardBase> t)
	{
		if (HasFSM())
			m_FSM.AddTransition(t);
		else
			Error("[dzefsmstatebase] adding transition to state without FSM. Configure FSM first.");
	}


	/**@fn		OnEntry
	 * @brief	called upon entry to state
	 * @NOTE	if state has (non-running) sub-machine, it's started on entry
	 * @param[in] e	the event that triggered transition to this state
	 **/
	void OnEntry (DZEEventBase e)
	{
		if (HasFSM() && !m_FSM.IsRunning())
		{
			//DZEFsmDebugPrint("[dzefsmstatebase] { " + this.Type().ToString() + "  Has Sub-FSM! Starting submachine...");
			m_FSM.Start(e);
		}
		else
			//DZEFsmDebugPrint("[dzefsmstatebase] { " + this.Type().ToString());
	}

	/**@fn		OnUpdate
	 * @brief ongoing behavior, performed while being in the state
	 *
	 * @NOTE: this is supposed to be the Do() operation in UML speak
	 **/
	void OnUpdate (float dt)
	{
		if (HasFSM() && m_FSM.IsRunning())
			m_FSM.GetCurrentState().OnUpdate(dt);
	}

	/**@fn		OnAbort
	 * @brief called when abort signal arrives
	 * @param[in] e	the event that triggered abort from this state
	 **/
	void OnAbort (DZEEventBase e)
	{
		if (HasFSM() && m_FSM.IsRunning())
		{
			//DZEFsmDebugPrint("[dzefsmstatebase] OnAbort " + this.Type().ToString() + "  Has Sub-FSM! Aborting submachine...");
			m_FSM.Abort(e);
		}
		//DZEFsmDebugPrint("[dzefsmstatebase] } ABORTED " + this.Type().ToString());
	}

	/**@fn		OnExit
	 * @brief called on exit from state
	 * @param[in] e	the event that triggered transition from this state
	 **/
	void OnExit (DZEEventBase e)
	{
		//DZEFsmDebugPrint("[dzefsmstatebase] } " + this.Type().ToString()+" Exited");
	}

	/**@fn		IsWaitingForActionFinish
	 * @brief	waiting for active animation action/actionType finish
	 * @return true if this state or active substate is waiting for finish signal
	 **/
	bool IsWaitingForActionFinish () { return HasFSM() && m_FSM.IsRunning() && m_FSM.GetCurrentState().IsWaitingForActionFinish(); }

	/**@fn		IsIdle
	 * @brief	idle state does not expect any animation events
	 * @return	true if this state is idle
	 **/
	bool IsIdle () { return false; }

	/**@fn		OnSubMachineChanged
	 * @brief	called when sub-machine has changed its state
	 * @param[in]	src		from state (previous)
	 * @param[in]	dst		to state (current)
	 **/
	void OnSubMachineChanged (DZEStateBase src, DZEStateBase dst) { }
	
	/**@fn		OnStateChanged
	 * @brief	called on current state when state machine has changed its state
	 * @param[in]	src		from state (previous)
	 * @param[in]	dst		to state (current)
	 **/
	void OnStateChanged (DZEStateBase src, DZEStateBase dst) { }
};

class DZEStateIdle : DZEStateBase
{
	void DZEStateIdle (DZE_Fsm_Init dze_fsm_init = NULL, DZEStateBase parent = NULL) { m_fsm_init = dze_fsm_init; m_Owner = dze_fsm_init.m_Owner; m_ParentState = parent; }

	override bool IsIdle () { return true; }
};
