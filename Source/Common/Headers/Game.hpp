#ifndef __OPENPHANTASYCLIENT_GAME_HPP__
#define __OPENPHANTASYCLIENT_GAME_HPP__

#include <System/DataTypes.hpp>
#include <System/Window.hpp>
#include <System/Keyboard.hpp>
#include <System/InputManager.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/CanvasDescription.hpp>
#include <Configuration.hpp>
#include <Renderer/Text.hpp>
#include <Renderer/Font.hpp>

namespace OpenPhantasyClient
{
	class Game
	{
	public:
		Game( );
		~Game( );

		ZED_UINT32 Initialise( );
		ZED_UINT32 Execute( );

	private:
		ZED_UINT32 PreInitialise( );
		void Update( const ZED_UINT64 p_MicroSeconds );
		void Render( );

		ZED::System::Window					*m_pWindow;
		ZED::System::InputManager			*m_pInputManager;
		ZED::System::Keyboard				m_Keyboard;
		ZED::Renderer::Renderer				*m_pRenderer;
		ZED::Renderer::CanvasDescription	m_Canvas;
		Configuration						m_GameConfiguration;

		ZED_BOOL			m_Running;
		ZED::Renderer::Font	*m_pFont;
		ZED::Renderer::Text	m_Text;
		ZED_CHAR8			*m_pOSName;
	};
}

#endif // __OPENPHANTASYCLIENT_GAME_HPP__

