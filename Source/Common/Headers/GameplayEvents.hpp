#ifndef __OPENPHANTASYCLIENT_GAMEPLAYEVENTS_HPP__
#define __OPENPHANTASYCLIENT_GAMEPLAYEVENTS_HPP__

#include <System/DataTypes.hpp>
#include <Utility/Event.hpp>

namespace OpenPhantasyClient
{
	class GameplayGameState;

	class GameplayInputListener : public ZED::Utility::EventListener
	{
	public:
		GameplayInputListener( );
		virtual ~GameplayInputListener( );

		virtual ZED_BOOL HandleEvent( const ZED::Utility::Event &p_Event );

		virtual ZED_CHAR8 *GetName( ) const;

		ZED_UINT32 SetGameplayGameState(
			GameplayGameState * const &p_pGameplayGameState );

	private:
		GameplayGameState	*m_pGameplayGameState;
	};
}

#endif // __OPENPHANTASYCLIENT_GAMEPLAYEVENTS_HPP__

