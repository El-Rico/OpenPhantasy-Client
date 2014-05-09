#include <GameState.hpp>

namespace OpenPhantasyClient
{
	GameState::~GameState( )
	{
	}

	ZED::Utility::EventRouter * const GameState::GetEventRouter( )
	{
		return m_pEventRouter;
	}
}

