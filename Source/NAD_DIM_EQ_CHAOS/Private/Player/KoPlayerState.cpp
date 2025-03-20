// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KoPlayerState.h"

void AKoPlayerState::OnFeathersChanged_Implementation()
{
}

void AKoPlayerState::IncreaseFeathers(int32 ByValue)
{
	SetScore(GetScore() + ByValue);

	OnFeathersChanged();
}