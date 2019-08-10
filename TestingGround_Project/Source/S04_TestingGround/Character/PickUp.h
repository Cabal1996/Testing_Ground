// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUp.generated.h"

class UBoxComponent;

UCLASS()
class S04_TESTINGGROUND_API APickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Default")
	float DespawnTime = 60.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	
	
};
