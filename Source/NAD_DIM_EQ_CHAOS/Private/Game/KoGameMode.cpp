// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/KoGameMode.h"

#include "Kismet/GameplayStatics.h"

void AKoGameMode::LoadShopLevel()
{
	const FLatentActionInfo LatentActionInfo;

	UGameplayStatics::LoadStreamLevel(GetWorld(), ShopLevel, true, true, LatentActionInfo);
}

void AKoGameMode::LoadHuntLevel()
{
	const FLatentActionInfo LatentActionInfo;

	UGameplayStatics::LoadStreamLevel(GetWorld(), HuntLevel, true, true, LatentActionInfo);
}
