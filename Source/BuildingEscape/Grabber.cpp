// Copyright Beau Taapken 2019

///Components imports
#include "Components/PrimitiveComponent.h"
///GameFramework imports
#include "GameFramework/PlayerController.h"
///Engine imports
#include "Engine/World.h"
///Other header file imports
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

///Look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No physicshandle has been found on object %s"), *GetOwner()->GetName())
	}
}

///Look for attached input component(only appears at runtime)
void UGrabber::SetupInputComponent()
{
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No inputcomponent has been found on object %s"), *GetOwner()->GetName())
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//If the physics handle is attached
	if(physicsHandle->GrabbedComponent)
	{
		//Move the object that we're holding
		physicsHandle->SetTargetLocation(GetLineTraceStartAndEnd().v2);
	}
}

void UGrabber::Grab()
{
	///Line trace and see if we reach any actors with physics body collision channel set
	FHitResult hitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = hitResult.GetComponent();
	
	///If we hit something then attach a physics handle
	if(hitResult.GetActor())
	{
		//GrabComponentAtLocationWithRotation is used instead of GrabComponentAtLocation because rotation of the objects needs to be locked
		physicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None, //No bones needed
			ComponentToGrab->GetComponentLocation(),
			ComponentToGrab->GetComponentRotation()
		);
	}
}

void UGrabber::Release()
{
	physicsHandle->ReleaseComponent();
}

///Get player viewpoint this tick
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///Line-trace(Ray-cast) out to reach distance
	FHitResult hitResult;
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());
	FTwoVectors lineTracePoints = GetLineTraceStartAndEnd();
	
	GetWorld()->LineTraceSingleByObjectType(
		OUT hitResult,
		lineTracePoints.v1,
		lineTracePoints.v2,
		FCollisionObjectQueryParams(ECC_PhysicsBody/*Gotten from ECollisionChannel::*/),
		traceParameters
	);
	
	return hitResult;
}

FTwoVectors UGrabber::GetLineTraceStartAndEnd()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);

	FVector endLocation = playerViewPointLocation + playerViewPointRotation.Vector() * reach;
	
	return FTwoVectors(playerViewPointLocation, endLocation);
}
