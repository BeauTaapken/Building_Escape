// Fill out your copyright notice in the Description page of Project Settings.

//Other header file imports
#include "OpenDoor.h"
//GameFramework imports
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
//Engine imports
#include "Engine/World.h"

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

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll the Trigger volume
	if(PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
	}		
}

void UOpenDoor::OpenDoor()
{
	//Find the owning actor
	AActor* owner = GetOwner();

	//Create a rotator
	FRotator newRotation = FRotator(0.0f, OpenAngle, 0.0f);

	//Set the door rotation
	owner->SetActorRotation(newRotation);
}