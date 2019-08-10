// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "Public/WorldCollision.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "ActorPool.h"
#include "Engine/World.h"
//#include "AI/NavigationSystem.h" OLD
#include "AI/NavigationSystemBase.h" // new since 4.20
#include "S04_TestingGroundGameMode.h"
#include "Character/Mannequin.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NavigationBoundsOffset = FVector(2000, 0, 0);

	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 1);
	
}

//Setting up Pool reference
void ATile::SetPool(UActorPool* PoolToSet)
{
	Pool = PoolToSet;

	PositionNavMeshBoundsVolume();
}

//Repositioning NavMeshe volume and rebuilding NavMesh
void ATile::PositionNavMeshBoundsVolume()
{
	
	NavMeshBoundsVolume = Pool->Checkout();
	if (!NavMeshBoundsVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("%s No enough actors in pool."), *GetName());
		return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);

	//New since 4.20
	
	UWorld* const World = GetWorld();
	const bool show = true;
	FNavigationSystem::Build(*World);
	
	

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	RandomlyPlaceActors(ToSpawn,  MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

void ATile::PlaceAIPawns(TSubclassOf<AMannequin> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, 1.f, 1.f);
}

template<class T>
inline void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		//if (FindeEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale))
		if (CalculateEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale))
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}

}

// Not in use
bool ATile::FindeEmptyLocation(FVector& OutLocation, float Radius)
{

	FBox Bounds(MinExtent, MaxExtent);
	

	const int MAX_ATTEMPTS = 100;
	for (int i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);

		if (CanSpawnAtLocation(CandidatePoint, Radius))
		{
			OutLocation = CandidatePoint;
			return true;
		}
	}

	return false;
}

// Not in use
bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
	///////////////////Debug Radius //////////////////
	/*

	FColor DebugColor = HasHit ? FColor::Red : FColor::Green;
	
	DrawDebugCapsule(
	GetWorld(),
	GlobalLocation,
	0,
	Radius,
	FQuat::Identity,
	DebugColor,
	true,
	100
	);
	*/
	return !HasHit;
}

bool ATile::CalculateEmptyLocation(FVector& OutLocation, float Radius)
{
	FBox Bounds(MinExtent, MaxExtent);

	const int MAX_ATTEMPTS = 100;
	for (int i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);

		FVector LeftDown(CandidatePoint.X - Radius, CandidatePoint.Y - Radius, 0.4f);
		FVector RightUp(CandidatePoint.X + Radius, CandidatePoint.Y + Radius, 0.5f);

		FBox CandidateBounds(LeftDown, RightUp);

		if (Bounds.IsInside(CandidateBounds))
		{
			if (IsLocationEmpty(FObjectVolume(CandidatePoint, Radius)))
			{
				OutLocation = CandidatePoint;
				return true;
			}
		}
	}

	return false;
}

bool ATile::IsLocationEmpty(FObjectVolume ObjectVolume)
{

	if (TotalVolume.Num() != 0)
	{
		for (FObjectVolume Volume : TotalVolume)
		{
			FVector OldVector = Volume.CenterPoin;
			FVector NewVector = ObjectVolume.CenterPoin;

			if (ObjectVolume.Radius + Volume.Radius > FVector::Distance(OldVector, NewVector))
			{
				return false;
			}

			if (TotalVolume.Last().CenterPoin == Volume.CenterPoin && TotalVolume.Last().Radius == Volume.Radius)
			{
				TotalVolume.Add(ObjectVolume);
			}
		}
	}
	else
	{
		TotalVolume.Add(ObjectVolume);
	}
	
	return true;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition)
{
	AActor* Spawn = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnPosition.Location, FRotator(0, SpawnPosition.Rotation, 0));
	if (Spawn)
	{
		Spawn->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawn->SetActorScale3D(FVector(SpawnPosition.Scale));

		SpawnedActors.Add(Spawn);
	}
	
}

void ATile::PlaceActor(TSubclassOf<AMannequin> ToSpawn, FSpawnPosition& SpawnPosition)
{
	FRotator Rotation = FRotator(0, SpawnPosition.Rotation, 0);

	AMannequin* SpawnedAI = GetWorld()->SpawnActor<AMannequin>(ToSpawn, SpawnPosition.Location, Rotation);
	if (SpawnedAI)
	{
		SpawnedAI->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		SpawnedAI->SpawnDefaultController();
		SpawnedAI->Tags.Add(FName("Enemy"));

		SpawnedAIPawns.Add(SpawnedAI);
	}
}

void ATile::DestroyTile()
{
	for (auto A : SpawnedActors)
	{
		if (A)
		{
			A->Destroy();
		}
	}
	
	for (auto P : SpawnedAIPawns)
	{
		if (P != NULL)
		{
			P->Despawn();
		}
	}
}

void ATile::TileConquered()
{
	if (!bIsTileConquered)
	{
		auto GameMode = Cast<AS04_TestingGroundGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->NewTileConquered();
		bIsTileConquered = true;
	}
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Super::EndPlay(EndPlayReason);

	if (Pool != nullptr && NavMeshBoundsVolume != nullptr)
	{
		Pool->Return(NavMeshBoundsVolume);
	}
	
}