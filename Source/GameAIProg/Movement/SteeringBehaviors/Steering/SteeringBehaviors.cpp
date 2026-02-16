#include "SteeringBehaviors.h"
#include "GameAIProg/Movement/SteeringBehaviors/SteeringAgent.h"

//SEEK
SteeringOutput Seek::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput steering{};
	steering.LinearVelocity = Target.Position - Agent.GetPosition();

	float Distance = steering.LinearVelocity.Size();
	if (Distance < StopRadius)
	{
		steering.LinearVelocity = FVector2D::ZeroVector;
		return steering;
	}

	steering.LinearVelocity /= Distance; // normalize
	steering.LinearVelocity *= Agent.GetMaxLinearSpeed();
	return steering;
}

//FLEE
SteeringOutput Flee::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput steering{};
	steering.LinearVelocity = Agent.GetPosition() - Target.Position;

	float Distance = steering.LinearVelocity.Size();
	if (Distance > FleeRadius)
	{
		steering.LinearVelocity = FVector2D::ZeroVector;
		return steering;
	}

	steering.LinearVelocity /= Distance;
	steering.LinearVelocity *= Agent.GetMaxLinearSpeed();
	return steering;
}

//FACE
SteeringOutput Face::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput steering{};

	FVector2D Direction = Target.Position - Agent.GetPosition();
	if (Direction.IsNearlyZero())
		return steering;

	float DesiredAngle = FMath::RadiansToDegrees(FMath::Atan2(Direction.Y, Direction.X));
	float CurrentAngle = Agent.GetRotation();
	float AngleDiff = FMath::FindDeltaAngleDegrees(CurrentAngle, DesiredAngle);

	float MaxAngular = Agent.GetMaxAngularSpeed();
	steering.AngularVelocity = FMath::Clamp(AngleDiff, -MaxAngular, MaxAngular);

	return steering;
}

//ARRIVE
SteeringOutput Arrive::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	if (OriginalMaxSpeed == 0.f)
		OriginalMaxSpeed = Agent.GetMaxLinearSpeed();

	SteeringOutput steering{};
	steering.LinearVelocity = Target.Position - Agent.GetPosition();

	float Distance = steering.LinearVelocity.Size();

	if (Distance < TargetRadius)
	{
		Agent.SetMaxLinearSpeed(OriginalMaxSpeed);
		steering.LinearVelocity = FVector2D::ZeroVector;
		return steering;
	}

	if (Distance > SlowRadius)
	{
		Agent.SetMaxLinearSpeed(OriginalMaxSpeed);
	}
	else
	{
		float speed = OriginalMaxSpeed * (Distance - TargetRadius) / (SlowRadius - TargetRadius);
		Agent.SetMaxLinearSpeed(speed);
	}

	steering.LinearVelocity /= Distance;
	steering.LinearVelocity *= Agent.GetMaxLinearSpeed();
	return steering;
}

//PURSUIT
SteeringOutput Pursuit::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	float Distance = (Target.Position - Agent.GetPosition()).Size();
	float t = Distance / Agent.GetMaxLinearSpeed();

	FTargetData OriginalTarget = Target;
	Target.Position += Target.LinearVelocity * t;

	SteeringOutput steering = Seek::CalculateSteering(DeltaT, Agent);

	Target = OriginalTarget;
	return steering;
}