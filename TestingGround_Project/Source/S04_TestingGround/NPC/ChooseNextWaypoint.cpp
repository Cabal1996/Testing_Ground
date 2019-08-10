// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRout.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get the patrol rout points
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRout = ControlledPawn->FindComponentByClass<UPatrolRout>();
	if (!ensure(PatrolRout)) { return EBTNodeResult::Failed; }

	//Get the patrol way points
	auto WP = PatrolRout->GetWaypoints();
	if (WP.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	//Set next waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, WP[Index]);
	
	//Cycle the index
	auto NewIndex = (Index + 1) % WP.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NewIndex);

	return EBTNodeResult::Succeeded;
}
