// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UObject/Object.h"
#include "KoBirdWanderTask.generated.h"

//This task finds a random point around the bird to simulate wandering. It is used to make the bird act more natural
//and not stay still when it isn't fleeing or eating.
UCLASS()
class NAD_DIM_EQ_CHAOS_API UKoBirdWanderTask : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selectors")
	FBlackboardKeySelector DestinationKeySelector;

	FVector GenerateRandomWanderingPoint(AActor* Actor);
};
