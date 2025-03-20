// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/KoBirdController.h"
#include "Bird/KoBirdPerceptionComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"

AKoBirdController::AKoBirdController()
	: AAIController()
{
	//Setting up perception component.
	PerceptionComponent = CreateDefaultSubobject<UKoBirdPerceptionComponent>(TEXT("UKoBirdPerceptionComponent"));
}

void AKoBirdController::BeginPlay()
{
	Super::BeginPlay();
	
	SetGenericTeamId(FGenericTeamId(1));

	check(BehaviorTree);

	RunBehaviorTree(BehaviorTree);
}
