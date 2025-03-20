// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/Tasks/KoBirdMoveToDestinationTask.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "Bird/KoBird.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UKoBirdMoveToDestinationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;

	const auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	check(BlackboardComponent);

	const auto Bird = Cast<AKoBird>(OwnerComp.GetAIOwner()->GetPawn());

	//Setting up the bird to start its movement.
	Bird->StartingFleePosition = Bird->GetActorLocation();
	Bird->Path.Empty();
	Bird->TimeoutTimer = 0;

	FVector Destination = BlackboardComponent->GetValueAsVector(DestinationKeySelector.SelectedKeyName);
	AdjustDestinationHeight(Destination, Bird);

	//The bird guide has completed its path, start flying away and self destruct when out of view.
	if (BlackboardComponent->GetKeyID(FName("GuideIsFinalStep")) != FBlackboard::InvalidKey && BlackboardComponent->GetValueAsBool(FName("GuideIsFinalStep")))
	{
		Bird->OnFlyingAway();
	}

	Bird->Path.Push(Destination);

	return EBTNodeResult::InProgress;
}

void UKoBirdMoveToDestinationTask::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

void UKoBirdMoveToDestinationTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	const auto Bird = Cast<AKoBird>(OwnerComp.GetAIOwner()->GetPawn());
	
	//The bird starts landing when at the last 15% of its movement.
	constexpr float LandingProportion = 0.15;

	const float MovementDistance = FVector::Distance(Bird->StartingFleePosition, Bird->Path[Bird->Path.Num() - 1]);

	auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	check(BlackboardComponent);

	//This timer is used to generate a new point if the bird is stuck for too long.
	if (!Bird->CanAddPoint)
	{
		Bird->PointTimeoutTimer += DeltaSeconds;
	}

	//The bird should start landing now. It is at the end of its flight.
	if (FVector::Distance(Bird->GetActorLocation(), Bird->Path[Bird->Path.Num() - 1]) <= LandingProportion * MovementDistance)
	{
		if (!Bird->IsLanding)
		{
			Bird->IsLanding = true;
		}
	}

	//The bird is not moving. It may be stuck.
	if (Bird->GetMovementComponent()->Velocity.IsNearlyZero(0.1) &&
		Bird->TimeoutTimer < Bird->TimeoutTime)
	{
		Bird->TimeoutTimer += DeltaSeconds;
	}

	//The bird is close enough to its destination and is barely moving. We then finish the task early.
	if (Bird->GetMovementComponent()->Velocity.Size() < 20 &&
		FVector::Dist2D(Bird->GetActorLocation(), Bird->Path[Bird->Path.Num() - 1]) <= 30 &&
		FMath::Abs(Bird->GetActorLocation().Z - Bird->Path[Bird->Path.Num() - 1].Z) <= 40)
	{
		Bird->GetMovementComponent()->Velocity = FVector(0, 0, 0);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	//The timeout has been reached. The bird is stuck, we abort the movement.
	else if (Bird->GetMovementComponent()->Velocity.IsNearlyZero(0.01) &&
			 Bird->TimeoutTimer >=  Bird->TimeoutTime)
	{
		BlackboardComponent->ClearValue(DestinationKeySelector.SelectedKeyName);
		BlackboardComponent->ClearValue(FName("ShouldRunAway"));
		
		AbortTask(OwnerComp, NodeMemory);
	}
	//We generate a point that the bird has to move to.
	else
	{
		FollowPath(Bird, DeltaSeconds);
	}
}

//This function adjusts the height of the destination so the bird doesn't start floating or going underground.
void UKoBirdMoveToDestinationTask::AdjustDestinationHeight(FVector& Destination, const AKoBird* Bird) const
{
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, Destination, Destination - FVector(0, 0, 1000), ECC_Visibility);

	const float GroundZ = HitResult.ImpactPoint.Z;

	Destination.Z = GroundZ + Bird->BirdHalfHeight;
}

//This function generates a point that the bird will move to. It handles the obstacle avoidance.
void UKoBirdMoveToDestinationTask::FollowPath(APawn* Bird, float DeltaSeconds)
{
	FHitResult HitResult;

	TArray<FVector> PossiblePoints;
	TArray<FVector> PossibleDirections;

	auto BirdObj = Cast<AKoBird>(Bird);
		
	FVector Direction;

	const int MaxHeight = FVector::Dist2D(BirdObj->StartingFleePosition, BirdObj->Path[BirdObj->Path.Num() - 1]) / BirdObj->
		MaxFleeingDistance * 300;
	
	const float DistanceToDestination2D = FVector::Dist2D(BirdObj->Path[BirdObj->Path.Num() - 1], Bird->GetActorLocation());
	
	const float SensorDistance = 600 < DistanceToDestination2D ? 600 : DistanceToDestination2D;

	//We raycast in front of the bird to check for obstacles.
	bool IsObstacleInWay = GetWorld()->LineTraceSingleByChannel(HitResult,
	                                                            Bird->GetActorLocation(),
	                                                            Bird->GetActorLocation() + (Bird->
		                                                            GetActorForwardVector() *
		                                                            SensorDistance),
	                                                            ECC_Visibility);

	//We need to get a new point
	if (IsObstacleInWay || BirdObj->NeedsNewPoint)
	{
		BirdObj->NeedsNewPoint = false;
		
		constexpr int NbLines = 8;
        
        bool IsAngleBlocked = false;

		//Raycasts for obstacle avoidance. We raycast in a cone formation to check if there are still obstacles in those directions.
        for (int i = 0; i < NbLines; i++)
        {
            constexpr float ConeAngle = 360 / NbLines;

            const float LineLength = SensorDistance / FMath::Cos(FMath::DegreesToRadians(ConeAngle));

            const float Angle = ConeAngle * i;

        	//Rotate the lines accordingly
            FRotator Rotation = FRotator(ConeAngle * FMath::Cos(FMath::DegreesToRadians(Angle)),
                                         ConeAngle * FMath::Sin(FMath::DegreesToRadians(Angle)), 0);

            FVector ConeDirection = Rotation.RotateVector(Bird->GetActorForwardVector());

            IsAngleBlocked = GetWorld()->LineTraceSingleByChannel(HitResult, Bird->GetActorLocation(),
                                                                  Bird->GetActorLocation() + (ConeDirection *
            	                                                      LineLength), ECC_Visibility);
        	
        	//If there is no hit, the path is clear and there is no obstacle in the way. We add a potential point in that direction.
            if (!IsAngleBlocked)
            {
            	PossiblePoints.Push(Bird->GetActorLocation() + ConeDirection * LineLength / 2);
            }
        }
	}
	//Reset the last direction bias
	else if (!(BirdObj->Path.Num() - 1))
	{
		BirdObj->LastValidDirection = FVector::ZeroVector;
	}

	//If there is no potential direction, there are still obstacles blocking the directions.
	//We check the following directions: above, below and to the sides
	if (IsObstacleInWay && PossiblePoints.Num() == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			const float Angle = 90 * i;

			FRotator Rotation = FRotator(90 * FMath::Cos(FMath::DegreesToRadians(Angle)),
			                             90 * FMath::Sin(FMath::DegreesToRadians(Angle)), 0);

			FVector SideDirection = Rotation.RotateVector(Bird->GetActorForwardVector());

			bool IsAngleBlocked = GetWorld()->LineTraceSingleByChannel(HitResult, Bird->GetActorLocation(),
			                                                           Bird->GetActorLocation() + (SideDirection *
				                                                           SensorDistance), ECC_Visibility);

			//If there is no hit, we add to the list of potential directions
			if (!IsAngleBlocked)
			{
				PossiblePoints.Push(Bird->GetActorLocation() + (SideDirection * SensorDistance));
				PossibleDirections.Push(SideDirection);
			}
		}
	}

	//We have a potential point.
	if (PossiblePoints.Num() > 0)
	{
		FVector ChosenPoint = FVector::ZeroVector;

		FVector ChosenDirection = FVector::ZeroVector;
		FVector DestinationVector = BirdObj->Path[BirdObj->Path.Num() - 1];

		//We check for which point is the closest to the direction on a 2D plane and we choose that one.
		for (int i = 0; i < PossiblePoints.Num(); i++)
		{
			if (FVector::Dist2D(PossiblePoints[i], DestinationVector) < FVector::Dist2D(ChosenPoint, DestinationVector) || ChosenPoint == FVector::ZeroVector)
			{
				ChosenPoint = PossiblePoints[i];
				if (PossibleDirections.Num() > i)
				{
					ChosenDirection = PossibleDirections[i];
				}
				else
				{
					ChosenDirection = (ChosenPoint - Bird->GetActorLocation()).GetSafeNormal();
				}
			}
		}

		//To avoid adding points every tick, we have a timeout on when you can add points
		if (BirdObj->CanAddPoint)
		{
			if (BirdObj->Path.Num() > 1)
			{
				auto PointToRemove = BirdObj->Path[0];
				BirdObj->Path.Remove(PointToRemove);
			}
			BirdObj->LastValidDirection = ChosenDirection;

			BirdObj->Path.Insert(Bird->GetActorLocation() + BirdObj->LastValidDirection * SensorDistance, 0);

			BirdObj->CanAddPoint = false;
		}
	}

	//We get the direction of the next point
	Direction = (BirdObj->Path[0] - Bird->GetActorLocation()).GetSafeNormal();

	FVector NextPoint = (Bird->GetActorLocation() + (Direction * Bird->GetMovementComponent()->GetMaxSpeed() *
		DeltaSeconds));

	float TotalDistanceToTravel = FVector::Dist2D(BirdObj->Path[BirdObj->Path.Num() - 1], BirdObj->StartingFleePosition);

	//We adjust the height of the points in the first 65% of the travel to simulate flight.
	if ((TotalDistanceToTravel - DistanceToDestination2D) /  TotalDistanceToTravel < 0.65)
	{
		AdjustFlightHeight(NextPoint, BirdObj->Path[BirdObj->Path.Num() - 1], BirdObj->StartingFleePosition, MaxHeight);
	} 

	//We get the adjusted direction
	Direction = (NextPoint - Bird->GetActorLocation()).GetSafeNormal();

	//If we are close enough to the point and we are not at the final destination
	if (FVector::Dist2D(Bird->GetActorLocation(), BirdObj->Path[0]) <= 10 && BirdObj->Path.Num() > 1)
	{
		auto PointToRemove = BirdObj->Path[0];
		BirdObj->Path.Remove(PointToRemove);	
		BirdObj->CanAddPoint = true;
		BirdObj->PointTimeoutTimer = 0;
	}

	//Gradually rotate the bird over time
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Bird->GetActorLocation(), BirdObj->Path[0]);
	TargetRotation.Pitch = 0;
	FRotator RotationRemaining = TargetRotation - Bird->GetActorRotation();

	//We adjust the rotation to be within 180 degrees, negative or positive. That way, we get the shortest rotation.
	if (FMath::Abs(RotationRemaining.Yaw) > 180)
	{
		RotationRemaining.Yaw = RotationRemaining.Yaw < 0 ? -360 - RotationRemaining.Yaw : 360 - RotationRemaining.Yaw;
	}

	float RemainingDistance = FVector::Dist2D(Bird->GetActorLocation(), BirdObj->Path[0]);
	
	//The bird won't be able to rotate fast enough.
	if (FMath::Abs(RotationRemaining.Yaw) > 120 && RemainingDistance < 400)
	{
		BirdObj->NeedsNewPoint = true;
	}

	if (FMath::Abs(RotationRemaining.Yaw) > 0)
	{
		float AngleToRotate = RotationRemaining.Yaw > 0 ? 270 * DeltaSeconds : -270 * DeltaSeconds;

		//Rotation change for this frame.
		FRotator RotationDelta = Bird->GetActorForwardVector().RotateAngleAxis(AngleToRotate, Bird->GetActorUpVector()).ToOrientationRotator() - Bird->GetActorRotation();

		//Clamp the value if the delta is the bird has less rotating to do than it can.
		if (FMath::Abs(RotationRemaining.Yaw) < FMath::Abs(RotationDelta.Yaw))
		{
			RotationDelta = TargetRotation - Bird->GetActorRotation();
		}

		//Only change the yaw.
		RotationDelta.Pitch = 0;
		RotationDelta.Roll = 0;
		
		Bird->SetActorRotation(Bird->GetActorRotation() + RotationDelta);
	}

	//Adjusting movement direction.
	FVector TempDirection = FVector(Bird->GetActorForwardVector().X, Bird->GetActorForwardVector().Y, Direction.Z);

	//The bird is close to the destination.
	if (FVector::Dist2D(Bird->GetActorLocation(), BirdObj->Path[BirdObj->Path.Num() - 1]) <= 50)
	{
		Direction = (BirdObj->Path[BirdObj->Path.Num() - 1] - Bird->GetActorLocation()).GetSafeNormal();
	} else
	{
		Direction = TempDirection + Direction;
	}
	
	Bird->AddMovementInput((Direction).GetSafeNormal(), 1000, false);
}

//This function adjusts the height of the points the bird generates. The height is determined by an ease-out cubic
//function and by the proportion of movement that is left. It makes the flight look somewhat realistic.
void UKoBirdMoveToDestinationTask::AdjustFlightHeight(FVector& Point, FVector Destination, FVector Start,
                                                      float MaxFlightHeight)
{
	const float DistanceToDestination = FVector::Dist2D(Destination, Start);

	const float HalfDistance = DistanceToDestination / 2.f;

	const float DistanceFromStart = FVector::Dist2D(Point, Start);

	const float ZOffset = MaxFlightHeight * (1.f - FMath::Pow(
		(1.f - ((DistanceFromStart > HalfDistance
			         ? HalfDistance - (DistanceFromStart - HalfDistance)
			         : DistanceFromStart) / HalfDistance)), 3));

	Point.Z = Point.Z > Start.Z + ZOffset ? Point.Z : Start.Z + ZOffset;
}
