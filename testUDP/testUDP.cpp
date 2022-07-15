#include "PA_CB.hpp"
#include "PA_Protocol.hpp"
#include "PA_Sockets.hpp"
#include "PA_ToolBox.hpp"
#include "Errors.hpp"
#include "Positionning_Kreon_From_VAL3.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <thread>

void initSocket();
void sendMessage();
void rcvMessage(PA_Communication::CircularBuffer &cbRx);
void readBuffer(PA_Communication::CircularBuffer &cbRx);
void executionTimeStatistics();

bool programEnded = false;
bool taskFinished = false;
const unsigned int nTest = 500;
float testsDuration[nTest];
PA_Communication::UdpSocketManager scktMngr(1980, "192.168.0.254", 1980);

int main()
{
	// Initialize socket manager
	initSocket();

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
	std::thread sendThread(sendMessage);
	std::thread rcvThread(rcvMessage, std::ref(cbRx));
	std::thread readCBThread(readBuffer, std::ref(cbRx));
	std::cout << "Entrez le texte a envoyer (vide pour quitter)> " << std::endl;
	std::cout << "\n'a' : Debut Acq\n'e' : Fin Acq\n'p' : Demande Position\n's' : Statistique temps execution\n" << std::endl;
	
	sendThread.join();
	rcvThread.join(); // A tester sans cette ligne, on a peut-être pas besoin d'attendre de recevoir pour passer à la suite
	readCBThread.join();

	std::cout << std::endl << "Execution terminee." << std::endl << std::endl;
	
	return 0;
}

void initSocket()
{
	while (!scktMngr.IsInitialized())
		scktMngr.Initialize();
}

void sendMessage()
{
	while (!programEnded)
	{
		std::string data;
		std::getline(std::cin, data);
		if (data.empty())
		{
			programEnded = true;
			scktMngr.Send("\n");
			break;
		}

		bool statsQuery = false;
		switch (data[0])
		{
		case 'a':
			data[0] = 0xFE;
			break;

		case 'e':
			data[0] = 0xEF;
			break;

		case 'p':
			data[0] = 0x01;
			break;

		case 's':
			executionTimeStatistics();
			statsQuery = true;
			break;

		default:
			data[0] = 0xAA;
			break;
		}

		data.append("\n");
		if (!statsQuery)
		{
			int ret = scktMngr.Send(data.data());
			if (ret <= 0)
			{
				scktMngr.Close();
				initSocket();
				continue;
			}
		}
	}
}

void rcvMessage(PA_Communication::CircularBuffer &cbRx)
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
		{
			scktMngr.Close();
			initSocket();
			continue;
		}
		
		char fromAdress[128];
		inet_ntop(AF_INET, &from.sin_addr, fromAdress, sizeof(fromAdress));
		//std::cout << "Recu : " << buffString << "\r\nde " << fromAdress << ":" << ntohs(from.sin_port) << std::endl << std::endl;

		cbRx.Add(buff, ret);
	}
}

float pose[6];
void readBuffer(PA_Communication::CircularBuffer &cbRx)
{
	while (!programEnded)
	{
		if (cbRx.IsDataAvailable())
		{
			if (PA_Protocol::decodeMessage(cbRx.Get()))
			{
				PA_Protocol::retrievePose(pose);
				for (int i = 0; i < 6; i++)
					std::cout << pose[i] << std::endl;

				PA_Positionning::tx2ToKreonTransform(pose);
				PA_ToolBox::toc();
				taskFinished = true;

				std::cout << std::endl << "Kreon Pose : " << std::endl;
				for (int i = 0; i < 6; i++)
					std::cout << pose[i] << std::endl;
				std::cout << std::endl << std::endl;
			}
		}
	}
}


void executionTimeStatistics()
{
	for (int i = 0; i < nTest; i++)
	{
		taskFinished = false;
		// Début commande à mesurer
		PA_ToolBox::tic();
		int ret = scktMngr.Send(new char[2]{ (char)0x01, '\n' });
		if (ret <= 0)
		{
			scktMngr.Close();
			initSocket();
			continue;
		}

		while (!taskFinished);
		// Fin commande à mesurer
		testsDuration[i] = PA_ToolBox::getTicTocDuration();
	}

	float mean = PA_ToolBox::getMean(testsDuration, nTest);
	float standartDeviation = PA_ToolBox::getStandartDeviation(testsDuration, nTest);

	std::cout << "Temps execution total : \nMoyenne : " << mean << "\nSigma : " << standartDeviation << std::endl;
}