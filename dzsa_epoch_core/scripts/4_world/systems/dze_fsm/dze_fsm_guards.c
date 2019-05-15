/**@class	DZEGuardBase
 * @brief	represents guard on a transition from state to state
 **/
class DZEGuardBase
{
	/**@fn		GuardCondition
	 * @brief enable or disable transition based on condition
	 * the guard is a boolean operation executed first and which can prevent the transition from firing by returning false
	 * @return	true if transition is allowed
	 **/
	bool GuardCondition (DZEEventBase e) { return true; }
};

class DZEGuardAnd extends DZEGuardBase
{
	ref DZEGuardBase m_arg0;
	ref DZEGuardBase m_arg1;

	void DZEGuardAnd (DZEGuardBase arg0 = NULL, DZEGuardBase arg1 = NULL) { m_arg0 = arg0; m_arg1 = arg1; }

	override bool GuardCondition (DZEEventBase e)
	{
		bool result = m_arg0.GuardCondition(e) && m_arg1.GuardCondition(e);
		DZEFsmDebugPrint("[dzefsmguard] guard - " + m_arg0.Type() + " && " + m_arg1.Type() + " = " + result);
		return result;
	}
};

class DZEGuardNot extends DZEGuardBase
{
	ref DZEGuardBase m_arg0;

	void DZEGuardNot (DZEGuardBase arg0 = NULL) { m_arg0 = arg0; }

	override bool GuardCondition (DZEEventBase e)
	{
		bool result = !m_arg0.GuardCondition(e);
		DZEFsmDebugPrint("[dzefsmguard] guard - ! " + m_arg0.Type() + " = " + result);
		return result;
	}
};

class DZEGuardOr extends DZEGuardBase
{
	ref DZEGuardBase m_arg0;
	ref DZEGuardBase m_arg1;

	void DZEGuardOr (DZEGuardBase arg0 = NULL, DZEGuardBase arg1 = NULL) { m_arg0 = arg0; m_arg1 = arg1; }

	override bool GuardCondition (DZEEventBase e)
	{
		bool result = m_arg0.GuardCondition(e) || m_arg1.GuardCondition(e);
		DZEFsmDebugPrint("[dzefsmguard] guard - " + m_arg0.Type() + " || " + m_arg1.Type() + " = " + result);
		return result;
	}
};

class DZEGuardHasItemInHands extends HandGuardBase
{
	protected Man m_Player;
	void DZEGuardHasItemInHands (Man p = NULL) { m_Player = p; }

	override bool GuardCondition (HandEventBase e)
	{
		if (m_Player.GetHumanInventory().GetEntityInHands())
		{
			hndDebugPrint("[dzefsmguard] guard - has valid entity in hands");
			return true;
		}

		hndDebugPrint("[dzefsmguard] guard - no entity in hands");
		return false;
	}
};

