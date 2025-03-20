// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "KoBirdGuideDestroy.generated.h"

//This task is used to destroy the bird guide at the start of the game. The bird guide is a bird that the player follows
//to reach the main hunting area.
UCLASS()
class UKoBirdGuideDestroy : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selectors")
	FBlackboardKeySelector DestinationKeySelector;

	void DestroyGuide(AActor* Bird);
};
