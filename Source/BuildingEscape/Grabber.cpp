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
	if (physicsHandle)
	{
		//physicsHandle is found
	}
	else
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
		//inputComponent is found
		UE_LOG(LogTemp, Warning, TEXT("Inputcomponent has been found on object %s"), *GetOwner()->GetName())
			///Bind the input axis
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
		//Move the object that we're holding
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("in grab"));

	///Line trace and see if we reach any actors with physics body collision channel set
	GetFirstPhysicsBodyInReach();
	
	///If we hit something then attach a physics handle
		//TODO attach physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("dropped"));

	//TODO release physics handle
}

///Get player viewpoint this tick
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);

	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;

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
	if (hitResultActor)
	{
		UE_LOG(LogTemp, Error, TEXT("LineTrace hit: %s"), *hitResultActor->GetName());
	}
	
	return FHitResult();
}