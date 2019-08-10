// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AProjectile::AProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	//Set a mesh component
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh"));
	StaticMesh->AttachToComponent(CollisionComp, FAttachmentTransformRules::KeepRelativeTransform);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	//Set a impact particle system component
	ImpactParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("ImpactParticles"));
	ImpactParticle->AttachToComponent(CollisionComp, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactParticle->bAutoActivate = false;
	
}

void AProjectile::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	ImpactPoint = Hit.ImpactPoint;
	ImpactNormal = Hit.ImpactNormal;
	BoneName = Hit.BoneName;
	ComponentOfHit = OtherComp;

	UGameplayStatics::ApplyPointDamage(Other, BaseDamage, HitNormal, Hit, GetInstigatorController(), this, NULL);

	SetRootComponent(ImpactParticle);
	CollisionComp->DestroyComponent();
	StaticMesh->DestroyComponent();
	ImpactParticle->Activate();
	
	//Destroy();
	//OnProjectileHit_Implementation();
	OnProjectileHit();
}

void AProjectile::OnProjectileHit_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Hello"));
}
