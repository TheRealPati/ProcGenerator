#include "TerrainGenAlgo.hpp"

TerrainGenAlgo::TerrainGenAlgo(int surfaceSideSize, int density) 
: rd{}, mt{rd()}, dist{0.0, 1.0}, 
surfaceSideSize(surfaceSideSize), density(density)
{}

TerrainGenAlgo::~TerrainGenAlgo(){}

double TerrainGenAlgo::rand() 
{
    return this->dist(mt);
}

double TerrainGenAlgo::randCenterZero() 
{
    return this->dist(mt) - 0.5f;
}

void TerrainGenAlgo::modifyTerrain(std::vector<VertexPNT>& vertices){}

std::vector<VertexPNT> TerrainGenAlgo::generateVertices()
{
    std::vector<VertexPNT> vertexData;

    for(unsigned int i = 0; i < surfaceSideSize; i++){
        for(unsigned int j = 0; j < surfaceSideSize; j++){
                VertexPNT pc;
                pc.position = {(float)i/density - surfaceSideSize/density/2, 0.0f, (float)j/density - surfaceSideSize/density/2};
                pc.texCoords = {1.0f/surfaceSideSize*i, 1.0f/surfaceSideSize*j};
                vertexData.push_back(pc);
        }
    }

    modifyTerrain(vertexData);
    genNormals(vertexData);

    return vertexData;
}

std::vector<GLuint> TerrainGenAlgo::generateIndices()
{
    std::vector<GLuint> indices;
    for(unsigned int j = 0; j < surfaceSideSize-1; j++){ //Last row not matter 
        for(unsigned int i = 0; i < surfaceSideSize-1; i++){ //Last column not matter
            //0 1 50 = i i+1 i+j
            //1 50 51 = i+1 i+j i+j+1
            indices.push_back(j*surfaceSideSize+i); 
            indices.push_back(j*surfaceSideSize+i+1);
            indices.push_back(j*surfaceSideSize+i+surfaceSideSize);
            indices.push_back(j*surfaceSideSize+i+1); 
            indices.push_back(j*surfaceSideSize+i+surfaceSideSize);
            indices.push_back(j*surfaceSideSize+i+surfaceSideSize+1);
        }
    }
    return indices;
}

void TerrainGenAlgo::genNormals(std::vector<VertexPNT>& vertices){
    for(unsigned int i = 0; i < vertices.size(); i++){
        glm::vec3 sum = glm::vec3(0.0f,0.0f,0.0f);

        //Ha van bal felső szomszédja
        if(i > surfaceSideSize - 1 && i % surfaceSideSize != 0){
            glm::vec3 directionLeft = glm::normalize(vertices[i-1].position - vertices[i].position);
            glm::vec3 directionUp = glm::normalize(vertices[i-surfaceSideSize].position - vertices[i].position);
            sum += glm::vec3(glm::cross(directionLeft, directionUp));
        }

        //Ha van jobb felső szomszédja
        if(i > surfaceSideSize - 1 && i % surfaceSideSize != surfaceSideSize-1){
            glm::vec3 directionRight = glm::normalize(vertices[i+1].position - vertices[i].position);
            glm::vec3 directionUp = glm::normalize(vertices[i-surfaceSideSize].position - vertices[i].position);
            sum += glm::vec3(glm::cross(directionUp, directionRight));
        }

        //Ha van bal alsó szomszédja
        if(i > surfaceSideSize*surfaceSideSize - 1 - surfaceSideSize && i % surfaceSideSize != 0){
            glm::vec3 directionLeft = glm::normalize(vertices[i-1].position - vertices[i].position);
            glm::vec3 directionDown = glm::normalize(vertices[i+surfaceSideSize].position - vertices[i].position);
            sum += glm::vec3(glm::cross(directionDown, directionLeft));
        }

        //Ha nem jobb oldal
        if(i > surfaceSideSize*surfaceSideSize - 1 - surfaceSideSize && i % surfaceSideSize != surfaceSideSize-1){
            glm::vec3 directionRight = glm::normalize(vertices[i+1].position - vertices[i].position);
            glm::vec3 directionDown = glm::normalize(vertices[i+surfaceSideSize].position - vertices[i].position);
            sum += glm::vec3(glm::cross(directionRight, directionDown));
        }
        vertices[i].normal = glm::normalize(sum);
    }
}