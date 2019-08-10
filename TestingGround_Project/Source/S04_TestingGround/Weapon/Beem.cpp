// Fill out your copyright notice in the Description page of Project Settings.

#include "Beem.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ABeem::ABeem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BeemMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	SetRootComponent(Cast<USceneComponent>(BeemMesh));

	//Set a impact particle system component
	ImpactParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("ImpactParticles"));
	ImpactParticle->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	ImpactParticle->bAutoActivate = true;
}

// Called when the game starts or when spawned
void ABeem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABeem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

