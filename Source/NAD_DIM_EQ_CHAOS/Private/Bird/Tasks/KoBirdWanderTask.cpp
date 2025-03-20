// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/Tasks/KoBirdWanderTask.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Bird/KoBird.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UKoBirdWanderTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;
	
	auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	check(BlackboardComponent);
	
	FVector Destination = GenerateRandomWanderingPoint(OwnerComp.GetAIOwner()->GetPawn());
	
	BlackboardComponent->SetValueAsVector(DestinationKeySelector.SelectedKeyName, Destination);
	
	return EBTNodeResult::InProgress;
}

//This function finds a random accessible point within a certain radius around the bird.
FVector UKoBirdWanderTask::GenerateRandomWanderingPoint(AActor* Actor)
{
	const auto Bird = Cast<AKoBird>(Actor);
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	FNavLocation WanderingLocation;
	
	const FVector BirdLocation = Bird->GetActorLocation();
	NavSystem->GetRandomReachablePointInRadius(BirdLocation,200.f, WanderingLocation);
	
	UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(),
	Bird->GetActorLocation(), WanderingLocation.Location);

	return  WanderingLocation.Location ;
}

void UKoBirdWanderTask::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

//In the TickTask, we handle the rotation of the bird towards its destination. We want the bird to rotate naturally before
//starting its movement.
void UKoBirdWanderTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBTTaskNode::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	const auto Bird = OwnerComp.GetAIOwner()->GetPawn();

	AKoBird* BirdObj = Cast<AKoBird>(Bird); 

	const auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	check(BlackboardComponent);

	const FVector Destination = BlackboardComponent->GetValueAsVector(FName("Destination"));
	
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), Destination);
	TargetRotation.Pitch = 0;
	FRotator RotationRemaining = TargetRotation - Bird->GetActorRotation();

	//This transforms the rotation to be within 180 degrees, positive or negative. This is used so the bird rotates in the
	//correct direction. For example, instead of rotating 270 degrees to the right, we can just rotate 90 degrees to the left.
	if (FMath::Abs(RotationRemaining.Yaw) > 180)
	{
		RotationRemaining.Yaw = RotationRemaining.Yaw < 0 ? -360 - RotationRemaining.Yaw : 360 - RotationRemaining.Yaw;
	}

	//The bird is not yet facing in an acceptable direction.
	if (FMath::Abs(RotationRemaining.Yaw) > 0.1)
	{
		const float AngleToRotate = RotationRemaining.Yaw > 0 && RotationRemaining.Yaw <= 180 ? 540 * DeltaSeconds : -540 * DeltaSeconds;
		
		FRotator RotationDelta = Bird->GetActorForwardVector().RotateAngleAxis(AngleToRotate, Bird->GetActorUpVector()).ToOrientationRotator() - Bird->GetActorRotation();
		
		//If the bird has to rotate less than it can in a frame. We just complete the rotation.
		if (FMath::Abs(RotationRemaining.Yaw) < FMath::Abs(RotationDelta.Yaw))
		{
			RotationDelta = TargetRotation - Bird->GetActorRotation();
		}

		RotationDelta.Pitch = 0;
		RotationDelta.Roll = 0;
		
		Bird->SetActorRotation(Bird->GetActorRotation() + RotationDelta);
	}
	//The bird is done rotating towards its wandering point.
	else
	{
		BirdObj->IsWandering = true;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
