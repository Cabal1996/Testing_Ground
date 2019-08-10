// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GrassComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class S04_TESTINGGROUND_API UGrassComponent : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()
	
public:

	UGrassComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	FBox SpawningExtents;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	int SpawnCount;
	

protected:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	void SpawnGrass();
};
