// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Mannequin.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class AGun;


UCLASS()
class S04_TESTINGGROUND_API AMannequin : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AMannequin();

	//First person camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FPCamera = nullptr;

	// Pawn mesh: 1st person view (arms; seen only by self)
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* FPMesh = nullptr;

	//Blueprint class for default gun
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class AGun> GunBlueprint;

	//Returns equipped weapon pinter
	UFUNCTION(BlueprintPure, Category = "Weapon")
	AGun* GetActiveWeapon() { return ActiveWeapon; };

	//Returns default gun pointer
	UFUNCTION(BlueprintPure, Category = "Weapon")
	AGun* GetGun() { return Gun; };

	//Returns Shotgun pointer
	UFUNCTION(BlueprintPure, Category = "Weapon")
	AGun* GetShotgun() { return Shotgun; };

	//Returns Machine gun pointer
	UFUNCTION(BlueprintPure, Category = "Weapon")
	AGun* GetMachinegun() { return Machinegun; };

	//Returns Beam gun pointer
	UFUNCTION(BlueprintPure, Category = "Weapon") // TODO Rename Beem to Beam
	AGun* GetBeemgun() { return Beemgun; };

	//Assign given weapon to "Active weapon" (Equip given weapon)
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ActivateWeapon(AGun* WeaponToActivate);

	//Pull a trigger of equipped weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PullTrigger();

	//Release a trigger of equipped weapon (Used with Beam and Machine guns)
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ReleaseTrigger();

	//Called from pickups to create weapon from blueprint subclass and assign it to a given weapon variable (Activate newly picked weapon)
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PickupWeapon(TSubclassOf<class AGun> GunClass, FName GunName);

	//Called from pickups to replenish energy
	UFUNCTION(BlueprintCallable, Category = "Energy")
	void PickupEnergy(int EnergyAmount);

	//Returns amount of energy currently available for character
	UFUNCTION(BlueprintPure, Category = "Energy")
	int GetEnergy() { return Energy; }

	//Called from pickups to replenish ammo of a given type
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void PickupAmmo(int AmmoAmount, FName GunName);

	//Returns current amount of shotgun ammo
	UFUNCTION(BlueprintPure, Category = "Ammo")
	int GetShotgunShellAmount() { return ShotgunShellAmount; }

	//Returns current amount of machine gun ammo
	UFUNCTION(BlueprintPure, Category = "Ammo")
	int GetMachinegunAmmoAmount() { return MachinegunAmmoAmount; }

	//Returns current amount of beam gun ammo
	UFUNCTION(BlueprintPure, Category = "Ammo")
	int GetBemgunAmmoAmount() { return BeemgnuAmmoAmount; }

	//Activate death sequence (Not in used, blueprint version richer)
	UFUNCTION(BlueprintCallable, Category = "Death")
	void Death(FVector FirstImpulsLocation, FVector SecondImpulsLocation);

	//Activate despawn sequence, called from Tile Destroy function to remove outdated AI characters from the world
	UFUNCTION(BlueprintCallable, Category = "Death")
	void Despawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Called when the game ends or when destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	//Not in use, part of undeveloped drop pickups system
	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	TMap<int, TSubclassOf<class APickUp>> PickupsAndChens;

	//Energy regeneration rate
	UPROPERTY(EditDefaultsOnly, Category = "Energy")
	float EnergyPerSecond = 1.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called on unpossess event fires, to move weapon from first person mesh hands to third person mesh hands of player character 
	virtual void UnPossessed() override;
private:


	AGun* Gun = nullptr;
	AGun* Shotgun = nullptr;
	AGun* Machinegun = nullptr;
	AGun* Beemgun = nullptr;

	AGun* ActiveWeapon = nullptr;
	

	int Energy = 100;

	int GunAmmoAmount = 0;
	int ShotgunShellAmount = 0;
	int MachinegunAmmoAmount = 0;
	int BeemgnuAmmoAmount = 0;

	float LastTimeEnergyGet = 0;
};
