modded class Barrel_ColorBase : Container_Base
{
	private bool m_dze_IsLocked = false;

	void Barrel_ColorBase()
	{
		RegisterNetSyncVariableBool("m_dze_IsLocked");
	}
	override bool IsLocked()
	{
		return m_dze_IsLocked;
	}

	void SetLock(bool state)
	{
		m_dze_IsLocked = state;
	}

	override void Lock(float actiontime)
	{
		SetLock(true);
	}

	override void Unlock()
	{
		SetLock(false);
	}

};
