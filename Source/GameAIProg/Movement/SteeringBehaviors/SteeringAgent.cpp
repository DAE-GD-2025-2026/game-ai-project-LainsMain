// Fill out your copyright notice in the Description page of Project Settings.

#include "SteeringAgent.h"
#include "DrawDebugHelpers.h"


// Sets default values
ASteeringAgent::ASteeringAgent()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASteeringAgent::BeginPlay()
{
	Super::BeginPlay();
}

void ASteeringAgent::BeginDestroy()
{
	Super::BeginDestroy();
}

// Called every frame
void ASteeringAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SteeringBehavior)
	{
		SteeringOutput output = SteeringBehavior->CalculateSteering(DeltaTime, *this);
		AddMovementInput(FVector{output.LinearVelocity, 0.f});

		if (bIsDebugRenderingEnabled)
		{
			FVector AgentPos = GetActorLocation();

			// Current velocity (green)
			FVector2D Vel2D = GetLinearVelocity();
			FVector VelEnd = AgentPos + FVector{Vel2D.X, Vel2D.Y, 0.f};
			DrawDebugLine(GetWorld(), AgentPos, VelEnd, FColor::Green, false, -1.f, 0, 2.f);

			// Desired velocity / steering output (cyan)
			FVector DesiredEnd = AgentPos + FVector{output.LinearVelocity.X, output.LinearVelocity.Y, 0.f};
			DrawDebugLine(GetWorld(), AgentPos, DesiredEnd, FColor::Cyan, false, -1.f, 0, 2.f);
		}
	}
}

// Called to bind functionality to input
void ASteeringAgent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASteeringAgent::SetSteeringBehavior(ISteeringBehavior* NewSteeringBehavior)
{
	SteeringBehavior = NewSteeringBehavior;
}

