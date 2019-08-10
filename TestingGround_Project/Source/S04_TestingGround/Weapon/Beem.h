// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Beem.generated.h"

UCLASS()
class S04_TESTINGGROUND_API ABeem : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UParticleSystemComponent* ImpactParticle = nullptr;
	
public:	
	// Sets default values for this actor's properties
	ABeem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BeemMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float BaseDamage = 1.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UParticleSystemComponent* GetImpactParticle() { return ImpactParticle; }

	float GetBaseDamage() { return BaseDamage; }

	
	
};
