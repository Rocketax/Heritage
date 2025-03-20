// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "KoBirdGetNewDestinationTask.generated.h"

//Task that finds a fleeing destination after the bird has fled the player.
UCLASS()
class UKoBirdGetNewDestinationTask : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selectors")
	FBlackboardKeySelector DestinationKeySelector;

	FVector GenerateRandomFleeingPoint(AActor* Bird, FVector PlayerLocation,float StimuliStrength);	
};

