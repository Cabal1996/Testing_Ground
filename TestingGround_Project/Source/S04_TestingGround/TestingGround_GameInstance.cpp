// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGround_GameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSubsystemUtils.h"

UTestingGround_GameInstance::UTestingGround_GameInstance()
{
	GameLevel = "/Game/Levels/FirstPersonExampleMap";
	MenuLevel = "/Game/Levels/MainMenu";

	SessionName = NAME_GameSession;
}

void UTestingGround_GameInstance::Init()
{
	Super::Init();

	//Online subsystem initialization
	OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO OSS!!!"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Find: %s"), *OnlineSubsystem->GetSubsystemName().ToString());


		//Session interface initialization

		SessionInterface = Online::GetSessionInterface(GetWorld());
		//SessionInterface = OnlineSubsystem->GetSessionInterface();




		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Session interface active"));

			//Event bindings
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UTestingGround_GameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UTestingGround_GameInstance::OnDestroySessionComplete);

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO Session interface!!!!"));
		}
	}
}

void UTestingGround_GameInstance::HostGame()
{
	MenuResponce.Broadcast(EResponceType::VE_Loading, TEXT(""));

	CreateSession();

	
	/*
	//Server Travel to Lobby Map
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	World->ServerTravel(FString::Printf(TEXT("%s?listen"), *GameLevel));
	*/
}

void UTestingGround_GameInstance::JoinGame(FString ServerIP)
{
	this->ServerIP = ServerIP;
	bIsJoining = true;

	MenuResponce.Broadcast(EResponceType::VE_Loading, TEXT(""));

	CreateSession();

	

}

void UTestingGround_GameInstance::CreateSession()
{
	//Check if any Internet Online Subsystem available
	if (!ensure(OnlineSubsystem != nullptr))
	{
		MenuResponce.Broadcast(EResponceType::VE_EndLoading, TEXT(""));
		MenuResponce.Broadcast(EResponceType::VE_Message, TEXT("Online subsystem unavailable"));
		return;
	}

	if (SessionInterface->GetNamedSession(SessionName) != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cabal1996"))
		bSessionNeed = true;
		SessionInterface->DestroySession(SessionName);
	}
	else
	{
		if (SessionInterface.IsValid())
		{
			//Applying Server settings to Session settings (NEED DEEP IMPLEMENTATION)
			FOnlineSessionSettings SessionSettings;

			SessionSettings.bIsLANMatch = false;
			SessionSettings.bShouldAdvertise = true; //Make session visible
			SessionSettings.bUsesPresence = true; //Enabled steam lobby on server side

			SessionSettings.bAllowJoinViaPresence = true;
			SessionSettings.bAllowJoinInProgress = true;

			SessionSettings.NumPublicConnections = 4; // Maximum Number of Players

			SessionSettings.bIsLANMatch = true;
			//SessionSettings.Set(TEXT("ServerName"), TEXT("MYServer"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing); //Custom information


			//Create session with Specified settings
			SessionInterface->CreateSession(0, SessionName, SessionSettings);
		}
	}
}

void UTestingGround_GameInstance::OnCreateSessionComplete(FName SessionName, bool IsSuccess)
{
	if (IsSuccess == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session %s"), *SessionName.ToString())
		MenuResponce.Broadcast(EResponceType::VE_EndLoading, TEXT(""));
		MenuResponce.Broadcast(EResponceType::VE_Message, TEXT("Could not create session"));
	}
	else
	{
		if (bIsJoining)
		{
			APlayerController* PlayerController = GetFirstLocalPlayerController();



			if (PlayerController != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Join success Start traveling"))
					MenuResponce.Broadcast(EResponceType::VE_EndLoading, TEXT(""));
				bIsJoining = false;
				PlayerController->ClientTravel(ServerIP, ETravelType::TRAVEL_Absolute);

			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("URL not found Failed"))
			}

			bIsJoining = false;
		}
		else
		{
			MenuResponce.Broadcast(EResponceType::VE_EndLoading, TEXT(""));
			//Server Travel to Lobby Map
			UWorld* World = GetWorld();
			if (!ensure(World != nullptr)) return;
			World->ServerTravel(FString::Printf(TEXT("%s?listen"), *GameLevel));
		}
	}
	
}

void UTestingGround_GameInstance::OnDestroySessionComplete(FName SessionName, bool IsSuccess)
{
	if (!IsSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not destroy session %s"), *SessionName.ToString())

			return;
	}

	//Create session if DestroySession() called from CreateSession() method
	if (bSessionNeed)
	{
		bSessionNeed = false;
		CreateSession();
	}

}