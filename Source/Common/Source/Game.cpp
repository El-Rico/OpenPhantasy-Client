#include <System/Memory.hpp>
#include <Game.hpp>
#include <GitVersion.hpp>
#include <cstring>
#include <unistd.h>
#include <Renderer/OGL/GLFont.hpp>
#include <System/OS.hpp>
#include <System/Time.hpp>
#include <GameStateManager.hpp>
#include <GameplayGameState.hpp>
#include <Utility/EventRouter.hpp>
#include <Utility/Events.hpp>

namespace OpenPhantasyClient
{
	Game::Game( ) :
		m_pOSName( ZED_NULL )
	{
		m_pWindow = ZED_NULL;
		m_pRenderer = ZED_NULL;
		m_pInputManager = ZED_NULL;

		memset( &m_Canvas, 0, sizeof( m_Canvas ) );
		m_Running = ZED_FALSE;
	}

	Game::~Game( )
	{
		zedSafeDelete( m_pInputManager );
		zedSafeDelete( m_pRenderer );
		zedSafeDelete( m_pWindow );
		zedSafeDeleteArray( m_pOSName );
	}

	ZED_UINT32 Game::Initialise( )
	{
		if( this->PreInitialise( ) != ZED_OK )
		{
			return ZED_FAIL;
		}

		if( m_GameConfiguration.Read( ) != ZED_OK )
		{
			return ZED_FAIL;
		}

		ZED_SINT32 X = 0, Y = 0;
		ZED_UINT32 Width = 0, Height = 0;
		ZED_UINT32 WindowStyle = ZED_WINDOW_STYLE_MINIMISE |
			ZED_WINDOW_STYLE_CLOSE | ZED_WINDOW_STYLE_TITLEBAR |
			ZED_WINDOW_STYLE_MOVE;
		ZED_SINT32 DisplayNumber = 0, ScreenNumber = 0;

		X = m_GameConfiguration.GetXPosition( );
		Y = m_GameConfiguration.GetYPosition( );
		Width = m_GameConfiguration.GetWidth( );
		Height = m_GameConfiguration.GetHeight( );
		DisplayNumber = m_GameConfiguration.GetDisplayNumber( );
		ScreenNumber = m_GameConfiguration.GetScreenNumber( );

		if( m_pWindow->Create( X, Y, Width, Height, DisplayNumber,
			ScreenNumber, WindowStyle ) != ZED_OK )
		{
			zedTrace( "[OpenPhantasyClient::Game::Initialise] <ERROR> "
				"Failed to create window\n" );

			return ZED_FAIL;
		}

		m_Canvas.Width( Width );
		m_Canvas.Height( Height );
		m_Canvas.BackBufferCount( 1 );
		m_Canvas.ColourFormat( ZED_FORMAT_ARGB8 );
		m_Canvas.DepthStencilFormat( ZED_FORMAT_D24S8 );

		if( m_pRenderer->Create( m_Canvas, ( *m_pWindow ) ) != ZED_OK )
		{
			zedTrace( "[OpenPhantasyClient::Game::Initialise] <ERROR> "
				"Failed to create renderer\n" );
			
			return ZED_FAIL;
		}

		float Red = 21.0f / 255.0f;
		float Green = 114.0f / 255.0f;
		float Blue = 179.0f / 255.0f;

		m_pRenderer->ClearColour( Red, Green, Blue );
		m_pRenderer->RenderState( ZED_RENDERSTATE_CULLMODE,
			ZED_CULLMODE_NONE );
		m_pRenderer->RenderState( ZED_RENDERSTATE_DEPTH, ZED_ENABLE );
		ZED::System::WINDOWDATA WindowData = m_pWindow->WindowData( );

		if( m_pInputManager->Initialise( WindowData ) != ZED_OK )
		{
			zedTrace( "[OpenPhantasyClient::Game::Initialise] <ERROR> "
				"Failed to set window data for the new input manager\n" );

			return ZED_FAIL;
		}

		m_pInputManager->AddDevice( &m_Keyboard );

		char Title[ 1024 ];
		memset( Title, '\0', sizeof( Title ) );

		strcat( Title, "Open|Phantasy Client" );

#if defined ZED_BUILD_DEBUG
		strcat( Title, " [DEBUG] | Ver. " );
		strcat( Title, GIT_BUILD_VERSION );
#elif defined ZED_BUILD_PROFILE
		strcat( Title, " [PROFILE] | Ver. " );
		strcat( Title,  GIT_BUILD_VERSION );
#elif defined ZED_BUILD_RELEASE
		strcat( Title, " Ver. " );
		strcat( Title, GIT_BUILD_VERSION );
#else
#error Unknown build configuration
#endif

		m_pWindow->Title( Title );

		m_pFont = new ZED::Renderer::GLFont( m_pRenderer );

		if( m_pFont->Load( "test.zed" ) != ZED_OK )
		{
			return ZED_FAIL;
		}

		m_pFont->SetViewport( 0.0f, 0.0f, 
			static_cast< ZED_FLOAT32 >( m_Canvas.Width( ) ),
			static_cast< ZED_FLOAT32 >( m_Canvas.Height( ) ) );

		m_Text.SetFont( m_pFont );

		if( ZED::System::GetOSFullName( &m_pOSName ) != ZED_OK )
		{
			zedTrace( "[OpenPhantasyClient::Game::Initialise] <ERROR> "
				"Failed to retreive the operating system name\n" );

			return ZED_FAIL;
		}

		if( GameStateManager::GetInstance( ).SetRenderer( m_pRenderer ) !=
			ZED_OK )
		{
			zedTrace( "[OpenPhantasyClient::Game::Initialise] <ERROR> "
				"Could not set the game state manager's Renderer\n" );

			return ZED_FAIL;
		}

		if( GameStateManager::GetInstance( ).Initialise( ) != ZED_OK )
		{
			zedTrace( "[OpenPhantasyClient::Game::Initialise] <ERROR> "
				"Unable to initialise game state manager\n" );

			return ZED_FAIL;
		}

		GameStateManager::GetInstance( ).SetWindowDimensions(
			m_pWindow->GetWidth( ), m_pWindow->GetHeight( ) );

		return ZED_OK;
	}

	ZED_UINT32 Game::Execute( )
	{
		GameplayGameState *pGameplay = new GameplayGameState( );
		GameStateManager::GetInstance( ).RegisterState( pGameplay );

		if( GameStateManager::GetInstance( ).PushState( "Gameplay" ) !=
			ZED_OK )
		{
			zedTrace( "[OpenPhantasyClient::Game::Execute] <ERROR> "
				"Failed to push state \"Gameplay\"\n" );
			return ZED_FAIL;
		}

		ZED_KEYBOARDSTATE PreviousKeyboardState;
		memset( &PreviousKeyboardState, 0, sizeof( PreviousKeyboardState ) );
		ZED_MEMSIZE KeyCount = sizeof( PreviousKeyboardState ) /
			sizeof( PreviousKeyboardState.Key[ 0 ] );

		ZED::Utility::ResolutionChangeEventData ResolutionData;
		ResolutionData.SetResolution( m_pWindow->GetWidth( ),
			m_pWindow->GetHeight( ) );

		ZED::Utility::ResolutionChangeEvent Resolution( &ResolutionData );

		ZED::Utility::SendEvent( Resolution );

		m_Running = ZED_TRUE;

		while( m_Running )
		{
			m_pWindow->Update( );
			m_pInputManager->Update( );
			m_pWindow->FlushEvents( ZED_WINDOW_FLUSH_NONE );

			ZED_KEYBOARDSTATE NewKeyboardState;
			m_Keyboard.State( &NewKeyboardState );

			if( m_pWindow->Resized( ) )
			{
				m_GameConfiguration.SetWidth( m_pWindow->GetWidth( ) );
				m_GameConfiguration.SetHeight( m_pWindow->GetHeight( ) );

				ZED::Utility::ResolutionChangeEventData ResolutionData;
				ResolutionData.SetResolution( m_pWindow->GetWidth( ),
					m_pWindow->GetHeight( ) );
				ZED::Utility::ResolutionChangeEvent Resolution(
					&ResolutionData );

				ZED::Utility::SendEvent( Resolution );
			}

			if( m_pWindow->Moved( ) )
			{
				m_GameConfiguration.SetXPosition( m_pWindow->GetXPosition( ) );
				m_GameConfiguration.SetYPosition( m_pWindow->GetYPosition( ) );
			}

			for( ZED_MEMSIZE i = 0; i < KeyCount; ++i )
			{
				ZED_KEY ZEDKey = static_cast< ZED_KEY >( i );

				if( ( NewKeyboardState.Key[ i ] !=
						PreviousKeyboardState.Key[ i ] ) ||
					( NewKeyboardState.Key[ i ] == 1 &&
						PreviousKeyboardState.Key[ i ] == 1 ) )
				{
					ZED::Utility::KeyboardInputEventData KeyboardData;
					KeyboardData.SetState( ZEDKey,
						NewKeyboardState.Key[ i ] );

					ZED::Utility::KeyboardEvent Keyboard( &KeyboardData );
					ZED::Utility::SendEvent( Keyboard );
				}
			}

			// For emergency use only!
			if( m_Keyboard.IsKeyDown( ZED_KEY_F12 ) )
			{
				m_Running = ZED_FALSE;
			}

			GameStateManager::GetInstance( ).Execute( );

			if( GameStateManager::GetInstance( ).IsRunning( ) == ZED_FALSE )
			{
				m_Running = ZED_FALSE;
			}

			memcpy( &PreviousKeyboardState, &NewKeyboardState,
				sizeof( PreviousKeyboardState ) );
		}

		m_GameConfiguration.Write( );

		return ZED_OK;
	}

	void Game::Update( const ZED_UINT64 p_MicroSeconds )
	{
	}

	void Game::Render( )
	{
		m_pRenderer->BeginScene( ZED_TRUE, ZED_TRUE, ZED_TRUE );
		ZED_FLOAT32 TextWidth, TextHeight;
		m_Text.MeasureString( &TextWidth, &TextHeight,
			"Open|Phantasy [Ver. %s]", GIT_BUILD_VERSION);
		ZED_FLOAT32 TextX =
			static_cast< ZED_FLOAT32 >( m_Canvas.Width( ) ) / 2.0f;
		ZED_FLOAT32 TextY =
			static_cast< ZED_FLOAT32 >( m_Canvas.Height( ) );

		TextX -= ( TextWidth / 2.0f );
		TextY -= ( TextHeight ) * 2.5f;

		m_Text.Render( TextX, TextY, "Open|Phantasy [Ver. %s] //%s",
			GIT_BUILD_VERSION, GIT_TAG_NAME );

		TextX = static_cast< ZED_FLOAT32 >( m_Canvas.Width( ) ) / 2.0f;
		TextY = static_cast< ZED_FLOAT32 >( m_Canvas.Height( ) );

		m_Text.MeasureString( &TextWidth, &TextHeight, "%s", m_pOSName );

		TextX -= ( TextWidth / 2.0f );
		TextY -= ( TextHeight );

		m_Text.Render( TextX, TextY, "%s", m_pOSName );

		ZED::System::BEAT_TIME BeatTime;

		ZED::System::GetBeatTime( BeatTime );

		TextX = 5.0f;
		TextY = static_cast< ZED_FLOAT32 >( m_Canvas.Height( ) ) - 5.0f;
		
		m_Text.Render( TextX, TextY, "@%03d", BeatTime.Beat );

		ZED_COLOUR TextColour = { 0.0f, 1.0f, 0.0f, 1.0f };
		m_pFont->SetForegroundColour( TextColour );

		ZED_FLOAT32 DotWidth = 0.0f;
		
		m_Text.MeasureString( &DotWidth, ZED_NULL, "." );

		m_Text.MeasureString( &TextWidth, &TextHeight, "@%03d",
			BeatTime.Beat );

		TextY = static_cast< ZED_FLOAT32 >( m_Canvas.Height( ) ) - 2.0f;

		for( ZED_UINT32 i = 0; i < BeatTime.CentiBeat; ++i )
		{
			if( ( i % 10 ) == 0 )
			{
				TextColour = { 1.0f, 1.0f, 0.0f, 1.0f };
				m_pFont->SetForegroundColour( TextColour );

				m_Text.Render( TextX, TextY, "." );

				TextColour = { 0.0f, 1.0f, 0.0f, 1.0f };
				m_pFont->SetForegroundColour( TextColour );
			}
			else
			{
				m_Text.Render( TextX, TextY, "." );
			}
			TextX += DotWidth;
		}

		TextColour = { 1.0f, 1.0f, 1.0f, 1.0f };

		m_pFont->SetForegroundColour( TextColour );

		m_pRenderer->EndScene( );
	}
}

