class DZE_Timer extends TimerBase
{
	protected Managed m_target;
	protected string m_function;
	protected ref Param m_params;

	void DZE_Timer(int category = CALL_CATEGORY_SYSTEM)
	{
		//Message("DZE Timer Constructed");
		OnInit(category);
	}

	/**
  \brief Starts timer.
	@param duration function is executed after this time (in seconds).
	@param obj target object on which function will be executed
	@param fn_name name of function (on object "obj") which will be executed
	@param params function arguments see Param for usage, default NULL (no arguments)
	@param loop when true, timer is looped endlessly and function is executed after every loop.
	*/
	void Run(float duration, Class obj, string fn_name, Param params = NULL, bool loop = false)
	{
		//Message("DZE Timer Run");
		m_target = obj;
		m_function = fn_name;
			
		m_params = params;
		OnStart(duration, loop);
	}

	override protected void OnTimer()
	{
		
		if (m_params)
		{
			GetGame().GameScript.CallFunctionParams(m_target, m_function, NULL, m_params);
			m_params = NULL;
		}
		else
		{
			Object returnVal;
			int res = GetGame().GameScript.CallFunction(m_target, m_function, returnVal, 0);
			//Message("Call Function : "+m_target.ToString()+" | "+m_function+" | "+res.ToString()+" | "+returnVal.ToString());
		}
	}
		
	override void Stop()
	{
		super.Stop();
		m_params = NULL;
	}
};


class DZE_TimerTest
{
	//GetGame().GameScript.CallFunction
	protected const float c_TriggerTimeoutMS = 1000.0;
	protected ref DZE_Timer dze_Timer;
	
	void DZE_TimerTest(int category = CALL_CATEGORY_SYSTEM){
		//Message("Construct Timer Test");
		dze_Timer = new DZE_Timer(category);
	}
	
	void OnTrigger(){
		//Message("DZE Timer Trigger."+dze_Timer.GetTime().ToString());
	}
	
	bool IsRunning(){
		return dze_Timer.IsRunning();
	}
	
	void Run(float duration = c_TriggerTimeoutMS / 1000.0, Class obj=null, string fn_name = "OnTrigger", Param params = NULL, bool loop = true)
	{
		if(!obj){obj = this;}
		dze_Timer.Run(duration,obj,fn_name,params,loop);
		//Message("DZE_Timer Run : "+dze_Timer.IsRunning().ToString()+" | "+this.ToString()+" | "+obj.ToString()+" | "+fn_name);
	}
	
	void Stop()
	{
		dze_Timer.Stop();
	}
	
	void Pause()
	{
		dze_Timer.Pause();
	}
	
	void Continue()
	{
		dze_Timer.Continue();
	}
	
	float GetDuration()
	{
		return dze_Timer.GetDuration();
	}
};