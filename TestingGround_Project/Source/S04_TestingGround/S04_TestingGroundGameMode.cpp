// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "S04_TestingGroundGameMode.h"
#include "S04_TestingGroundHUD.h"
#include "Player/FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"
#include "Misc/ConfigCacheIni.h"

AS04_TestingGroundGameMode::AS04_TestingGroundGameMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Character/BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AS04_TestingGroundHUD::StaticClass();

	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(FName("NavMesh Bounds Volume Pool"));
}

void AS04_TestingGroundGameMode::AddToPool(AActor* VolumeToAdd)
{
	
	NavMeshBoundsVolumePool->Add(VolumeToAdd);
}

void AS04_TestingGroundGameMode::PopulateBoundsVolumePool(TArray<AActor*> NavMseshArray)
{
	for (auto Nav : NavMseshArray)
	{
		AddToPool(Nav);
	}

	//TODO OLD Need to be solved (this implementation does not need in arguments)
	/*
	TActorIterator<ANavMeshBoundsVolume> VolumeIterator(GetWorld());

	while (VolumeIterator)
	{
		AddToPool(*VolumeIterator);
		++VolumeIterator;
	}
	*/
}

void AS04_TestingGroundGameMode::NewTileConquered()
{
	Score++;
}

FGameResult AS04_TestingGroundGameMode::GetGameResult() const
{
	return FGameResult(Score, RoundTime, PlayerName);
}

void AS04_TestingGroundGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsPaused() && GetWorld()->GetFirstPlayerController()->GetPawn() != nullptr)
	{
		RoundTime += DeltaTime;
	}
	

}
