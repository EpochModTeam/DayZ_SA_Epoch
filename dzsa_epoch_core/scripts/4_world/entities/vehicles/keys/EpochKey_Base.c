class EpochKey_Base extends ItemBase
{
	private int m_dze_keyCode = 0;

	void EpochKey_Base()
	{
		m_dze_keyCode = Math.RandomInt(0, 9999);
		RegisterNetSyncVariableInt("m_dze_keyCode");
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		//write data
		ctx.Write(m_dze_keyCode);
		//Print("DEBUG keycode save :" + m_dze_keyCode);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		//combination
		int keycode;
		if (!ctx.Read(keycode))
		{
			keycode = 0;		//set default
		}
		SetCode(keycode);
		Print("DEBUG keycode load :" + keycode);

		return true;
	}

	bool IsMatchingLock(int inputnumber)
	{
		if (inputnumber == m_dze_keyCode)
		{
			return true;
		}
		else 
		{
			return false;
		}
		
	}

	void SetCode(int number)
	{
		m_dze_keyCode = number;
		
	}

};