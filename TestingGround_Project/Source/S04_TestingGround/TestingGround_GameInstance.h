// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "TestingGround_GameInstance.generated.h"



UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EResponceType : uint8
{
	VE_Message 	UMETA(DisplayName = "Message"),
	VE_Loading 	UMETA(DisplayName = "Loading"),
	VE_EndLoading	UMETA(DisplayName = "EndLoading")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMenuResponce, EResponceType, ResponceType, FString, ResponseMesage);
/**
 * 
 */
UCLASS()
class S04_TESTINGGROUND_API UTestingGround_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UTestingGround_GameInstance();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "NetWork")
	void HostGame();

	UFUNCTION(BlueprintCallable, Category = "NetWork")
	void JoinGame(FString ServerIP);

	void OnCreateSessionComplete(FName SessionName, bool IsSuccess);
	void OnDestroySessionComplete(FName SessionName, bool IsSuccess);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	FString GetGameLevel() { return GameLevel; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	FString GetMenuLevel() { return MenuLevel; };

	UPROPERTY(BlueprintAssignable, Category = "My Events")
	FMenuResponce MenuResponce;

private:

	

	FString GameLevel;
	FString MenuLevel;

	class IOnlineSubsystem* OnlineSubsystem;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
	void CreateSession();

	
	FName SessionName;
	bool bIsJoining = false;
	FString ServerIP;
	bool bSessionNeed = false;
};
