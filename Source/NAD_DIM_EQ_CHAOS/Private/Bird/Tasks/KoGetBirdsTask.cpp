// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/Tasks/KoGetBirdsTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Bird/KoBird.h"
#include "Game/KoGameState.h"

EBTNodeResult::Type UKoGetBirdsTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	checkf(Blackboard, TEXT("No blackboard found"))

	Blackboard->SetValueAsBool(BirdsSetKeySelector.SelectedKeyName, true);
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UKoGetBirdsTask::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}
