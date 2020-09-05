#include "RegionSeeder.hpp"

RegionSeeder::RegionSeeder(Randomizer& randomizer, int numberOfPoints) 
: randomizer(randomizer)
{
    this->numberOfPoints = numberOfPoints;
}

std::vector<PN> RegionSeeder::seed(PN bottomCenter, float width, float height, FrameRegion type)
{
    std::vector<PN> generatedPoints = {};
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
        PN p;
        p.position.x = randomizer.randCenterEqually() * width;
        p.position.y = randomizer.rand() * height;
        p.position.z = randomizer.randCenterEqually() * width;
        if(frame->inBound(p))
        {
            p.position.x += bottomCenter.position.x;
            p.position.y += bottomCenter.position.y;
            p.position.z += bottomCenter.position.z;
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