// Lucas Spiker EGP-405: Project 1
/* COA:
 * “We certify that this work is entirely our own.
 * The assessor of this project may reproduce this project and provide copies to other academic staff,
 * and/or communicate a copy of this project to a plagiarism-checking service,
 * which may retain a copy of the project on its database.”
 */


#include <stdio.h>
#include <string.h>
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"  // MessageID

// Possible messages our server/client will look for.
enum GameMessages
{
	ID_GAME_MESSAGES = ID_USER_PACKET_ENUM,
	
	// Handshake exchange
	ID_USERNAME,		// Client responds to connection by sending its username to server
	ID_NEW_CLIENT_JOIN,	// Server broadcasts welcome message to all clients
	ID_UNIQUE_CLIENT,	// Server associates username with ID

	// Messaging exchange
	ID_CHAT_MESSAGE,	// Sent by anyone

	// Misc.
	ID_WHO_REQUEST		// Client request for list of current users
	
};

// The struct that holds the data passed between server and client.
#pragma pack(push, 1)
struct DataPacket
{
	unsigned char typeId;
	char message[255];
};

struct UsernameMessage
{
	unsigned char messageID;
	char username[31];
};

struct ClientNumberMessage
{
	char messageID;
	unsigned int clientNumber;
};

#pragma pack(pop)

int main(void)
{

	char str[512];
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;
	RakNet::Packet *packet;

	unsigned int maxClients;
	unsigned short serverPort;

	printf("(C) or (S)erver?\n");
	fgets(str, 512, stdin);
	printf("Port #?\n");
	scanf("%hi", &serverPort);
	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else {

		printf("Max Clients?\n");
		scanf("%i", &maxClients);

		RakNet::SocketDescriptor sd(serverPort, 0);
		peer->Startup(maxClients, &sd, 1);
		isServer = true;
	}

	if (isServer)
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		peer->SetMaximumIncomingConnections(maxClients);
	}
	else {
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		//fgets(str, 512, stdin);
		//if (str[0] == 0) {
			strcpy(str, "127.0.0.1");
		//}
		printf("Starting the client.\n");
		peer->Connect(str, serverPort, 0, 0);

	}

	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					printf("Our connection request has been accepted.\n");

					// Set up username packet
					UsernameMessage username[1] = { ID_USERNAME, "Lucas123" };
					peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				}
				break;
			case ID_USERNAME:
				{
					// Store username into dictionary
					UsernameMessage* username = (UsernameMessage *)packet->data;
					username->messageID = ID_NEW_CLIENT_JOIN;

					// Broadcast a welcome message
					peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);

					// Send new client their identifier
					ClientNumberMessage clientNumber [1] = { ID_UNIQUE_CLIENT, 0 };
					peer->Send((char*)clientNumber, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				}
				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer) {
					printf("A client has disconnected.\n");
				}
				else {
					printf("We have been disconnected.\n");
				}
				break;
			case ID_CONNECTION_LOST:
				if (isServer) {
					printf("A client lost the connection.\n");
				}
				else {
					printf("Connection lost.\n");
				}
				break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}

	// TODO - Add code body here

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}