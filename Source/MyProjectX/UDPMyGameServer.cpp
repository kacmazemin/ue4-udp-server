// Fill out your copyright notice in the Description page of Project Settings.


#include "UDPMyGameServer.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <WS2tcpip.h>
#include "Windows/HideWindowsPlatformTypes.h"


// Sets default values
FUDPMyGameServer::FUDPMyGameServer()
	: bStopThread( false )
	, ThreadInstancePtr( nullptr ) 
{
	ThreadInstancePtr = FRunnableThread::Create(this, TEXT("EMINThreadUDP"));
}

FUDPMyGameServer::~FUDPMyGameServer()
{
	bStopThread = true;

	if(SocketInstance != INVALID_SOCKET)
	{
		closesocket(SocketInstance);
	}
		
	if(ThreadInstancePtr)
	{
		ThreadInstancePtr->Kill();
	}
}

void FUDPMyGameServer::RegisterCommandSink(TFunction<void(const char* command)> function)
{
	CommandSink = function;
}

bool FUDPMyGameServer::Init()
{
	bStopThread = false;
	WSAData data;
	WORD version = MAKEWORD(2, 2);
	int wsOk = WSAStartup(version, &data);
	if(wsOk != NO_ERROR)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't start SOCKET...!d"));
		return false;		
	}
	return true;
}
	
void FUDPMyGameServer::Stop()
{
	
}
	
void FUDPMyGameServer::Exit()
{
	//Cleanup
	WSACleanup();
}

uint32 FUDPMyGameServer::Run()
{
	u_short Port = 2330; //change this later, or read it from asset properties
	SocketInstance = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(SocketInstance == INVALID_SOCKET)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't start SOCKET...!d"));
		return 0;
	}
	
	SOCKADDR_IN serverHint;
	FMemory::Memzero(serverHint);
	serverHint.sin_family = AF_INET;
	serverHint.sin_addr.s_addr = htonl(ADDR_ANY);
	serverHint.sin_port = htons(Port);

	int iResult = bind(SocketInstance, (SOCKADDR*)&serverHint, sizeof(serverHint));
	int iFailureReson = WSAGetLastError();
	if(iResult == SOCKET_ERROR)
	{
		//todo change this line with UNREAL COUT or printString
		UE_LOG(LogTemp, Error, TEXT("Can't bind socket %d"), WSAGetLastError());
		return 0;
	}

	sockaddr_in client;
	int clientLength = sizeof(client);   
	FMemory::Memzero(&client, clientLength);

	char buf[32];

	UE_LOG(LogTemp, Log, TEXT("STARTING EMIN UDP SERVER ON PORT: %d"), Port);
	while(!bStopThread)
	{ //Thread processing starts here
		FMemory::Memzero(buf);

		UE_LOG(LogTemp, Warning, TEXT("Waiting for data..!!"), Port);
		
		int bytesIn = recvfrom(SocketInstance, buf, 32, 0, (sockaddr*)&client, &clientLength);

		if(bytesIn == SOCKET_ERROR)
		{
			UE_LOG(LogTemp, Error, TEXT("Error receiving from client! %d"), WSAGetLastError());
		}
		//char clientIp[256];
		//ZeroMemory(clientIp, 256);

		//inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		if(CommandSink)
		{
			CommandSink(buf);
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Message recieve >>> Message %s") ,ANSI_TO_TCHAR(buf)));
	}
	return 0;
}