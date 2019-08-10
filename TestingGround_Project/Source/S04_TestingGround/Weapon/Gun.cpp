// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Projectile.h"
#include "Beem.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AGun::AGun()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->bReceivesDecals = false;
	SetRootComponent(FP_Gun);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LastFire += DeltaTime;

	if (bTriggerOn == true)
	{
		if (WeaponType == EWeaponType::FullAuto)
		{
			SpawnProjectile();
		}
		else if (WeaponType == EWeaponType::Beem)
		{
			Beaming(DeltaTime);
		}
	}
}

//Trigger of a gun pulled
void AGun::OnFire(int& Energy, int& Ammo)
{
	InEnergy = &Energy;
	InAmmo = &Ammo;

	if (WeaponType == EWeaponType::SemiAuto)
	{
		SpawnProjectile();
	}
	else if (WeaponType == EWeaponType::FullAuto)
	{
		bTriggerOn = true;
	}
	else if (WeaponType == EWeaponType::Beem)
	{
		bTriggerOn = true;
		if (BeemProjectileClass != NULL)
		{
			if (BeamProjectile) { return; }
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				const FRotator SpawnRotation = FP_MuzzleLocation->GetComponentRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				ActorSpawnParams.Instigator = Cast<APawn>(GetAttachParentActor());

				// spawn the projectile at the muzzle
				BeamProjectile = World->SpawnActor<ABeem>(BeemProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				BeamProjectile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}
}

//Trigger of a gun released
void AGun::OffFire()
{
	bTriggerOn = false;

	if (WeaponType == EWeaponType::Beem)
	{
		bTriggerOn = false;
		if (BeamProjectile)
		{
			BeamProjectile->Destroy();
			BeamProjectile = nullptr;
		}
	}
}

void AGun::SpawnProjectile()
{
	// Release trigger on drop of a gun
	if (!GetAttachParentActor())
	{
		bTriggerOn = false;
		return;
	}

	// Check if Player have enough energy or/and ammo (Only calls for player)
	if (Cast<APawn>(GetAttachParentActor())->IsPlayerControlled())
	{
		if (InEnergy == nullptr || InAmmo == nullptr) { return; }
		if (*InEnergy < EnergyConsumption || *InAmmo < AmmoConsumption)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not enough energy or ammo!!!"));
			return;
		}
	}

	//Checking rate of fire for auto guns  
	if (LastFire >= (1 / ShotsPerSecond) || ShotsPerSecond == 0)
	{
		
		LastFire = 0;

		// try and fire a projectile
		if (ProjectileClass != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				for (int i = 0; i < ProjectileQuantity; i++)
				{
					const FRotator SpawnRotation = FP_MuzzleLocation->GetComponentRotation() + FRotator(FMath::RandRange(Scatter * -1, Scatter), FMath::RandRange(Scatter * -1, Scatter), 0.f);
					// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
					const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
					ActorSpawnParams.Instigator = Cast<APawn>(GetAttachParentActor());

					// spawn the projectile at the muzzle
					World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
				
				//Consume energy or/and ammo on every projectile spawned
				if (InEnergy != nullptr && InAmmo != nullptr && Cast<APawn>(GetAttachParentActor())->IsPlayerControlled())
				{
					*InEnergy = *InEnergy - EnergyConsumption;
					*InAmmo = *InAmmo - AmmoConsumption;
				}
			}
		}

		// try and play the sound if specified
		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FPFireAnimation != NULL)
		{
			// Get the animation object for the arms mesh
			if (FPAnimInstance != NULL)
			{
				FPAnimInstance->Montage_Play(FPFireAnimation, 1.f);
			}
		}

		// try and play a firing animation if specified
		if (TPFireAnimation != NULL)
		{
			// Get the animation object for the arms mesh
			if (TPAnimInstance != NULL)
			{
				TPAnimInstance->Montage_Play(TPFireAnimation, 1.f);
			}
		}
	}
	
}

void AGun::Beaming(float DeltaTime)
{
	// Release trigger on drop of a gun
	if (!GetAttachParentActor())
	{
		bTriggerOn = false;
		return;
	}

	// Check if Player have enough energy or/and ammo (Only calls for player)
	if (Cast<APawn>(GetAttachParentActor())->IsPlayerControlled())
	{
		if (InEnergy == nullptr || InAmmo == nullptr) { return; }
		if (*InEnergy < EnergyConsumption || *InAmmo < AmmoConsumption)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not enough energy or ammo!!!"));
			return;
		}
	}

	FHitResult Hit;
	FCollisionQueryParams CollisionParam;
	CollisionParam.AddIgnoredActor(this);
	UWorld* World = GetWorld();
	if (World)
	{
		if (World->LineTraceSingleByChannel(
			Hit,
			FP_MuzzleLocation->GetComponentLocation(),
			FP_MuzzleLocation->GetComponentLocation() + (FP_MuzzleLocation->GetForwardVector() * 10000),
			ECollisionChannel::ECC_GameTraceChannel1,
			CollisionParam
		))
		{
			float distance = Hit.Distance;
			if (BeamProjectile)
			{
				
				//scale up a beam to reach a target
				BeamProjectile->SetActorScale3D(FVector(distance/14.f, 1.f, 1.f));
				BeamProjectile->AddActorLocalRotation(FRotator(0, 0, 720.f * DeltaTime));
				//scale down particle effect of the beam
				BeamProjectile->GetImpactParticle()->SetRelativeScale3D(FVector(1 / (distance / 14.f), 1.f, 1.f));
			}


			//Applying damage to actor on line of trace every frame
			FVector ImpactPoint = Hit.ImpactPoint;
			FName BoneName = Hit.BoneName;
			AActor* Other = Hit.GetActor();
			FVector HitNormal = Hit.ImpactNormal;
			float BaseDamage = BeamProjectile->GetBaseDamage() * DeltaTime;

			UGameplayStatics::ApplyPointDamage(Other, BaseDamage, HitNormal, Hit, Cast<APawn>(GetAttachParentActor())->GetController(), this, NULL);
		}

		/*
		DrawDebugLine(
			World,
			FP_MuzzleLocation->GetComponentLocation(),
			FP_MuzzleLocation->GetComponentLocation() + (FP_MuzzleLocation->GetForwardVector() * 10000),
			FColor::Red,
			false,
			0.f,
			0,
			5.f
		);
		*/
	}
	
	
}
