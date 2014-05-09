#include <GameStateEvents.hpp>

namespace OpenPhantasyClient
{
	GameStateInputListener::GameStateInputListener( ) :
		m_pInputBinder( ZED_NULL )
	{
	}

	GameStateInputListener::~GameStateInputListener( )
	{
	}
	
	ZED_CHAR8 *GameStateInputListener::Name( ) const
	{
		return "Game State Input Listener";
	}

	ZED_BOOL GameStateInputListener::HandleEvent(
		const ZED::Utility::Event &p_Event )
	{
		return ZED_FALSE;
	}

	ZED_UINT32 GameStateInputListener::SetInputBinder(
		ZED::Utility::InputBinder * const &p_pInputBinder )
	{
		m_pInputBinder = p_pInputBinder;

		return ZED_OK;
	}	
}

