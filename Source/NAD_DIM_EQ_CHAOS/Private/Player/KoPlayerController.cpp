// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/KoPlayerController.h"
#include "Player/KoPlayerCollector.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Game/KoGameState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Player/KoPlayerCharacter.h"
#include "Player/Interaction/KoInteractionComponent.h"

void AKoPlayerController::OnPauseInput(const FInputActionInstance& Instance)
{
	OnPause(UGameplayStatics::IsGamePaused(GetWorld()));
}

void AKoPlayerController::OnMoveInput(const FInputActionInstance& Instance)
{
	//Fetching actors and components.
	auto* PlayerCharacter = GetCharacter();
	check(PlayerCharacter);
	
	const auto* SpringArm = PlayerCharacter->FindComponentByClass<USpringArmComponent>();
	check(SpringArm);

	const auto& Value = Instance.GetValue().Get<FVector>();	

	const auto& Direction = SpringArm->GetComponentRotation();

	auto Movement = Direction.RotateVector(FVector{Value.Y, Value.X, 0});
	Movement.Normalize();

	//We calculate how much noise the character makes depending on her speed.
	auto NoiseStrength = GetPawn()->GetVelocity().Size() / GetCharacter()->GetCharacterMovement()->MaxWalkSpeed;
	
	//We make a noise at the character's location.
	MakeNoise(NoiseStrength > 0.25 ? NoiseStrength: 0.25, this->GetPawn(), this->GetPawn()->GetActorLocation());

	//We reduce the character's speed when crouched.
	float newValue = PlayerCharacter->bIsCrouched? Value.Size2D() : FMath::Clamp(Value.Size2D(), .4f, 1.f);
		
	PlayerCharacter->AddMovementInput(Movement, newValue);

	OnMove(Movement);
}

//This function handles the camera movement inputs to rotate it.
void AKoPlayerController::OnAimInput(const FInputActionInstance& Instance)
{
	const auto* PlayerCharacter = GetCharacter();
	check(PlayerCharacter);

	auto* SpringArm = PlayerCharacter->FindComponentByClass<USpringArmComponent>();
	check(SpringArm);

	const auto Direction = Instance.GetValue().Get<FVector>();

	const FRotator PreviousRotation = SpringArm->GetComponentRotation();

	float RotX = PreviousRotation.Yaw, RotY = PreviousRotation.Pitch;

	constexpr float MaxRotation = 60, MinRotation = -40;
	
    RotX += Direction.X * static_cast<const AKoPlayerCharacter*>(PlayerCharacter)->CameraSpeedX;
	
	//We calculate the rotation based on the camera rotation speed.
    if (RotY <= MaxRotation && RotY >= MinRotation )
    {
    	RotY += -Direction.Y * static_cast<const AKoPlayerCharacter*>(PlayerCharacter)->CameraSpeedY;
		//We clamp the Y Rotation according to the limits.
    	RotY = FMath::Clamp(RotY, MinRotation, MaxRotation);
    }

	const FQuat NewRotation = FQuat::MakeFromEuler(FVector(0, RotY, RotX));

	SpringArm->SetWorldRotation(NewRotation);

	OnAim(Direction);
}

void AKoPlayerController::OnInteractInput(const FInputActionInstance& Instance)
{
	if (!CanCapture) return;

	OnCapture();
}

void AKoPlayerController::OnPlaceSeedInput(const FInputActionInstance& Instance)
{
	//The player has the seed unlocked and can place it.
	if (CanPlaceSeed && PlaceSeedUnlocked)
	{
		CanPlaceSeed = false;
		OnPlaceSeed();
	}
}

//This function handles the crouch state of the character.
void AKoPlayerController::OnCrouchInput(const FInputActionInstance& Instance)
{
	auto* PlayerCharacter = GetCharacter();
	check(PlayerCharacter);

	auto* KoPlayer = static_cast<AKoPlayerCharacter*>(PlayerCharacter);
	check(KoPlayer);

	UKoPlayerCollector* PlayerCollector = KoPlayer->FindComponentByClass<UKoPlayerCollector>();
	
	if(PlayerCharacter->bIsCrouched)
	{
		PlayerCharacter->UnCrouch();
		PlayerCollector->IsCrouched = false;
	}
	else
	{
		PlayerCharacter->Crouch();
		PlayerCollector->IsCrouched = true;
	}
}

//This function places the seed on the ground, in front of the player character.
void AKoPlayerController::PlaceSeed()
{
	auto* PlayerCharacter = static_cast<AKoPlayerCharacter*>(GetCharacter());
 	check(PlayerCharacter);
	
 	const auto& Location = GetCharacter()->GetActorLocation() + GetCharacter()->GetActorForwardVector() * 250;
 
 	FHitResult HitResult;
 	
	//We raycast down where the seed would be placed to find the ground. We then spawn it at that location.
 	GetWorld()->LineTraceSingleByChannel(HitResult,
 										Location,
 										Location + -PlayerCharacter->GetActorUpVector() * 5000,
 										ECC_Visibility);

 	const auto& Rotation = GetCharacter()->GetActorRotation();

 	const FActorSpawnParameters SpawnParameters;
 	GetWorld()->SpawnActor<AActor>(PlayerCharacter->Seed, HitResult.Location, Rotation, SpawnParameters);
}

//This function interacts with the interaction area of the bird to capture it.
void AKoPlayerController::CaptureBird()
{
	const auto* PlayerCharacter = GetCharacter();

	check(PlayerCharacter);

	auto* InteractionComponent = PlayerCharacter->FindComponentByClass<UKoInteractionComponent>();

	check(InteractionComponent);

	InteractionComponent->TryInteractWithCurrentArea();
}

//This function unlocks the ability to place seeds. The player unlocks the seed when completing the first set of orders.
void AKoPlayerController::UnlockPlaceSeed(int32 Zone)
{
	PlaceSeedUnlocked = true;
	OnPlaceSeedUnlocked.Broadcast();
}

void AKoPlayerController::OnPlaceSeed_Implementation()
{	
}

AKoPlayerController::AKoPlayerController()
{
	bShowMouseCursor = true;
}

//This function binds all the inputs with their corresponding functions.
void AKoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

#pragma region ENHANCED_INPUTS_MAPPING
	const auto* LocalPlayer = GetLocalPlayer();

	check(LocalPlayer);

	auto* EnhancedInputLocalPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	check(EnhancedInputLocalPlayerSubsystem);

	EnhancedInputLocalPlayerSubsystem->ClearAllMappings();

	EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);

#pragma endregion

	auto* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	check(EnhancedInput);

	check(InputActionRegistry);

	EnhancedInput->BindAction(InputActionRegistry->PauseInput, ETriggerEvent::Triggered, this,
	                          &AKoPlayerController::OnPauseInput);

	EnhancedInput->BindAction(InputActionRegistry->MoveInput, ETriggerEvent::Triggered, this,
	                          &AKoPlayerController::OnMoveInput);

	EnhancedInput->BindAction(InputActionRegistry->AimInput, ETriggerEvent::Triggered, this,
	                          &AKoPlayerController::OnAimInput);

	EnhancedInput->BindAction(InputActionRegistry->InteractInput, ETriggerEvent::Triggered, this,
	                          &AKoPlayerController::OnInteractInput);

	EnhancedInput->BindAction(InputActionRegistry->PlaceSeedInput, ETriggerEvent::Triggered, this,
	                          &AKoPlayerController::OnPlaceSeedInput);

	EnhancedInput->BindAction(InputActionRegistry->CrouchInput, ETriggerEvent::Triggered, this,
							  &AKoPlayerController::OnCrouchInput);
}

void AKoPlayerController::OnCapture_Implementation()
{
}

void AKoPlayerController::OnMove_Implementation(const FVector Direction)
{
}

void AKoPlayerController::OnAim_Implementation(const FVector Direction)
{
}

void AKoPlayerController::OnPause_Implementation(bool IsGamePaused)
{
}
