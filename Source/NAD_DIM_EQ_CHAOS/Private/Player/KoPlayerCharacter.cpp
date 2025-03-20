// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KoPlayerCharacter.h"

#include "Bird/KoBirdPerceptionStimuliComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Sight.h"
#include "Player/KoPlayerCollector.h"
#include "Player/Interaction/KoInteractionComponent.h"

AKoPlayerCharacter::AKoPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	//Setting up components.
	PlayerCollector = CreateDefaultSubobject<UKoPlayerCollector>(TEXT("UKoPlayerCollector"));
	
	InteractionComponent = CreateDefaultSubobject<UKoInteractionComponent>(TEXT("UKoInteractionComponent"));

	BirdPerceptionStimuliComponent = CreateDefaultSubobject<UKoBirdPerceptionStimuliComponent>(
		TEXT("UKoBirdPerceptionStimuliComponent"));

	BirdPerceptionStimuliComponent->RegisterForSense(UAISense_Sight::StaticClass());

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);

	InitialCameraSpringArmLength = CameraSpringArm->TargetArmLength;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);

	TeamId = FGenericTeamId(0);

	GetMovementComponent()->NavAgentProps.bCanCrouch = true;
}

void AKoPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FGenericTeamId AKoPlayerCharacter::GetGenericTeamId() const
{
	return TeamId;
}
