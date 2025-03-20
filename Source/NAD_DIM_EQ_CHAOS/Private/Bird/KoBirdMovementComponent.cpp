// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/KoBirdMovementComponent.h"


UKoBirdMovementComponent::UKoBirdMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	NavAgentProps.bCanFly = true;
}
