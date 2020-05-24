#include "RegionSeeder.hpp"

RegionSeeder::RegionSeeder(Randomizer& randomizer, int numberOfPoints) 
: randomizer(randomizer)
{
    this->numberOfPoints = numberOfPoints;
}

std::vector<Point> RegionSeeder::seed(Point bottomCenter, float width, float height, FrameRegion type)
{
    std::vector<Point> generatedPoints = {};
    int accepted = 0;

    Frame* frame;
    if(type == FrameRegion::SPHERE)
        frame = new SphereRegion(width, height);
    else if(type == FrameRegion::CYLINDER)
        frame = new CylinderRegion(width, height);
    else if(type == FrameRegion::CONE)
        frame = new ConeRegion(width, height);

    while(accepted < numberOfPoints || accepted == 10000)
    {
        Point p;
        p.x = randomizer.randCenterEqually() * width;
        p.y = randomizer.rand() * height;
        p.z = randomizer.randCenterEqually() * width;
        if(frame->inBound(p))
        {
            p.x += bottomCenter.x;
            p.y += bottomCenter.y;
            p.z += bottomCenter.z;
            generatedPoints.emplace_back(p);
            accepted++;
        }
    }
    delete frame;

    return generatedPoints;
}

RegionSeeder::~RegionSeeder()
{
}