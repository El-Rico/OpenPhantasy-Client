#ifndef __OPENPHANTASYCLIENT_GAMESTATEMANAGER_HPP__
#define __OPENPHANTASYCLIENT_GAMESTATEMANAGER_HPP__

#include <Game/GameStateManager.hpp>

namespace ZED
{
	namespace Game
	{
		class GameState;
		class GameStateInputListener;
	}
}

namespace OpenPhantasyClient
{
	class GameStateManager : public ZED::Game::GameStateManager
	{
	public:
		GameStateManager( );
		virtual ~GameStateManager( );

		virtual ZED_UINT32 Initialise( );

		static GameStateManager &GetInstance( );

		ZED_UINT32 SetWindowDimensions( const ZED_UINT32 p_Width,
			const ZED_UINT32 p_Height );

		void SetMinimumWindowDimensions( const ZED_UINT32 p_Width,
			const ZED_UINT32 p_Height );
	
	protected:
		virtual ZED_UINT32 Update( const ZED_UINT64 p_DeltaTime );
		virtual ZED_UINT32 Render( );

	private:
		ZED_UINT32	m_WindowWidth;
		ZED_UINT32	m_WindowHeight;
		ZED_UINT32	m_MinimumWindowWidth;
		ZED_UINT32	m_MinimumWindowHeight;
	};
}

#endif // __OPENPHANTASYCLIENT_GAMESTATEMANAGER_HPP__

