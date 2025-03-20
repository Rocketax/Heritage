// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Bird/KoBird.h"
#include "KoBirdMoveToDestinationTask.generated.h"

//This task moves the bird to the chosen destination. It handles the obstacle avoidance and the flying of the bird.
UCLASS()
class NAD_DIM_EQ_CHAOS_API UKoBirdMoveToDestinationTask : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void AdjustDestinationHeight(FVector& Destination, const AKoBird* Bird) const;

	void AdjustFlightHeight(FVector& Point, FVector Destination, FVector Start, float MaxFlightHeight);
	
	void FollowPath(APawn* Bird, float DeltaSeconds);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selectors")
	FBlackboardKeySelector DestinationKeySelector;
	
};
