// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class MYPROJECTX_API FUDPMyGameServer : public FRunnable
{
	// Sets default values for this actor's properties
	FUDPMyGameServer();

public:
	~FUDPMyGameServer();
	
	static FUDPMyGameServer& GET()
	{
		static FUDPMyGameServer myServerInstane;
		return myServerInstane;
	}
	
	void RegisterCommandSink(TFunction<void(const char* command)> function);

	virtual bool Init() override;
	
	virtual uint32 Run() override;
	
	virtual void Stop() override;
	
	virtual void Exit() override;

private:
	//private Data
	bool 				bStopThread;

	std::function<void(const char* command)> CommandSink;

	FRunnableThread 	*ThreadInstancePtr;

	UINT_PTR			SocketInstance;
};
