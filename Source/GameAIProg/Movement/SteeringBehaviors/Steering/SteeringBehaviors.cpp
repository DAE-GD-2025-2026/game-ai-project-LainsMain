#include "SteeringBehaviors.h"
#include "GameAIProg/Movement/SteeringBehaviors/SteeringAgent.h"

//SEEK
//*******
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