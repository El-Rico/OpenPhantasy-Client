#ifndef __OPENPHANTASYCLIENT_GAMEPLAYGAMESTATE_HPP__
#define __OPENPHANTASYCLIENT_GAMEPLAYGAMESTATE_HPP__

#include <System/DataTypes.hpp>
#include <Game/GameState.hpp>

namespace OpenPhantasyClient
{
	class GameplayInputListener;

	class GameplayGameState : public ZED::Game::GameState
	{
	public:
		GameplayGameState( );
		virtual ~GameplayGameState( );

		virtual ZED_UINT32 Enter( );
		virtual void Render( );
		virtual void Update( const ZED_UINT64 p_ElapsedTime );
		virtual ZED_UINT32 Exit( );

		virtual ZED_CHAR8 *GetName( ) const;

	private:
		GameplayInputListener	*m_pInputListener;
	};
}

#endif // __OPENPHANTASYCLIENT_GAMEPLAYGAMESTATE_HPP__

