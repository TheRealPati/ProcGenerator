#include "LodManager.hpp"

LodManager::LodManager()
{

}

void LodManager::loadObjects(Material* woodMat)
{
    importer.loadModel("resources/models/h2.dae", ModelType::RIGGED);
    highTwo = new RiggedMesh(importer.getVertexData(), importer.getIndices(), importer.getWeights(), {});

    importer.loadModel("resources/models/h3.dae", ModelType::RIGGED);
    highThree = new RiggedMesh(importer.getVertexData(), importer.getIndices(), importer.getWeights(), {});

    importer.loadModel("resources/models/h4.dae", ModelType::RIGGED);
    highFour = new RiggedMesh(importer.getVertexData(), importer.getIndices(), importer.getWeights(), {});

    importer.loadModel("resources/models/l2.dae", ModelType::RIGGED);
    lowTwo = new RiggedMesh(importer.getVertexData(), importer.getIndices(), importer.getWeights(), {});

    importer.loadModel("resources/models/l3.dae", ModelType::RIGGED);
    lowThree = new RiggedMesh(importer.getVertexData(), importer.getIndices(), importer.getWeights(), {});

    importer.loadModel("resources/models/l4.dae", ModelType::RIGGED);
    lowFour = new RiggedMesh(importer.getVertexData(), importer.getIndices(), importer.getWeights(), {});

    Model* lowTwoModel = new Model();
    Model* lowThreeModel = new Model();
    Model* lowFourModel = new Model();
    Model* highTwoModel = new Model();
    Model* highThreeModel = new Model();
    Model* highFourModel = new Model();

    lowTwoModel->addMesh(lowTwo);
    lowThreeModel->addMesh(lowThree);
    lowFourModel->addMesh(lowFour);
    highTwoModel->addMesh(highTwo);
    highThreeModel->addMesh(highThree);
    highFourModel->addMesh(highFour);
    
    lowTwoModel->addMat(woodMat, 0);
    lowThreeModel->addMat(woodMat, 0);
    lowFourModel->addMat(woodMat, 0);
    highTwoModel->addMat(woodMat, 0);
    highThreeModel->addMat(woodMat, 0);
    highFourModel->addMat(woodMat, 0);

    lowTwoObj = new GameObject();
    lowThreeObj = new GameObject();
    lowFourObj = new GameObject();
    highTwoObj = new GameObject();
    highThreeObj = new GameObject();
    highFourObj = new GameObject();

    lowTwoObj->setInstanceCount(0);
    lowThreeObj->setInstanceCount(0);
    lowFourObj->setInstanceCount(0);
    highTwoObj->setInstanceCount(0);
    highThreeObj->setInstanceCount(0);
    highFourObj->setInstanceCount(0);

    lowTwoObj->setModel(lowTwoModel);
    lowThreeObj->setModel(lowThreeModel);
    lowFourObj->setModel(lowFourModel);
    highTwoObj->setModel(highTwoModel);
    highThreeObj->setModel(highThreeModel);
    highFourObj->setModel(highFourModel);
}

GameObject* LodManager::getObject(int numOfPipe, bool highPoly)
{
    if(highPoly)
    {
        if(numOfPipe == 2)
            return highTwoObj;
        else if(numOfPipe == 3)
            return highThreeObj;
        else
            return highFourObj;
    }
    else{
        if(numOfPipe == 2)
            return lowTwoObj;
        else if(numOfPipe == 3)
            return lowThreeObj;
        else
            return lowFourObj;
    }
}

void LodManager::setObject(std::vector<InstanceInfo> info, int numOfPipe, bool highPoly)
{
    if(highPoly)
    {
        if(numOfPipe == 2)
        {
            static_cast<RiggedMesh*>(highTwo)->setInstanceInfo(info);
            highTwoObj->setInstanceCount(info.size());
        }
        else if(numOfPipe == 3)
        {
            static_cast<RiggedMesh*>(highThree)->setInstanceInfo(info);
            highThreeObj->setInstanceCount(info.size());
        }
        else
        {
            static_cast<RiggedMesh*>(highFour)->setInstanceInfo(info);
            highFourObj->setInstanceCount(info.size());
        }
            
    }
    else{
        if(numOfPipe == 2)
        {
            static_cast<RiggedMesh*>(lowTwo)->setInstanceInfo(info);
            lowTwoObj->setInstanceCount(info.size());
        }
        else if(numOfPipe == 3)
        {
            static_cast<RiggedMesh*>(lowThree)->setInstanceInfo(info);
            lowThreeObj->setInstanceCount(info.size());
        }
        else
        {
            static_cast<RiggedMesh*>(lowFour)->setInstanceInfo(info);
            lowFourObj->setInstanceCount(info.size());
        }
    }
}

LodManager::~LodManager()
{
    
}