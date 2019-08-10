// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UENUM()
enum class EWeaponType : uint8
{
	SemiAuto,
    FullAuto,
	Beem
};


UCLASS(meta = (BlueprintSpawnableComponent))
class S04_TESTINGGROUND_API AGun : public AActor
{
	GENERATED_BODY()

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;
	
public:	
	// Sets default values for this actor's properties
	AGun();

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ABeem> BeemProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Scatter = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	int ProjectileQuantity = 1;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FPFireAnimation;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* TPFireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimInstance* FPAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimInstance* TPAnimInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	EWeaponType WeaponType = EWeaponType::SemiAuto;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	int AmmoConsumption = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	int EnergyConsumption = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float ShotsPerSecond = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Fires a projectile. */
	UFUNCTION(BlueprintCallable, Category = "Gun")
	void OnFire(int& Energy, int& Ammo);

	UFUNCTION(BlueprintCallable, Category = "Gun")
	void OffFire();

	UFUNCTION(BlueprintPure, Category = "Gun")
	int GetAmmoConsumption() { return AmmoConsumption; }

	UFUNCTION(BlueprintPure, Category = "Gun")
	int GetEnergyConsumption() { return EnergyConsumption; }

	UFUNCTION(BlueprintPure, Category = "Crosshair")
	USceneComponent* GetMuzzleLocation() { return FP_MuzzleLocation; }

private:

	void SpawnProjectile();
	void Beaming(float DeltaTime);

	ABeem* BeamProjectile = nullptr;

	float LastFire = 0.f;

	bool bTriggerOn = false;

	int* InEnergy = nullptr;
	int* InAmmo = nullptr;

};
