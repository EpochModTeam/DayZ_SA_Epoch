#include "emt/dzsa_epoch_core/scripts/1_core/Constants.c"//To Do - Find a better way to reference the const
modded class ZombieBase extends DayZInfected
{
	
	override bool ChaseAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		// always update target - it can be destroyed		
		m_ActualTarget = pInputController.GetTargetEntity();
		if( m_ActualTarget == NULL )
			return false;
	
		vector targetPos = m_ActualTarget.GetPosition();
		if( !CanAttackToPosition(targetPos) )
			return false;
		
		float targetDist = vector.Distance(targetPos, this.GetPosition());
		int pitch = GetAttackPitch(m_ActualTarget);
		
		m_ActualAttackType = GetDayZInfectedType().ChooseAttack(DayZInfectedAttackGroupType.CHASE, targetDist, pitch);
		if( m_ActualAttackType )
		{
			//DayZPlayerConstants.dz
			if(!dze_disableInfectedAttack){
				StartCommand_Attack(m_ActualTarget, m_ActualAttackType.m_Type, m_ActualAttackType.m_Subtype);
			}
			m_AttackCooldownTime = m_ActualAttackType.m_Cooldown;
			return true;
		}
		
		return false;
	}
	
	override bool FightAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		// always update target - it can be destroyed		
		m_ActualTarget = pInputController.GetTargetEntity();

		if( m_AttackCooldownTime > 0 )
		{
			m_AttackCooldownTime -= pDt;
			return false;
		}
					
		if( m_ActualTarget == NULL )
			return false;

		vector targetPos = m_ActualTarget.GetPosition();
		float targetDist = vector.Distance(targetPos, this.GetPosition());
		float diffY = Math.AbsFloat(GetPosition()[1] - targetPos[1]);
		int pitch = GetAttackPitch(m_ActualTarget);

		if( !CanAttackToPosition(targetPos) )
			return false;

		m_ActualAttackType = GetDayZInfectedType().ChooseAttack(DayZInfectedAttackGroupType.FIGHT, targetDist, pitch);
		if( m_ActualAttackType )
		{
			if(!dze_disableInfectedAttack){
				StartCommand_Attack(m_ActualTarget, m_ActualAttackType.m_Type, m_ActualAttackType.m_Subtype);
			}
			m_AttackCooldownTime = m_ActualAttackType.m_Cooldown;
			return true;
		}

		return false;
	}

};