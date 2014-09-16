#include <Utility/Events.hpp>
#include <GameplayEvents.hpp>
#include <GameplayGameState.hpp>
#include <Arithmetic/Arithmetic.hpp>
#include <Actions.hpp>
#include <System/Debugger.hpp>

namespace OpenPhantasyClient
{
	GameplayInputListener::GameplayInputListener( ) :
		m_pGameplayGameState( ZED_NULL )
	{
	}

	GameplayInputListener::~GameplayInputListener( )
	{
	}

	ZED_BOOL GameplayInputListener::HandleEvent(
		const ZED::Utility::Event &p_Event )
	{
		if( p_Event.Type( ).ID( ) == ZED::Utility::ActionInputEventType.ID( ) )
		{
			ZED::Utility::ActionInputEventData *pActionData =
				p_Event.Data< ZED::Utility::ActionInputEventData >( );

			ZED_UINT32 Action = 0;
			ZED_FLOAT32 ActionValue = 1.0f;

			pActionData->GetAction( Action, ActionValue );

			switch( Action )
			{
				case ACTION_STATE_EXIT:
				{
					zedTrace( "Exiting...\n" );
					if( ZED::Arithmetic::Equal( ActionValue, 1.0f ) )
					{
						m_pGameplayGameState->Exit( );
					}
					return ZED_TRUE;
				}
				default:
				{
					zedTrace( "[OpenPhantasyClient::GameplayInputListener::"
						"HandleEvent] <WARN> Unknown action ID: %d\n",
						Action );
					return ZED_FALSE;
				}
			}
		}

		return ZED_FALSE;
	}

	ZED_CHAR8 *GameplayInputListener::GetName( ) const
	{
		return "Gameplay Input Listener";
	}

	ZED_UINT32 GameplayInputListener::SetGameplayGameState(
		GameplayGameState * const &p_pGameplayGameState )
	{
		if( p_pGameplayGameState )
		{
			m_pGameplayGameState = p_pGameplayGameState;
			return ZED_OK;
		}

		return ZED_FAIL;
	}
}

