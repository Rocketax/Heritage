// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/KoBirdPerceptionComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Bird/KoBirdController.h"
#include "Bird/KoBirdPerceptionStimuliComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Player/KoPlayerController.h"

//This function is called when the player is perceived. It stores information on the stimuli.
void UKoBirdPerceptionComponent::OnTargetPerception(AActor* Actor, FAIStimulus Stimulus)
{
	auto* BirdController = Cast<AKoBirdController>(GetOwner());

	check(BirdController);
	
	const auto* StimuliComponent = Actor->FindComponentByClass<UKoBirdPerceptionStimuliComponent>();
	
	StimuliComponent->NotifyStimuliSensed(Cast<AKoBird>(GetOwner()), Stimulus.WasSuccessfullySensed());
	
	auto* Blackboard = BirdController->GetBlackboardComponent();
	
	check(Blackboard)
	
	Blackboard->SetValueAsFloat(FName("StimuliStrength"), Stimulus.Strength);
	
	Blackboard->SetValueAsObject(FName("Player"), Actor);

	auto* Bird = Cast<AKoBird>(Cast<AKoBirdController>(GetOwner())->GetPawn());

	check(Bird);

	auto* ActorInfo = GetActorInfo(*Actor);
	
	StimulusType = Stimulus.Type;
	
	IsPlayerHeard = ActorInfo->LastSensedStimuli[1].IsActive();

	IsPlayerSeen = ActorInfo->LastSensedStimuli[0].IsActive();
	
	StimulusStrength = Stimulus.Strength;

	ShouldRunAway = Blackboard->GetValueAsBool(FName("ShouldRunAway"));
}

UKoBirdPerceptionComponent::UKoBirdPerceptionComponent()
{
}

void UKoBirdPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//Setting a reoccuring timer that constantly adjusts the bird's detection.
	auto& TimerManager = GetWorld()->GetTimerManager();
	
	FTimerDelegate TimerDelegate;
	
	TimerDelegate.BindUObject(this, &UKoBirdPerceptionComponent::AdjustDetection);
	
	TimerManager.SetTimer(TimerHandle, TimerDelegate, 0.05f, true);
	
	OnTargetPerceptionUpdated.AddDynamic(this, &UKoBirdPerceptionComponent::OnTargetPerception);
}

void UKoBirdPerceptionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Removing event bindings.
	OnTargetPerceptionUpdated.RemoveDynamic(this, &UKoBirdPerceptionComponent::OnTargetPerception);

	Super::EndPlay(EndPlayReason);
}

void UKoBirdPerceptionComponent::AdjustDetection()
{
	//We update the detection when the bird sensed something and isn't already fleeing.
	if ((IsPlayerSeen || IsPlayerHeard) && !ShouldRunAway)
	{
		auto* BirdController = Cast<AKoBirdController>(GetOwner());
	
		check(BirdController);
		
		auto* Blackboard = BirdController->GetBlackboardComponent();
		
		check(Blackboard)
	
		auto* Bird = Cast<AKoBird>(Cast<AKoBirdController>(GetOwner())->GetPawn());
	
		check(Bird);

		const auto* PlayerController = Cast<AKoPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		checkf(PlayerController, TEXT("Cannot find PlayerController"))
		
		UndetectedTime = 0;
		
		//DetectedForce is a value that combines both the sight and hearing stimuli. Both senses are weighed equally.
		//This value is used to determine wether the bird should fly away or not depeding on the force of both stimulis combined.
		DetectedForce += IsPlayerHeard ?
			0.05 * (Bird->IsEating ? 0.5: 1) * StimulusStrength * (PlayerController->IsCrouched ? 0.5: 1)
			: 0;
		DetectedForce += IsPlayerSeen ?
			0.05 * (Bird->IsEating ? 0.5: 1) * SightAmplifier * (PlayerController->IsCrouched ? 0.5: 1)
			: 0;
		
		DetectedForce = DetectedForce > DetectionThreshold ? DetectionThreshold: DetectedForce;
		Bird->OnDetectionChanged(DetectedForce / DetectionThreshold);
		
		if (DetectedForce >= DetectionThreshold)
		{
			Blackboard->SetValueAsBool(FName("ShouldRunAway"), true);
		}
	}
	
	//We increase a timer indicating the time since the player has been sensed.
	if (!(IsPlayerSeen && IsPlayerHeard) && UndetectedTime <= UndetectionThreshold && DetectedForce > 0)
	{
		UndetectedTime += 0.05f;
	}

	//We reduce the detected force gradually over time if the player isn't sensed until it reaches 0.
	if (!(IsPlayerSeen && IsPlayerHeard) && UndetectedTime >= UndetectionThreshold && DetectedForce > 0)
	{
		DetectedForce -= 0.05f;
		DetectedForce = DetectedForce < 0 ? 0 : DetectedForce;
		
		auto* Bird = Cast<AKoBird>(Cast<AKoBirdController>(GetOwner())->GetPawn());
		
		check(Bird);
		
		Bird->OnDetectionChanged(DetectedForce / DetectionThreshold);
	}
}
