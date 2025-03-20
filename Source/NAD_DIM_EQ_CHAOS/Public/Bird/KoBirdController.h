// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KoBirdController.generated.h"

class UBehaviorTree;

//Class for the bird's AI Controller.
UCLASS(BlueprintType, Blueprintable)
class NAD_DIM_EQ_CHAOS_API AKoBirdController : public AAIController
{
	GENERATED_BODY()

public:
	AKoBirdController();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	UBehaviorTree* BehaviorTree;
	
	virtual void BeginPlay() override;
};
