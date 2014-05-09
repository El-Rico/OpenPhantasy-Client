#ifndef __OPENPHANTASYCLIENT_EVENTS_HPP__
#define __OPENPHANTASYCLIENT_EVENTS_HPP__

#include <System/DataTypes.hpp>
#include <System/InputTypes.hpp>
#include <Utility/Event.hpp>

namespace OpenPhantasyClient
{
	const ZED::Utility::EventType KeyboardInputEventType( "Keyboard Input" );
	const ZED::Utility::EventType MousePositionInputEventType(
		"Mouse Position Input" );
	const ZED::Utility::EventType ActionInputEventType( "Action Input" );
	const ZED::Utility::EventType ResolutionChangeEventType(
		"Resolution Change" );
}

#endif // __OPENPHANTASYCLIENT_EVENTS_HPP__

