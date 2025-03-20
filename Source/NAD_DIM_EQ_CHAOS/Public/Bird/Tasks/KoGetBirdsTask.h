// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "KoGetBirdsTask.generated.h"

//This task fetches all the bird actors. This is used to determine which color of feather the bird will give the player
//once captured.
UCLASS()
class NAD_DIM_EQ_CHAOS_API UKoGetBirdsTask : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selectors")
	FBlackboardKeySelector BirdsKeySelector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Selectors")
	FBlackboardKeySelector BirdsSetKeySelector;
};
