// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapon/Gun.h"


// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Creation of First person camera
	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FP Camera"));
	FPCamera->SetupAttachment(GetCapsuleComponent());
	FPCamera->RelativeLocation = FVector(4.5f, -4.25f, 64.f);
	FPCamera->bUsePawnControlRotation = true;

	//Creation of First person hands mesh (only owner can see)
	FPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP Mesh"));
	FPMesh->SetOnlyOwnerSee(true);
	FPMesh->SetupAttachment(FPCamera);
	FPMesh->bCastDynamicShadow = false;
	FPMesh->CastShadow = false;
	FPMesh->bReceivesDecals = false;
	FPMesh->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	FPMesh->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
	
}


void AMannequin::ActivateWeapon(AGun* WeaponToActivate)
{
	if (!WeaponToActivate) { return; }
	if (Gun)
	{
		Gun->GetRootComponent()->SetVisibility(false);
	}
	
	if (Shotgun)
	{
		Shotgun->GetRootComponent()->SetVisibility(false);
	}

	if (Machinegun)
	{
		Machinegun->GetRootComponent()->SetVisibility(false);
	}
	
	if (Beemgun)
	{
		Beemgun->GetRootComponent()->SetVisibility(false);
	}
	ActiveWeapon = WeaponToActivate;
	ActiveWeapon->GetRootComponent()->SetVisibility(true);
}

void AMannequin::PullTrigger()
{
	if (!ActiveWeapon) { return; }
	if (ActiveWeapon == Gun)
	{
		ActiveWeapon->OnFire(Energy, GunAmmoAmount);
	}
	else if (ActiveWeapon == Shotgun)
	{
		ActiveWeapon->OnFire(Energy, ShotgunShellAmount);		
	}
	else if (ActiveWeapon == Machinegun)
	{
		ActiveWeapon->OnFire(Energy, MachinegunAmmoAmount);
	}
	else if (ActiveWeapon == Beemgun)
	{
		ActiveWeapon->OnFire(Energy, BeemgnuAmmoAmount);
	}
}

void AMannequin::ReleaseTrigger()
{
	if (!ActiveWeapon) { return; }
	ActiveWeapon->OffFire();
}

void AMannequin::PickupWeapon(TSubclassOf<class AGun> GunClass, FName GunName)
{
	if (GunName == "Shotgun" && Shotgun == nullptr)
	{
		Shotgun = GetWorld()->SpawnActor<AGun>(GunClass);
		Shotgun->AttachToComponent(FPMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		Shotgun->FPAnimInstance = FPMesh->GetAnimInstance();
		Shotgun->TPAnimInstance = GetMesh()->GetAnimInstance();
		Shotgun->GetRootComponent()->SetVisibility(false);

		ActivateWeapon(Shotgun);
	}
	else if (GunName == "Machinegun" && Machinegun == nullptr)
	{
		Machinegun = GetWorld()->SpawnActor<AGun>(GunClass);
		Machinegun->AttachToComponent(FPMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		Machinegun->FPAnimInstance = FPMesh->GetAnimInstance();
		Machinegun->TPAnimInstance = GetMesh()->GetAnimInstance();
		Machinegun->GetRootComponent()->SetVisibility(false);

		ActivateWeapon(Machinegun);
	}
	else if (GunName == "Beemgun" && Beemgun == nullptr)
	{
		Beemgun = GetWorld()->SpawnActor<AGun>(GunClass);
		Beemgun->AttachToComponent(FPMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		Beemgun->FPAnimInstance = FPMesh->GetAnimInstance();
		Beemgun->TPAnimInstance = GetMesh()->GetAnimInstance();
		Beemgun->GetRootComponent()->SetVisibility(false);

		ActivateWeapon(Beemgun);
	}
}

void AMannequin::PickupEnergy(int EnergyAmount)
{
	Energy = FMath::Clamp<int>(Energy + EnergyAmount, 0, 100);
}

void AMannequin::PickupAmmo(int AmmoAmount, FName GunName)
{
	if (GunName == "Shotgun")
	{
		ShotgunShellAmount += AmmoAmount;
	}
	else if (GunName == "Machinegun")
	{
		MachinegunAmmoAmount += AmmoAmount;
	}
	else if (GunName == "Beemgun")
	{
		BeemgnuAmmoAmount += AmmoAmount;
	}
}

void AMannequin::Death(FVector FirstImpulsLocation, FVector SecondImpulsLocation)
{
	DetachFromControllerPendingDestroy();
	if (GetMesh())
	{
		
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
		GetMesh()->AddRadialImpulse(FirstImpulsLocation, 50, 1000, ERadialImpulseFalloff::RIF_Linear, true);
		GetMesh()->AddRadialImpulse(SecondImpulsLocation, 200, 200, ERadialImpulseFalloff::RIF_Linear, true);
	}

	Cast<UPrimitiveComponent>(GetRootComponent())->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AMannequin::Despawn()
{
	if (this)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(2.f);
	}
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	if (!GunBlueprint)
	{
		UE_LOG(LogTemp, Warning, TEXT("No main guns blueprints selected in %s"), *GetName());
		return;
	}
	
	Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	
	
	if (IsPlayerControlled())
	{
		Gun->AttachToComponent(FPMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	else
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	
	Gun->FPAnimInstance = FPMesh->GetAnimInstance();
	Gun->TPAnimInstance = GetMesh()->GetAnimInstance();
	ActiveWeapon = Gun;
}

void AMannequin::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (Gun)
	{
		Gun->Destroy();
	}

	if (Shotgun)
	{
		Shotgun->Destroy();
	}

	if (Machinegun)
	{
		Machinegun->Destroy();
	}

	if (Beemgun)
	{
		Beemgun->Destroy();
	}

	GetWorld()->DestroyActor(this, false, true);
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Energy regeneration
	if (LastTimeEnergyGet > 1 / EnergyPerSecond)
	{
		Energy = FMath::Clamp(Energy + 1, 0, 100);
		LastTimeEnergyGet = 0;
	}

	LastTimeEnergyGet +=DeltaTime;
}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMannequin::UnPossessed()
{
	Super::UnPossessed();

	if (Gun)
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	
	if (Shotgun)
	{
		Shotgun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}

	if (Machinegun)
	{
		Machinegun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}

	if (Beemgun)
	{
		Beemgun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
}