#include "LogicalOperations.h"
#include<cmath>

float LogicalOperations::And(float w1, float w2)
{
	return w1 + w2 + std::sqrt((w1 * w1 + w2 * w2));
}

float LogicalOperations::Or(float w1, float w2)
{
	return w1 + w2 - std::sqrt((w1 * w1 + w2 * w2));
}

