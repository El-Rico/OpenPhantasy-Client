#include <GameStateManager.hpp>
#include <System/Time.hpp>
#include <System/Memory.hpp>
#include <GameState.hpp>
#include <GameStateEvents.hpp>
#include <Events.hpp>
#include <cstring>

namespace OpenPhantasyClient
{
	GameStateManager::GameStateManager( ) :
		m_Running( ZED_FALSE ),
		m_StartTime( ZED::System::GetTimeMiS( ) ),
		m_TotalElapsedTime( 0ULL ),
		m_pRenderer( ZED_NULL ),
		m_pInputListener( ZED_NULL )
	{
	}

	GameStateManager::~GameStateManager( )
	{
	}

	ZED_UINT32 GameStateManager::Initialise( )
	{
		m_Running = ZED_TRUE;

		return ZED_OK;
	}

	ZED_UINT32 GameStateManager::Execute( )
	{
		if( m_GameStateStack.empty( ) )
		{
			this->Quit( );

			return ZED_FAIL;
		}

		ZED_UINT64 CurrentWallTime = ZED::System::GetTimeMiS( );
		ZED_UINT64 TimeDifference = CurrentWallTime - m_StartTime;
		m_TotalElapsedTime += TimeDifference;
		m_StartTime = CurrentWallTime;

		m_GameStateStack.top( )->Update( TimeDifference );

		m_pRenderer->BeginScene( ZED_TRUE, ZED_TRUE, ZED_TRUE );
		m_GameStateStack.top( )->Render( );
		m_pRenderer->EndScene( );

		return ZED_OK;
	}

	ZED_UINT32 GameStateManager::RegisterState(
		GameState * const &p_pGameState )
	{
		m_GameStateRegistry.insert( p_pGameState );

		return ZED_OK;
	}

	ZED_UINT32 GameStateManager::GetNumberOfStates( ) const
	{
		return m_GameStateRegistry.size( );
	}

	ZED_CHAR8 *GameStateManager::GetStateName( const ZED_UINT32 p_Index ) const
	{
		if( p_Index > m_GameStateRegistry.size( ) )
		{
			return "\0";
		}

		GameStateSet::const_iterator RegistryIterator =
			m_GameStateRegistry.begin( );

		for( ZED_UINT32 i = 0; i < p_Index; ++i )
		{
			++RegistryIterator;
		}

		return ( *RegistryIterator )->GetName( );
	}

	ZED_UINT32 GameStateManager::ChangeState(
		const ZED_CHAR8 *p_pGameStateName )
	{
		if( this->IsGameStateNameValid( p_pGameStateName ) == ZED_FALSE )
		{
			return ZED_FAIL;
		}

		this->PopState( );
		this->PushState( p_pGameStateName );

		return ZED_OK;
	}

	ZED_UINT32 GameStateManager::PushState( const ZED_CHAR8 *p_pGameStateName )
	{
		if( this->IsGameStateNameValid( p_pGameStateName ) == ZED_FALSE )
		{
			return ZED_FAIL;
		}

		GameStateSet::const_iterator RegistryIterator =
			m_GameStateRegistry.begin( );

		for( ; RegistryIterator != m_GameStateRegistry.end( );
			++RegistryIterator )
		{
			if( strcmp( ( *RegistryIterator )->GetName( ),
				p_pGameStateName ) == 0 )
			{
				break;
			}
		}

		m_GameStateStack.push( ( *RegistryIterator ) );

		if( m_GameStateStack.top( )->Enter( ) != ZED_OK )
		{
			this->PopState( );
			return ZED_FAIL;
		}

		m_GameStateStack.top( )->GetEventRouter( )->Add( m_pInputListener,
			KeyboardInputEventType );
		m_GameStateStack.top( )->GetEventRouter( )->Add( m_pInputListener,
			MousePositionInputEventType );
		m_GameStateStack.top( )->GetEventRouter( )->Add( m_pInputListener,
			ResolutionChangeEventType );

		return ZED_OK;
	}

	ZED_UINT32 GameStateManager::PopState( )
	{
		if( !m_GameStateStack.empty( ) )
		{
			m_GameStateStack.pop( );

			return ZED_OK;
		}

		return ZED_FAIL;
	}

	ZED_UINT32 GameStateManager::SetInputBinder(
		ZED::Utility::InputBinder * const &p_pInputBinder )
	{
		if( p_pInputBinder )
		{
			m_pInputListener->SetInputBinder( p_pInputBinder );
			return ZED_OK;
		}

		return ZED_FAIL;
	}

	void GameStateManager::Quit( )
	{
		while( !m_GameStateStack.empty( ) )
		{
			this->PopState( );
		}

		m_Running = ZED_FALSE;
	}

	ZED_BOOL GameStateManager::Running( ) const
	{
		return m_Running;
	}

	ZED::Renderer::Renderer * const GameStateManager::GetRenderer( )
	{
		return m_pRenderer;
	}

	ZED_UINT32 GameStateManager::SetRenderer(
		ZED::Renderer::Renderer * const &p_pRenderer )
	{
		if( p_pRenderer )
		{
			m_pRenderer = p_pRenderer;

			return ZED_OK;
		}

		return ZED_FAIL;
	}

	ZED_FLOAT64 GameStateManager::GetTotalElapsedTimeAsFloat64( ) const
	{
		return static_cast< ZED_FLOAT64 >( m_TotalElapsedTime ) *
			OneOverMillion;
	}

	ZED_UINT64 GameStateManager::GetTotalElapsedTimeAsUInt64( ) const
	{
		return m_TotalElapsedTime;
	}

	GameStateManager &GameStateManager::GetInstance( )
	{
		static GameStateManager Instance;

		return Instance;
	}


	ZED_BOOL GameStateManager::IsGameStateNameValid(
		const ZED_CHAR8 *p_pGameStateName ) const
	{
		GameStateSet::const_iterator RegistryIterator =
			m_GameStateRegistry.begin( );

		for( ; RegistryIterator != m_GameStateRegistry.end( );
			++RegistryIterator )
		{
			if( strcmp( ( *RegistryIterator )->GetName( ),
				p_pGameStateName ) == 0 )
			{
				return ZED_TRUE;
			}
		}

		return ZED_FALSE;
	}
}

