// Copyright Maddie Abbaspour 2021


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Math/TransformNonVectorized.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner() -> GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw ;

	FindPressurePlate();
	FindAudioComponent();
}
void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no PressuerPlte set"), *GetOwner()->GetName());
	}
}


void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Doesnt have AudioComponent"), *GetOwner()->GetName());
	}


}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld() ->GetTimeSeconds();
	}

	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
		
		// if the door has been open longer than DoorCloseDelay
		
	}

}

void UOpenDoor::OpenDoor(float DeltaTime){

	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle ,DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner() -> GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner() -> SetActorRotation(DoorRotation);

	if (!AudioComponent) { return; }
	if (!OpenDoorSoundPlayed)
	{
		AudioComponent->Play();
		OpenDoorSoundPlayed = true;
		CloseDoorSoundPlayed = false;
	}

}

void UOpenDoor::CloseDoor(float DeltaTime){

	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw ,DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner() -> GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner() -> SetActorRotation(DoorRotation);

	if (!AudioComponent) { return; }
	if (!CloseDoorSoundPlayed)
	{
		AudioComponent->Play();
		CloseDoorSoundPlayed = true;
		OpenDoorSoundPlayed = false;
	}

}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	//Find overlapping Actors
	TArray<AActor*> OverLappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);

	//Addup their masses
	for (AActor* Actor : OverLappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}


