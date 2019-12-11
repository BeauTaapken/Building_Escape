// Copyright Beau Taapken 2019

//GameFramework imports
#include "GameFramework/PlayerController.h"
//Engine imports
#include "Engine/World.h"
//Other header file imports
#include "Grabber.h"
#include "DrawDebugHelpers.h"

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

	///Get player viewpoint this tick
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);
	//Log out to test
	/*UE_LOG(LogTemp, Error, TEXT("PlayerViewPointLocation: %s"), *playerViewPointLocation.ToString());
	UE_LOG(LogTemp, Error, TEXT("PlayerViewPointRotation: %s"), *playerViewPointRotation.ToString());*/

	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;

	///Draw a red line to visualize the linetrace
	DrawDebugLine(
		GetWorld(),
		playerViewPointLocation,
		lineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0.0f,
		10.0f
	);

	///Setup query parameters
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());
	
	///Line-trace(Ray-cast) out to reach distance
	FHitResult hitResult;
	
	GetWorld()->LineTraceSingleByObjectType(
		OUT hitResult,
		playerViewPointLocation,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECC_PhysicsBody/*Gotten from ECollisionChannel::*/),
		traceParameters
	);
	
	///See what we hit
	AActor* hitResultActor = hitResult.GetActor();
	if(hitResultActor)
	{
		UE_LOG(LogTemp, Error, TEXT("LineTrace hit: %s"), *hitResultActor->GetName());
	}
}

