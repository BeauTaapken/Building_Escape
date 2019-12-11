// Copyright Beau Taapken 2019

//GameFramework imports
#include "GameFramework/PlayerController.h"
//Engine imports
#include "Engine/World.h"
//Other header file imports
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Error, TEXT("Grabber reporting for duty!"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Get player viewpoint this tick
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);
	//TODO Log out to test
	UE_LOG(LogTemp, Error, TEXT("PlayerViewPointLocation: %s"), *playerViewPointLocation.ToString());
	UE_LOG(LogTemp, Error, TEXT("PlayerViewPointRotation: %s"), *playerViewPointRotation.ToString());
	
	//Ray-cast out to reach distance

	//See what we hit
	
}

