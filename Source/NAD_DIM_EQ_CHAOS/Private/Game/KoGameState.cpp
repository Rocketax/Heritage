// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/KoGameState.h"

#include "Bird/KoBird.h"
#include "AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"
#include "Order/KoFeatherOrder.h"
#include "Player/KoPlayerController.h"

void AKoGameState::BeginPlay()
{
	Super::BeginPlay();

	//Binding event to the store.
	OnEnterStore.AddDynamic(this, &AKoGameState::GetNewOrders);
}

//This function generates all the orders needed to complete the game. The orders are completable in 2 stages : in the
//first zone and the second one. It generates orders based on what feathers are available in the world.
void AKoGameState::GenerateOrders(int32 Zone)
{
	TArray<FString> ClientsImages = GetAllClientsImages();
	TArray<FString> PapersImages = GetAllPaperImages();

	auto& OrdersList = Zone == 1 ? this->FirstOrderList : this->SecondOrderList;
	auto& FeathersList = Zone == 1 ? this->FirstFeatherList : this->SecondFeatherList;

	int32 OrderLimit = Zone == 1 ? FirstZoneOrderLimit : SecondZoneOrderLimit;
	
	//We generate orders as long as there are feathers or until we reach the order limit.
	while (FeathersList.Num() > 0 && OrdersList.Num() < OrderLimit)
	{
		int FeatherOrderCount = FMath::RandRange(2, 3);
		//This condition makes sure that we don't generate orders that are not completable.
		FeatherOrderCount = FeathersList.Num() == 3 ? 3 : FeatherOrderCount > FeathersList.Num() ? FeathersList.Num() : FeatherOrderCount;

		UKoFeatherOrder* Order = NewObject<UKoFeatherOrder>();
		
		//Each order is given a random client and paper style to be displayed when viewed.
		const int ChosenClient = FMath::RandRange(0, ClientsImages.Num() - 1);
		const int ChosenPaper = FMath::RandRange(0, PapersImages.Num() - 1);
		
		//Once a client image has been given, we remove that client image from the list to make it so every order has a unique client.
		Order->ClientImage = ClientsImages[ChosenClient];
		Order->PaperImage = PapersImages[ChosenPaper];
		ClientsImages.RemoveAt(ChosenClient);

		//We take the amount of feathers that are required for the order and remove them from the pool of available feathers.
		//This ensures that all orders are completable by not requiring a feather color that has been depleted.
		for (int i = 0; i < FeatherOrderCount; i++)
		{
			//We choose a random feather in the list.
			const int ChosenFeather = FMath::RandRange(0, FeathersList.Num() - 1);
			const FeatherColor Feather = FeathersList[ChosenFeather];
			//This is for the display of the colored feathers. FeathersRequired is an array of all the possible colors and their count.
			*Order->FeathersRequired.Find(Feather) += 1;
			FeathersList.RemoveAt(ChosenFeather);
		}
		OrdersList.Add(Order);
	}
	//We only assign the order list if we have just started the game.
	if (Zone == 1)
	{
		this->OrderList = OrdersList;
	}
	this->OnOrdersGenerated.Broadcast(OrdersList.Num(), Zone);
	this->AddRandomActiveOrder(3);
}

void AKoGameState::AddBird()
{
	this->BirdCount++;
}

//This function adds an order to the player's active order. It makes it viewable from the HUD.
void AKoGameState::AddActiveOrder(UKoFeatherOrder* Order)
{
	//We have reached the maximum amount of active orders. We can't proceed.
	if (this->ActiveOrders.Num() >= this->MaxActiveOrders) return;

	this->ActiveOrders.Add(Order);
}

//This function adds random active orders based on the amount provided.
void AKoGameState::AddRandomActiveOrder(int Amount)
{
	//We have reached the maximum amount of active orders or there are no orders left.
	if (this->ActiveOrders.Num() >= this->MaxActiveOrders || this->OrderList.Num() == 0) return;

	int i = 0;
	//We add active orders as long as there is space and orders to add until we have reached the amount desired.
	while (i < Amount && this->ActiveOrders.Num() < this->MaxActiveOrders && this->OrderList.Num() > 0)
	{
		UKoFeatherOrder* Order = NewObject<UKoFeatherOrder>();
		
		//We take a random order from the list.
		const int RandomIndex = FMath::RandRange(0, this->OrderList.Num() - 1);
		Order = this->OrderList[RandomIndex];
       
        this->ActiveOrders.Add(Order);
		this->OrderList.RemoveAt(RandomIndex);
		i++;
	}
}

//This function counts the available feathers and their color.
void AKoGameState::BirdsQueried(int32 Zone)
{
	TArray<FeatherColor>& FeatherList = Zone == 1 ? this->FirstFeatherList : this->SecondFeatherList;
	TArray<AActor*> BirdList = Zone == 1 ? this->BirdsFirstZone : this->BirdsSecondZone;

	for (auto* Actor : BirdList)
	{
		const AKoBird* Bird = Cast<AKoBird>(Actor);
		FeatherList.Add(Bird->FeatherColor);
	}
	this->GenerateOrders(Zone);
}

//This function fetched all the client images from the directory.
TArray<FString> AKoGameState::GetAllClientsImages() const
{
	const FString ClientsPath = "/Game/Images/Concept_Art/Commandes/Clients";
	
	TArray<FAssetData> AssetsInClientFolder;

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	
	FARFilter Filter;
	Filter.PackagePaths.Add(*ClientsPath);

	AssetRegistryModule.Get().GetAssets(Filter, AssetsInClientFolder);

	TArray<FString> ClientsImages = TArray<FString>();

	for (int i = 0; i < AssetsInClientFolder.Num(); ++i)
	{
		FAssetData& AssetData = AssetsInClientFolder[i];

		FString ClientImage = AssetData.ObjectPath.ToString();
		ClientsImages.Add(ClientImage);
	}
	return ClientsImages;
}

//This function fetched all the paper images from the directory.
TArray<FString> AKoGameState::GetAllPaperImages() const
{
	const FString PapersPath = "/Game/Images/Concept_Art/Commandes/Paper";
	
	TArray<FAssetData> AssetsInPaperFolder;

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	
	FARFilter Filter;
	Filter.PackagePaths.Add(*PapersPath);

	AssetRegistryModule.Get().GetAssets(Filter, AssetsInPaperFolder);

	TArray<FString> PapersImages = TArray<FString>();

	for (int i = 0; i < AssetsInPaperFolder.Num(); ++i)
	{
		FAssetData& AssetData = AssetsInPaperFolder[i];

		FString PaperImage = AssetData.ObjectPath.ToString();
		PapersImages.Add(PaperImage);
	}
	return PapersImages;
	
}

//This function makes sure that the player alway has orders to complete until all the birds are captured.
void AKoGameState::GetNewOrders()
{
	//The player completed all the orders in the first zone.
	if (this->OrderList.Num() == 0 && this->ActiveOrders.Num() == 0 && this->SecondOrderList.Num() != 0 && CurrentZone >= 2)
	{
		AKoPlayerController* PlayerController = Cast<AKoPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		checkf(PlayerController, TEXT("Cannot get PlayerController"))

		PlayerController->UnlockPlaceSeed(CurrentZone);
		this->OrderList = this->SecondOrderList;
		this->SecondOrderList.Empty();
	}

	//We top off the active orders so there is always the max amount.
	this->AddRandomActiveOrder(MaxActiveOrders - this->ActiveOrders.Num());

	OnNewOrdersObtained.Broadcast();
}

//This function is called when an order is completed and checks for game completion.
void AKoGameState::OrderCompleted()
{
	if (this->OrderList.Num() == 0)
	{
		this->OnAllOrdersCompleted.Broadcast(this->CurrentZone);
	}
}
