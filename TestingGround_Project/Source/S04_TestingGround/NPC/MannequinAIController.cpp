// Fill out your copyright notice in the Description page of Project Settings.

#include "MannequinAIController.h"

FVector AMannequinAIController::GetFocalPointOnActor(const AActor *Actor) const
{
	return Actor != nullptr ? (Actor->GetActorLocation() + FVector(0, 0, 90)) : FAISystem::InvalidLocation;
}

