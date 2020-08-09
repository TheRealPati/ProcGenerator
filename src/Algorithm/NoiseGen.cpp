#include "NoiseGen.hpp"

NoiseGen::NoiseGen()
{

}

NoiseGen::~NoiseGen()
{
}

double NoiseGen::noise(glm::vec2 vector)
{
    double dotValue = glm::dot(vector * 0.001f , glm::vec2(24.12357, 36.789));
    return glm::fract(sin(dotValue) * 12345.123);
}

//Blur noise
double NoiseGen::smoothNoise(glm::vec2 vector)
{
    double smooth = (noise(vector + glm::vec2(1,1)) + noise(vector + glm::vec2(1,1)) + noise(vector + glm::vec2(1,1)) + noise(vector + glm::vec2(1,1)) ) * 0.0625 + (noise(vector + glm::vec2(1,0)) + noise(vector + glm::vec2(-1,0)) + noise(vector + glm::vec2(0,1)) + noise(vector + glm::vec2(0,-1)) ) * 0.125 + noise(vector) * 0.25;
	return smooth;
}

//Linear interpolation
double NoiseGen::interpolateNoise(glm::vec2 vector)
{
	double a, b, c, d;

	a = smoothNoise(floor(vector));	
	b = smoothNoise(glm::vec2(floor(vector.x+1.0), floor(vector.y)));
	c = smoothNoise(glm::vec2(floor(vector.x), floor(vector.y+1.0)));
	d = smoothNoise(glm::vec2(floor(vector.x+1.0), floor(vector.y+1.0)));
		
	a = glm::mix(a, b, glm::fract(vector.x));
	b = glm::mix(c, d, glm::fract(vector.x));
	a = glm::mix(a, b, glm::fract(vector.y));

	return a;				   	
}

double NoiseGen::fractalNoise(glm::vec2 vector)
{
	double n;
	
	n = interpolateNoise(vector*0.0625f)*0.5;
	n += interpolateNoise(vector*0.125f)*0.25;
	n += interpolateNoise(vector*0.025f)*0.225;
	n += interpolateNoise(vector*0.05f)*0.0625;
	n += interpolateNoise(vector)*0.03125;
	return n;
}

double NoiseGen::woodValue(glm::vec2 vector)
{
	double noise = this->fractalNoise(vector);
	return fabs(cos(vector.x * 0.1 + noise * 20.0));
}
