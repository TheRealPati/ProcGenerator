#include "PlainGen.hpp"

PlainGen::PlainGen(int surfaceSideSize, int density)
: TerrainGenAlgo(surfaceSideSize, density)
{}

PlainGen::~PlainGen(){}

void PlainGen::modifyTerrain(std::vector<VertexPNT>& vertices){}