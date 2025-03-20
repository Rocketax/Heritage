// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "KoBirdGetSeedDestination.generated.h"

//This task finds a point for the bird to move to in the seed area.
UCLASS()
class NAD_DIM_EQ_CHAOS_API UKoBirdGetSeedDestination : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selectors")
	FBlackboardKeySelector SeedKeySelector;

};
