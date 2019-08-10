// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;


};

USTRUCT()
struct FObjectVolume
{
	GENERATED_USTRUCT_BODY()

	FVector CenterPoin;
	float Radius;
	FObjectVolume() : CenterPoin(FVector()), Radius(0) {}
	FObjectVolume(FVector &CenterPoint, float Radius) : CenterPoin(CenterPoint), Radius(Radius) {}
};

class UActorPool;
class AMannequin;

UCLASS()
class S04_TESTINGGROUND_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500, float MinScale = 1, float MaxScale = 1);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void PlaceAIPawns(TSubclassOf<AMannequin> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500);

	UFUNCTION(BlueprintCallable, Category = "Pool Setup")
	void SetPool(UActorPool* PoolToSet);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void DestroyTile();

	UFUNCTION(BlueprintCallable, Category = "GamePlay")
	void TileConquered();

	UFUNCTION(BlueprintPure, Category = "GamePlay")
	bool IsTileConquered() { return bIsTileConquered; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector NavigationBoundsOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void PositionNavMeshBoundsVolume();

	template<class T>
	void RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500, float MinScale = 1, float MaxScale = 1);

	bool FindeEmptyLocation(FVector& OutLocation, float Radius);

	bool CanSpawnAtLocation(FVector Location, float Radius);

	bool CalculateEmptyLocation(FVector& OutLocation, float Radius);

	bool IsLocationEmpty(FObjectVolume ObjectVolume);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition);

	void PlaceActor(TSubclassOf<AMannequin> ToSpawn, FSpawnPosition& SpawnPosition);

	TArray<AActor*> SpawnedActors;
	TArray<AMannequin*> SpawnedAIPawns;

	UActorPool* Pool = nullptr;
	
	AActor* NavMeshBoundsVolume = nullptr;

	bool bIsTileConquered = false;

	TArray<FObjectVolume> TotalVolume;
	
};

