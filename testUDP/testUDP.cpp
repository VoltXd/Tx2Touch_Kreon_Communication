#include "PA_CB.hpp"
#include "PA_Sockets.hpp"
#include "PA_ToolBox.hpp"
#include "Errors.hpp"
#include "Positionning_Kreon_From_VAL3.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <thread>

void sendMessage(PA_Communication::UdpSocketManager scktMngr);
void rcvMessage(PA_Communication::UdpSocketManager scktMngr, PA_Communication::CircularBuffer &cbRx);
void readBuffer(PA_Communication::CircularBuffer &cbRx);


bool programEnded = false;

int main()
{
	// Instantiate socket manager
	PA_Communication::UdpSocketManager scktMngr(1980, "192.168.0.254", 1980);
	
	// Initialize socket manager
	while (!scktMngr.IsInitialized())
		scktMngr.Initialize();

	// Initialize circular buffer
	PA_Communication::CircularBuffer cbRx(1024);



	

	//char adress[128];
	//int addrPort = ntohs(addr.sin_port);
	//inet_ntop(AF_INET, &addr.sin_addr, adress, sizeof(adress));
	//std::cout << "Liaison a " << adress << " au port " << addrPort << std::endl << std::endl;

	

	//int serverPort = ntohs(server.sin_port);
	//inet_ntop(AF_INET, &server.sin_addr, adress, sizeof(adress));
	//std::cout << "Connecte a " << adress << " au port " << serverPort << std::endl << std::endl;
	//
	////getchar();
	//
	std::thread sendThread(sendMessage, scktMngr);
	std::thread rcvThread(rcvMessage, scktMngr, std::ref(cbRx));
	std::thread readCBThread(readBuffer, std::ref(cbRx));
	std::cout << "Entrez le texte a envoyer (vide pour quitter)> " << std::endl;
	sendThread.join();
	rcvThread.join(); // A tester sans cette ligne, on a peut-être pas besoin d'attendre de recevoir pour passer à la suite
	readCBThread.join();

	std::cout << std::endl << "Execution terminee." << std::endl << std::endl;
	
	
	return 0;
}

void sendMessage(PA_Communication::UdpSocketManager scktMngr)
{
	while (!programEnded)
	{
		std::string data;
		std::getline(std::cin, data);
		if (data.empty())
		{
			programEnded = true;
			scktMngr.Send("\r\n");
			break;
		}
		data.append("\r\n");

		int ret = scktMngr.Send(data.data());
		if (ret <= 0)
			break;
	}
}

void rcvMessage(PA_Communication::UdpSocketManager scktMngr, PA_Communication::CircularBuffer &cbRx)
{
	while (!programEnded)
	{
		//if (!messageSent)
		//	continue;
		sockaddr_in from;
		char buff[1500] = { 0 };
		//std::cout << "Listening..." << std::endl;
		// Get the message and the sender's infos
		int ret =  scktMngr.Receive(buff, from);
		if (ret <= 0)
			break;
		
		char fromAdress[128];
		inet_ntop(AF_INET, &from.sin_addr, fromAdress, sizeof(fromAdress));
		//std::cout << "Recu : " << buffString << "\r\nde " << fromAdress << ":" << ntohs(from.sin_port) << std::endl << std::endl;

		cbRx.Add(buff, ret);
	}
}

float pose[6];
unsigned char poseIndex = 0;
char poseString[16];
unsigned char poseStringIndex = 0;
void readBuffer(PA_Communication::CircularBuffer &cbRx)
{
	while (!programEnded)
	{
		if (cbRx.IsDataAvailable())
		{
			// Décodage et calcul de la position Kreon (méthode peu fiable)
			poseString[poseStringIndex] = cbRx.Get();
			std::cout << poseString[poseStringIndex++];
			// If end of message
			if (poseString[poseStringIndex - 1] == '\n')
			{
				// string to float & index++
				poseString[poseStringIndex - 1] = '\0';
				pose[poseIndex++] = atof(poseString);

				poseStringIndex = 0;
				// If pose retrieved, calculate kreonPose
				if (poseIndex == 6)
				{
					PA_Positionning::TX2ToKreonTransform(pose);
					std::cout << std::endl << "Kreon Pose : " << std::endl;
					for (int i = 0; i < 6; i++)
						std::cout << pose[i] << std::endl;
					poseIndex = 0;
					std::cout << std::endl << std::endl;
				}
			}
		}
	}
}