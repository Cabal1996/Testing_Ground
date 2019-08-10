// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS(config=Game)
class AProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UParticleSystemComponent* ImpactParticle = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* StaticMesh = nullptr;

public:
	AProjectile();

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit) override;

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileHit();
	virtual void OnProjectileHit_Implementation();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float BaseDamage = 100.f;
	
	
	

	UFUNCTION(BlueprintPure, Category = "Info")
	FVector GetImpactPoint() { return ImpactPoint; }

	UFUNCTION(BlueprintPure, Category = "Info")
	FVector GetImpactNormal() { return ImpactNormal; }

	UFUNCTION(BlueprintPure, Category = "Info")
	FName GetBoneName() { return BoneName; }

	UFUNCTION(BlueprintPure, Category = "Info")
	UPrimitiveComponent* GetComponentOfHit() { return ComponentOfHit; }

private:

	FVector ImpactPoint;
	FVector ImpactNormal;
	FName BoneName;
	UPrimitiveComponent* ComponentOfHit;

	
};

