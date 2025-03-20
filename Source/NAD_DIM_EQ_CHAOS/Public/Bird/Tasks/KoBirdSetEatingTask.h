// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "KoBirdSetEatingTask.generated.h"

//This task makes the bird start to eat. When it eats, its awareness is lowered, making it easier to capture.
UCLASS()
class NAD_DIM_EQ_CHAOS_API UKoBirdSetEatingTask : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
};
