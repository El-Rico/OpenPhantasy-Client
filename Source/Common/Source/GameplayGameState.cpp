#include <GameplayGameState.hpp>
#include <GameplayEvents.hpp>
#include <GameStateManager.hpp>
#include <Utility/InputBinder.hpp>
#include <Utility/EventRouter.hpp>
#include <Utility/Events.hpp>
#include <System/Memory.hpp>
#include <Actions.hpp>

namespace OpenPhantasyClient
{
	GameplayGameState::GameplayGameState( ) :
		m_pInputListener( new GameplayInputListener( ) )
	{
		m_pInputBinder = new ZED::Utility::InputBinder( );
		m_pEventRouter = new ZED::Utility::EventRouter( "Gameplay", ZED_TRUE,
			2 );
	}

	GameplayGameState::~GameplayGameState( )
	{
		zedSafeDelete( m_pEventRouter );
		zedSafeDelete( m_pInputBinder );
	}

	ZED_UINT32 GameplayGameState::Enter( )
	{
		ZED::Utility::SetGlobalEventRouter( this->m_pEventRouter );

		m_pInputBinder->BindKey( ZED_KEY_ESCAPE, ACTION_STATE_EXIT );

		GameStateManager::GetInstance( ).SetInputBinder( m_pInputBinder );

		m_pEventRouter->Add( m_pInputListener,
			ZED::Utility::ActionInputEventType );

		m_pInputListener->SetGameplayGameState( this );

		return ZED_OK;
	}

	void GameplayGameState::Render( )
	{
	}

	void GameplayGameState::Update( const ZED_UINT64 p_ElapsedTime )
	{
	}

	ZED_UINT32 GameplayGameState::Exit( )
	{
		GameStateManager::GetInstance( ).Quit( );
		return ZED_OK;
	}

	ZED_CHAR8 *GameplayGameState::GetName( ) const
	{
		return "Gameplay";
	}
}

