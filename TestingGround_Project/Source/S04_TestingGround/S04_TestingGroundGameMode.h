// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetStringLibrary.h"
#include "GameFramework/GameModeBase.h"
#include "S04_TestingGroundGameMode.generated.h"


USTRUCT(BlueprintType)
struct FGameResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameResult")
	int Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameResult")
	float Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameResult")
	FName PlayerName;
	
	FGameResult() : Score(0), Time(0), PlayerName("") {}
	FGameResult(int Score, float Time, FName PlayerName) : Score(Score), Time(Time), PlayerName(PlayerName) {}
	//FGameResult(FGameResult &other) : Score(other.Score), Time(other.Time), PlayerName(other.PlayerName) {}

	
	FString ToString() const
	{
		return FString::Printf(TEXT("%s %i %s"), *PlayerName.ToString(), Score, *UKismetStringLibrary::TimeSecondsToString(Time));
	}

	bool operator<(const FGameResult &other) const
	{
		if (this->Score == other.Score)
		{
			return this->Time > other.Time;
		}
		else
		{
			return this->Score < other.Score;
		}
	}
	
};

class UActorPool;

UCLASS(minimalapi)
class AS04_TestingGroundGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AS04_TestingGroundGameMode();

	UFUNCTION(BlueprintCallable, Category = "Bonds Pool")
	void PopulateBoundsVolumePool(TArray<AActor*> NavMseshArray);

	UFUNCTION(BlueprintPure, Category = "Pool Setup")
	UActorPool* GetActorPool() { return NavMeshBoundsVolumePool; }

	void NewTileConquered();

	UFUNCTION(BlueprintPure, Category = "GamePlay")
	int GetScore() { return Score; }

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetPlayerName(FName NameToSet) { PlayerName = NameToSet; }

	UFUNCTION(BlueprintPure, Category = "Game Result")
	FGameResult GetGameResult() const;

	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	UActorPool* NavMeshBoundsVolumePool = nullptr;

private:
	
	void AddToPool(AActor* VolumeToAdd);

	int Score = 0;
	float RoundTime = 0;
	FName PlayerName = "Player";


};



