/**@class	BotEventBase
 * @brief	represents event that triggers transition from state to state
 **/
class DZEEventBase
{
	PlayerBase m_Player;
	EntityAI m_Entity;

	void DZEEventBase (PlayerBase p = NULL, EntityAI e = NULL) { m_Entity = e; }
};

//class DZEEventXXX : DZEEventBase { void DZEEventXXX (PlayerBase p = NULL) { } };


class DZEEventStart : DZEEventBase { };
class DZEEventStop : DZEEventBase { };

class DZEEventEndOK : DZEEventBase { };
class DZEEventEndFail : DZEEventBase { };
class DZEEventEndTimeout : DZEEventBase { };
class DZEEventOnItemInHandsChanged : DZEEventBase { };
class DzeEventHuntedTargetLost : DZEEventBase { };
class DzeEventHuntedTargetInSight : DZEEventBase { };
