// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerConnector.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include <stdio.h> 
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Runtime/Networking/Public/Networking.h"
#include "CString.h"
#include "string"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Components/StaticMeshComponent.h"
#include <Platform.h>





// Sets default values
AServerConnector::AServerConnector()
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AServerConnector::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AServerConnector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 AServerConnector::ConnectSocket(FString Ip, int32 Port)
{
	FSocket * Socket;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Connecting to socket... "+Ip));
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("Socket"), false);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	FIPv4Address ParsedAddress;
	
	FIPv4Address::Parse(Ip, ParsedAddress);

	addr->SetIp(ParsedAddress.Value);
	addr->SetPort(Port);

	bool connected = Socket->Connect(*addr);

	if (connected)
	{ 
		MySocketMap[MySocketMap.size()] = Socket;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Connected successfully!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to connect to server!"));
	}
	return MySocketMap.size()-1;
}

void AServerConnector::DisconnectSocket(int32 Id)
{
	//Check if socket exists in that Id
	FString Message;
	if (MySocketMap.find(Id) == MySocketMap.end())
	{
		 Message = "There is no socket with id: ";
		//Message.Append(Message);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message.Append(FString::FromInt(Id)));
	}
	else
	{ 
		switch (MySocketMap[Id]->GetConnectionState())
		{
		case SCS_NotConnected:
			Message = "Socket not connected: ";
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, Message.Append(FString::FromInt(Id)));
			break;
		case SCS_Connected:
			Message = "Socket closed: ";
			MySocketMap[Id]->Close();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Message.Append(FString::FromInt(Id)));
			MySocketMap.erase(Id);
			break;
		case SCS_ConnectionError:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Socket connection error!"));
			break;
		default:
			break;
		}
	}
	
}

bool AServerConnector::SendStringData(int32 ConnectionId, FString StringData)
{
	bool successful;
	//Check if socket exists on that Id
	FString Message;
	if (MySocketMap.find(ConnectionId) == MySocketMap.end())
	{
		Message = "There is no socket with id: ";
		//Message.Append(Message);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message.Append(FString::FromInt(ConnectionId)));
		successful = false;
	}
	else
	{
		//Serialize Data
		FString serialized = StringData;
		TCHAR *serializedChar = serialized.GetCharArray().GetData();
		int32 size = FCString::Strlen(serializedChar);
		int32 sent = 0;
		//Check cases
		switch (MySocketMap[ConnectionId]->GetConnectionState())
		{
		case SCS_NotConnected:
			Message = "Socket not connected: ";
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, Message.Append(FString::FromInt(ConnectionId)));
			successful = false;
			break;
		case SCS_Connected:
			//Actually send
			successful = MySocketMap[ConnectionId]->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent);

			if (successful)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("String sent!"));
			}
			else
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Sending failed!"));
			return successful;

			break;
		case SCS_ConnectionError:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Socket connection error!"));
			successful = false;
			break;
		default:
			break;
		}
	}

	return successful;

}

bool AServerConnector::SendIntData(int32 ConnectionId, int32 Int32Data)
{
	bool successful;
	//Check if socket exists on that Id
	FString Message;
	if (MySocketMap.find(ConnectionId) == MySocketMap.end())
	{
		Message = "There is no socket with id: ";
		//Message.Append(Message);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message.Append(FString::FromInt(ConnectionId)));
		successful = false;
	}
	else
	{
		//Serialize Data
		FString serialized = FString::FromInt(Int32Data);
		TCHAR *serializedChar = serialized.GetCharArray().GetData();
		int32 size = FCString::Strlen(serializedChar);
		int32 sent = 0;
		//Check cases
		switch (MySocketMap[ConnectionId]->GetConnectionState())
		{
		case SCS_NotConnected:
			Message = "Socket not connected: ";
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, Message.Append(FString::FromInt(ConnectionId)));
			successful = false;
			break;
		case SCS_Connected:
			//Actually send
			successful = MySocketMap[ConnectionId]->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent);

			if (successful)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Int sent!"));
			}
			else
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Sending failed!"));
			return successful;

			break;
		case SCS_ConnectionError:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Socket connection error!"));
			successful = false;
			break;
		default:
			break;
		}
	}

	return successful;

}

/*
//Thread
int32 AServerConnector::TryAsync(int32 number1, int32 number2)
{
	AsyncTaskConnectSocket* AsyncTaskConnectSocket::Runnable = NULL;
	AsyncTaskConnectSocket::AsyncTaskConnectSocket(int32 number1, int32 number2)
	{
		int32 nr1 = number1;
		int32 nr2 = number2;

		Thread = FRunnableThread::Create(this, TEXT("MyThread"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
	}

	AsyncTaskConnectSocket::~FAsyncTaskConnectSocket()
	{
		delete Thread;
		Thread = NULL;
	}

	//Init
	bool AsyncTaskConnectSocket::Init()
	{
		//Init the Data
		int32 result;
		return true;
	}

	//Run
	uint32 AsyncTaskConnectSocket::Run()
	{
		while (StopTaskCounter.GetValue() == 0 && !IsFinished())
		{
			result = number1 * number2;
		}

		return 0;
	}
	//Stop
	void AsyncTaskConnectSocket::Stop()
	{
		StopTaskCounter.Increment();
	}

	AsyncTaskConnectSocket* AsyncTaskConnectSocket::MyInit(int32 number1, int32 number2)
	{
		//Create new instance of thread if it does not exist
		//and the platform supports multi threading
		if (!Runnable && FPlatformProcess::SupportsMultithreading())
		{
			Runnable = new AsyncTaskConnectSocket(number1, number2);
		}
		return Runnable;
	}

	void AsyncTaskConnectSocket::EnsureCompletion()
	{
		Stop();
		Thread->WaitForCompletion();
	}

	void AsyncTaskConnectSocket::Shutdown()
	{
		if (Runnable)
		{
			Runnable->EnsureCompletion();
			delete Runnable;
			Runnable = NULL;
		}
	}

	bool AsyncTaskConnectSocket::IsThreadFinished()
	{
		if (Runnable) return Runnable->IsFinished();
		return true;
	}
}
*/
