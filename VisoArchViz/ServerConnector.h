// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IPAddress.h"
#include "ServerConnector.generated.h"




UCLASS()
class VISOARCHVIZ_API AServerConnector : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AServerConnector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Test")
		int32 ConnectSocket(FString Ip, int32 Port);
	UFUNCTION(BlueprintCallable, Category = "Test")
		void DisconnectSocket(int32 ConnectionId);
	UFUNCTION(BlueprintCallable, Category = "Test")
		bool SendStringData(int32 ConnectionId, FString StringData);
	UFUNCTION(BlueprintCallable, Category = "Test")
		bool SendIntData(int32 ConnectionId, int32 IntData);

	//ThreadTesting
	/*
	UFUNCTION(BlueprintCallable, Category = "Test")
		int32 TryAsync(int32 number1, int32 number2);
		*/
	//Define a map to store all connection ids
	std::map<int32, FSocket*> MySocketMap;
};



/* Thread Testing
//Nested async class
class AsyncTaskConnectSocket : public FRunnable
{
	//Singleton instance, can access the thread at any time via static accessor if it is active.
	static AsyncTaskConnectSocket* Runnable;

	//Thread to run the worker FRunnable on
	FRunnableThread* Thread;

	//The data
	int32 result;

	//Safe stopping for thread
	FThreadSafeCounter StopTaskCounter;

	//Done?
	bool IsFinished() const
	{
		return result;
	}

	//Thread Functions
	
	//Constructor / Destructor
	AsyncTaskConnectSocket(int32 number1, int32 number2);
	virtual ~AsyncTaskConnectSocket();

	//Begin FRunnable interface
	virtual bool Init();
	virtual uint32 Run();
	//End FRunnable interface
	virtual void Stop();

	//Make sure thread has stopped properly
	void EnsureCompletion();

	//Starting and Stopping Thread
	
	/*
		Start the thread and the worker from static (easy access)!
		This code ensures only 1 Prime Number thread will be able to run at a time.
		This function returns a handle to the newly started instance.
	*/

	//static AsyncTaskConnectSocket* MyInit(int32 number1, int32 number2);

	//Shut down the thread, static to easily call it outside context
//	static void Shutdown();

	//static bool IsThreadFinished();
	
//};

