// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	//Define a map to store all connection ids
	std::map<int32, FSocket*> MySocketMap;
};
