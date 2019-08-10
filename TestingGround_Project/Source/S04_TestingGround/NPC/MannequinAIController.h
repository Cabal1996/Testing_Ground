// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MannequinAIController.generated.h"

/**
 * 
 */
UCLASS()
class S04_TESTINGGROUND_API AMannequinAIController : public AAIController
{
	GENERATED_BODY()
	
	
public:

	//UFUNCTION(BlueprintCallable, Category = "AI")
	virtual FVector GetFocalPointOnActor(const AActor *Actor) const override;
	
};
