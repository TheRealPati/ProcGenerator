#include "PlainGen.hpp"

PlainGen::PlainGen(int surfaceSideSize, int density, Randomizer& randomizer)
: TerrainGenAlgo(surfaceSideSize, density, randomizer)
{}

PlainGen::~PlainGen(){}

void PlainGen::modifyTerrain(std::vector<VertexPNT>& vertices){}