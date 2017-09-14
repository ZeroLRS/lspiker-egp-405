#include "RakNet\RakPeerInterface.h"
#include <Windows.h>
#include <iostream>

// INTRO TO NETWORKED ARCHITECTURE - ASYNC CHAT ROOM APPLICATION

struct KeyboardState
{
	unsigned char keyboard[256];
};

struct ApplicationState
{
	int running;

	KeyboardState key[1];

	// example: in a chatroom
	char chatStream[255];
};

void updateInput(KeyboardState* key)
{
	// - get mouse, keyboard, etc
	// - track these for later

	// example: in a chatroom
	GetKeyboardState(key->keyboard);
}

void updateNetworking(/*param*/)
{
	/*// TMP: make some netowrking state
	//	that stores a peer instance
	// for now a temp
	RakNet::RakPeerInterface *peer;

	// - receive packets until no more packets
	// - while packet exists
	//		- process message
	//		- track data (if any) for later
	RakNet::Packet *packet;
	for (packet = peer->Receive();
		packet;
		peer->DeallocatePacket(packet),
		peer->Receive())
	{
		// HOORAY NETWORKING MESSAGES
	}*/
}

void updateState(ApplicationState* state)
{
	// - account for all previous updates here
	//		- change app wrt time & other updates

	// e.g. chat room
	//	- update chat stream
	
	//  - make sure the chat stream starts "empty"
	state->chatStream[0] = -1;

	int locCounter = 0;
	for (int i = 0; i < 256; i++)
	{
		if (state->key->keyboard[i] != 0)
		{
			state->chatStream[locCounter] = i;
			locCounter++;
		}
	}
	state->chatStream[locCounter] = -1;

}

void render(const ApplicationState* state)
{
	// - "clear buffer"
	// - display state to display medium

	// e.g. chat room
	//	- display chat stream (still rendering)

	for (int i = 0; i < 255 && state->chatStream[i] != -1; i++)
	{
		std::cout << state->chatStream[i];
	}

}

int main()
{
	ApplicationState theState[1];
	theState->running = 1;

	while (1)
	{
		updateInput(theState->key);

		updateNetworking(/**/);

		updateState(theState);

		render(theState);
	}
}
