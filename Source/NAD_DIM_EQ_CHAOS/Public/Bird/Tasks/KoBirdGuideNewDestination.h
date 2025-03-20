// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "KoBirdGuideNewDestination.generated.h"

//This task is used to move the bird guide across every point in its set path.
UCLASS()
class UKoBirdGuideNewDestination : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selectors")
	FBlackboardKeySelector DestinationKeySelector;

	FVector GenerateNextDestination(AActor* Bird, UBlackboardComponent* BlackboardComponent);

private:
	int PathStep = 0;
};
