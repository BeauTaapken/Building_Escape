// Fill out your copyright notice in the Description page of Project Settings.

//Other header file imports
#include "OpenDoor.h"
//GameFramework imports
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
//Engine imports
#include "Engine/World.h"
//Components imports
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();
	if(PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s doesn't have a pressure plate selected"), *owner->GetName());
	}

	DefaultLocation = owner->GetActorLocation();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PressurePlate) { return; }
	//Poll the Trigger volume
	if (GetTotalMassOfActorsOnPlate() > NeededWeight)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Check if door has to be closed
	if(GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float totalMass = 0.0f;

	//Find all overlapping actors
	TArray<AActor*> overlappingActors;

	if (!PressurePlate) { return totalMass; }
	PressurePlate->GetOverlappingActors(OUT overlappingActors);
	//Iterate through them adding masses
	for(const auto* Actor : overlappingActors)
	{
		totalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}
	
	return totalMass;
}

void UOpenDoor::OpenDoor()
{
	if(SlidingDoor)
	{
		owner->SetActorLocation(FMath::Lerp(owner->GetActorLocation(), SlidingDoorLerpLocation, 0.06f));
	}
	else
	{
		owner->SetActorRotation(FMath::Lerp(FQuat(owner->GetActorRotation()), FQuat(FRotator(0.0f, OpenAngle, 0.0f)), 0.06f));
	}
}

void UOpenDoor::CloseDoor()
{
	if (SlidingDoor)
	{
		owner->SetActorLocation(FMath::Lerp(owner->GetActorLocation(), DefaultLocation, 0.06f));
	}
	else
	{
		owner->SetActorRotation(FMath::Lerp(FQuat(owner->GetActorRotation()), FQuat(FRotator(0.0f, 90.0f, 0.0f)), 0.05f));
	}
}