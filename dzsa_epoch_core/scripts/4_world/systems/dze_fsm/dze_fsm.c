
class DZETransition<Class DZEStateBase, Class DZEEventBase, Class DZEActionBase, Class DZEGuardBase>
{
	ref DZEStateBase m_srcState;
	ref DZEEventBase m_event; // @NOTE: NULL event means "completion transition" in UML speak
	ref DZEStateBase m_dstState; // @NOTE: NULL dst state == UML terminate pseudonode
	ref DZEActionBase m_action;
	ref DZEGuardBase m_guard;

	void DZETransition (DZEStateBase src, DZEEventBase e, DZEStateBase dst, DZEActionBase a = NULL, DZEGuardBase g = NULL)
	{
		m_srcState = src;
		m_event = e;
		m_dstState = dst;
		m_action = a;
		m_guard = g;
	}
};

/**@class		DZEFSM
 * @brief		Day Z Epoch Finite State Machine (Hierarchical)
 **/
class DZEFsm : DZEHFSMBase<DZEStateBase, DZEEventBase, DZEActionBase, DZEGuardBase>
{ 
	/*
	DZEStateBase GetCurrentState ()
	{
		return m_State;
	}
	*/
	/**@fn		GetOwnerState
	 * @return	returns state that is owner of this fsm submachine. returns null if this is a root machine.
	 **/
	DZEStateBase GetOwnerState ()
	{
		return m_OwnerState;
	}

	override ProcessEventResult ProcessEvent (DZEEventBase e)
	{
		if (GetOwnerState())
			Print("[dzefsm] SUB!::" + GetOwnerState().Type().ToString() + "::ProcessEvent(" + e.Type().ToString() + ")");
		else
			Print("[dzefsm] root::ProcessEvent(" + e.Type().ToString() + ")");

		// 1) completion transitions have priority (if any)
		if (m_HasCompletions)
			ProcessCompletionTransitions();

		// 2) submachine then (if any)
		
		if(GetCurrentState()){
			Print("Has FSM ? : "+m_State.HasFSM().ToString()+" | Get Current State : "+m_State.ToString());
			if (m_State.HasFSM())
			{
				ProcessEventResult subfsm_res = m_State.ProcessEvent(e);
	
				switch (subfsm_res)
				{
					case ProcessEventResult.FSM_OK:
					{
						Print("[dzefsm] event processed by sub machine=" + m_State.ToString());
						return subfsm_res; // submachine accepted event
					}
					case ProcessEventResult.FSM_TERMINATED:
					{
						break; // submachine has finished, look for local transitions from exited submachine
					}
					case ProcessEventResult.FSM_NO_TRANSITION:
					{	
						break; // submachine has no transition, look for transitions in local machine
					}
				}
			}
		}

		// 3) local transitions
		int i = FindFirstUnguardedTransition(e);
		if (i == -1)
		{
			Print("[dzefsm] event has no transition: src=" + m_State.ToString() + " e=" + e.Type().ToString());
			return ProcessEventResult.FSM_NO_TRANSITION;
		}

		DZETransition<DZEStateBase, DZEEventBase, DZEActionBase, DZEGuardBase> row = m_Transitions.Get(i);
		ProcessEventResult res;
		if (row.m_dstState != NULL)
		{
			// this is regular transition
			if (row.m_srcState.GetParentState() == row.m_dstState.GetParentState())
				res = LocalTransition(i, e); // transition is within this state machine
			else
				Print("cross-hierarchy transition or misconfigured transition detected!");
				//res = HierarchicTransition(i, e); // transition has to cross hierarchy
		}
		else
		{
			// this is terminating transition
			if (row.m_srcState.GetParentState() == GetOwnerState())
				res = LocalTransition(i, e); // terminating transition is within this state machine
			else
				Print("cross-hierarchy transition or misconfigured transition detected!");
				//res = HierarchicTransition(i, e); // source node crosses hierarchy (terminate lies always within this machine)
		}
		return res;
	}

};

typedef DZETransition<DZEStateBase, DZEEventBase, DZEActionBase, DZEGuardBase> DzeTransition; /// shorthand


