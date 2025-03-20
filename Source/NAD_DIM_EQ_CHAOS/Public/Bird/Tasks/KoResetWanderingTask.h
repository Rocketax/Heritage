// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "KoResetWanderingTask.generated.h"

//This task stop the bird from wandering after it has been scared or detected a seed.
UCLASS()
class NAD_DIM_EQ_CHAOS_API UKoBirdResetWanderingTask : public UBTTaskNode
{
public:
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
};
