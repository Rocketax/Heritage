// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/Tasks/KoBirdGuideDestroy.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Bird/KoBird.h"

EBTNodeResult::Type UKoBirdGuideDestroy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	DestroyGuide(OwnerComp.GetAIOwner()->GetPawn());

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UKoBirdGuideDestroy::DestroyGuide(AActor* Actor)
{
	const auto Bird = Cast<AKoBird>(Actor);

	if(Bird)
	{
		for(int i = Bird->GuidePathDestinations.Num() - 1; i>=0; i--)
		{
			Bird->GuidePathDestinations[i]->Destroy();
		}
		Bird->Destroy();
	}
}

void UKoBirdGuideDestroy::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}
