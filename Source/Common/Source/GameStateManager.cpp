#include <GameStateManager.hpp>
#include <System/Time.hpp>
#include <System/Memory.hpp>
#include <Events.hpp>
#include <Game/GameState.hpp>
#include <System/Debugger.hpp>
#include <cstring>

namespace OpenPhantasyClient
{
	GameStateManager::GameStateManager( ) :
		m_MinimumWindowWidth( 640 ),
		m_MinimumWindowHeight( 480 )
	{
	}

	GameStateManager::~GameStateManager( )
	{
	}

	ZED_UINT32 GameStateManager::Initialise( )
	{
		if( ZED::Game::GameStateManager::Initialise( ) != ZED_OK )
		{
			zedTrace( "[OpenPhantasyClient::GameStateManager::Initialise] "
				"<ERROR> Unable to initialise the base game state manager\n" );

			return ZED_FAIL;
		}

		if( !m_pRenderer )
		{
			zedTrace( "[OpenPhantasyClient::GameStateManager::Initialise] "
				"<ERROR> Renderer not set\n" );

			return ZED_FAIL;
		}

		m_Running = ZED_TRUE;

		return ZED_OK;
	}

	GameStateManager &GameStateManager::GetInstance( )
	{
		static GameStateManager Instance;

		return Instance;
	}

	ZED_UINT32 GameStateManager::SetWindowDimensions( const ZED_UINT32 p_Width,
		const ZED_UINT32 p_Height )
	{
		if( p_Width < m_MinimumWindowWidth )
		{
			m_WindowWidth = m_MinimumWindowWidth;
		}

		if( p_Height < m_MinimumWindowHeight )
		{
			m_WindowHeight = m_MinimumWindowHeight;
		}
		
		return ZED_OK;
	}

	void GameStateManager::SetMinimumWindowDimensions(
		const ZED_UINT32 p_Width, const ZED_UINT32 p_Height )
	{
		if( p_Width > 0 )
		{
			m_MinimumWindowWidth = p_Width;
		}

		if( p_Height > 0 )
		{
			m_MinimumWindowHeight = p_Height;
		}
	}


	ZED_UINT32 GameStateManager::Update( const ZED_UINT64 p_DeltaTime )
	{
		m_GameStateStack.top( )->Update( p_DeltaTime );

		return ZED_OK;
	}

	ZED_UINT32 GameStateManager::Render( )
	{
		m_GameStateStack.top( )->Render( );

		return ZED_OK;
	}
}

