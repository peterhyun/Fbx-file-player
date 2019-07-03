//
//  FBXImporter.cpp
//  FBXLoader
//
//  Created by Jeehoon Hyun on 11/06/2019.
//  Copyright © 2019 Jeehoon Hyun. All rights reserved.
//

#include <FBXModel.h>
#include <btBulletDynamicsCommon.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//For debugging
void PrintMatrix(const FbxAMatrix& pMatrix)
{
    
    //print the TRS
    FBXSDK_printf(" T : %f %f %f %f\n",
                  pMatrix.GetT()[0],
                  pMatrix.GetT()[1],
                  pMatrix.GetT()[2],
                  pMatrix.GetT()[3] );
    
    FBXSDK_printf(" R : %f %f %f %f\n",
                  pMatrix.GetR()[0],
                  pMatrix.GetR()[1],
                  pMatrix.GetR()[2],
                  pMatrix.GetR()[3] );
    
    FBXSDK_printf(" S : %f %f %f %f\n",
                  pMatrix.GetS()[0],
                  pMatrix.GetS()[1],
                  pMatrix.GetS()[2],
                  pMatrix.GetS()[3] );
}

void FBXModel::ReadPosition(FbxMesh* mesh, int ctrlPointIndex, glm::vec3& position)
{
    FbxVector4* controlPoints = mesh->GetControlPoints();
    unsigned int ctrlPointCount = mesh->GetControlPointsCount();
    for(unsigned int i = 0; i < ctrlPointCount; i++)
    {
        position.x = controlPoints[ctrlPointIndex].mData[0];
        position.y = controlPoints[ctrlPointIndex].mData[1];
        position.z = controlPoints[ctrlPointIndex].mData[2];
    }
}

void FBXModel::ReadUV(FbxMesh* mesh, int inCtrlPointIndex, int textureUVIndex, glm::vec2& UV){
    if(mesh->GetElementNormalCount() < 1)
    {
        throw std::exception();
    }
    
    FbxGeometryElementUV * vertexUV = mesh->GetElementUV(0);
    
    switch(vertexUV->GetMappingMode())
    {
        case FbxGeometryElement::eByControlPoint:
            switch(vertexUV->GetReferenceMode())
        {
            case FbxGeometryElement::eDirect:
            {
                std::cout << "eByControlPoint" << std::endl;
                UV.x = vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0];
                UV.y = vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1];
            }
                break;
                
            case FbxGeometryElement::eIndexToDirect:
            {
                std::cout << "eByControlPoint" << std::endl;
                int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
                UV.x = vertexUV->GetDirectArray().GetAt(index).mData[0];
                UV.y = vertexUV->GetDirectArray().GetAt(index).mData[1];
            }
                break;
                
            default:{
                //std::cout << "ERROR1" << std::endl;
                throw std::exception();
            }
        }
            break;
            
        case FbxGeometryElement::eByPolygonVertex:
            switch(vertexUV->GetReferenceMode())
        {
            case FbxGeometryElement::eDirect:
            {
                std::cout << "eByVertex" << std::endl;
                UV.x = vertexUV->GetDirectArray().GetAt(textureUVIndex).mData[0];
                UV.y = vertexUV->GetDirectArray().GetAt(textureUVIndex).mData[1];
            }
                break;
                
            case FbxGeometryElement::eIndexToDirect:
            {
                //pirate example is here.
                //std::cout << "eByVertex" << std::endl;
                //int index = vertexUV->GetIndexArray().GetAt(textureUVIndex);
                //Hmm I don't get it but it worked lools
                UV.x = vertexUV->GetDirectArray().GetAt(textureUVIndex).mData[0];
                UV.y = vertexUV->GetDirectArray().GetAt(textureUVIndex).mData[1];
            }
                break;
                
            default:{
                //std::cout << "ERROR2" << std::endl;
                throw std::exception();
            }
        }
    }
}

void FBXModel::ReadNormal(FbxMesh* mesh, int inCtrlPointIndex, int inVertexCounter, glm::vec3& outNormal)
{
    if(mesh->GetElementNormalCount() < 1)
    {
        throw std::exception();
    }
    
    FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
    
    switch(vertexNormal->GetMappingMode())
    {
        case FbxGeometryElement::eByControlPoint:
            switch(vertexNormal->GetReferenceMode())
        {
            case FbxGeometryElement::eDirect:
            {
                //std::cout << "eByControlPoint" << std::endl;
                outNormal.x = vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0];
                outNormal.y = vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1];
                outNormal.z = vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2];
            }
                break;
                
            case FbxGeometryElement::eIndexToDirect:
            {
                //std::cout << "eByControlPoint" << std::endl;
                int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
                outNormal.x = vertexNormal->GetDirectArray().GetAt(index).mData[0];
                outNormal.y = vertexNormal->GetDirectArray().GetAt(index).mData[1];
                outNormal.z = vertexNormal->GetDirectArray().GetAt(index).mData[2];
            }
                break;
                
            default:{
                //std::cout << "ERROR1" << std::endl;
                throw std::exception();
            }
        }
            break;
            
        case FbxGeometryElement::eByPolygonVertex:
            switch(vertexNormal->GetReferenceMode())
        {
            case FbxGeometryElement::eDirect:
            {
                //std::cout << "eByVertex" << std::endl;
                outNormal.x = vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0];
                outNormal.y = vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1];
                outNormal.z = vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2];
            }
                break;
                
            case FbxGeometryElement::eIndexToDirect:
            {
                //std::cout << "eByVertex" << std::endl;
                int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
                outNormal.x = vertexNormal->GetDirectArray().GetAt(index).mData[0];
                outNormal.y = vertexNormal->GetDirectArray().GetAt(index).mData[1];
                outNormal.z = vertexNormal->GetDirectArray().GetAt(index).mData[2];
            }
                break;
                
            default:{
                //std::cout << "ERROR2" << std::endl;
                throw std::exception();
            }
        }
    }
}

void FBXModel::ProcessMesh(FbxNode* node){
    FbxMesh* currMesh = node->GetMesh();
    int mTriangleCount = currMesh->GetPolygonCount();
    std::cout << "Number of polygons are " << mTriangleCount << std::endl;
    int vertexCounter = 0;
    
    for (unsigned int i = 0; i < mTriangleCount; ++i)
    {
        glm::vec3 position[3];
        glm::vec3 normal[3];
        glm::vec2 UV[3];
        for (unsigned int j = 0; j < 3; ++j)
        {
            int ctrlPointIndex = currMesh->GetPolygonVertex(i, j);
            ReadPosition(currMesh, ctrlPointIndex, position[j]);
            ReadNormal(currMesh, ctrlPointIndex, vertexCounter, normal[j]);
            ReadUV (currMesh, ctrlPointIndex, currMesh->GetTextureUVIndex(i, j), UV[j]);
            Vertex vertex;
            vertex.position = position[j];
            vertex.normal = normal[j];
            vertex.UV = UV[j];
            vertices.push_back(vertex);
            vertexCounter++;
        }
    }
    std::cout << "Number of 'vertices': " << vertices.size() << std::endl;
}

void FBXModel::ProcessSkeletonHierarchy(FbxNode* rootNode){
    for(int childIndex=0;childIndex < rootNode->GetChildCount();childIndex++){
        FbxNode* currNode = rootNode->GetChild(childIndex);
        //Root of the SKELETON!! (not the Fbx root node) has the index 0. Index increases in DFS order.
        ProcessSkeletonHierarchyRecursively(currNode, 0, -1);
    }
    //std::cout << "number of joints are "<< rig.Joints.size() << std::endl;
}

void FBXModel::ProcessSkeletonHierarchyRecursively(FbxNode * node, int myIndex, int parentIndex){
    if(node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && node->GetNodeAttribute()->GetAttributeType()== FbxNodeAttribute::eSkeleton){
        Joint currJoint;
        currJoint.mParentIndex = parentIndex;
        currJoint.mName = node->GetName();
        //std::cout << currJoint.mName << std::endl; //For debugging
        rig.Joints.push_back(currJoint);
    }
    else{
        return;
    }
    //Process Skeleton in DFS way.
    for(int i=0;i<node->GetChildCount();i++){
        ProcessSkeletonHierarchyRecursively(node->GetChild(i), rig.Joints.size(), myIndex);
    }
}

int FBXModel::findJointIndexUsingName(const char * jointName){
    for(int i=0;i<rig.Joints.size();i++){
        if(strcmp(rig.Joints.at(i).mName, jointName)==0){
            return i;
        }
    }
    //Something is wrong if they haven't found the index til here.
    return -1;
}

void FBXModel::ProcessJointsAndAnimation(FbxNode* node){
    FbxMesh * currMesh = node->GetMesh();
    int numOfDeformers = currMesh->GetDeformerCount();
    for(int deformerIndex = 0;deformerIndex < numOfDeformers;deformerIndex++){
        FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
        if(!currSkin){
            continue;
        }
        int numOfClusters = currSkin->GetClusterCount();
        for(int clusterIndex=0;clusterIndex < numOfClusters;clusterIndex++){
            FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
            const char * currJointName = currCluster->GetLink()->GetName();
            int currJointIndex = findJointIndexUsingName(currJointName);
            std::cout << "currJointName is " << currJointName << std::endl;
            std::cout << "currJointIndex is " << currJointIndex << std::endl;
            FbxAMatrix transformLinkMatrix;
            FbxAMatrix globalBindPoseInverseMatrix;
            currCluster->GetTransformLinkMatrix(transformLinkMatrix);
            globalBindPoseInverseMatrix = transformLinkMatrix.Inverse(); //* transformMatrix;
            
            for(int i=0;i<4;i++){
                for(int j=0;j<4;j++){
                    rig.Joints[currJointIndex].globalBindPoseInverse[i][j] = globalBindPoseInverseMatrix.Get(i, j);
                }
            }
            
            rig.Joints[currJointIndex].node = currCluster->GetLink();
            //controlpointindices to controlpointweights is always a one-to-one relation
            rig.Joints[currJointIndex].controlPointIndices = currCluster->GetControlPointIndices();
            rig.Joints[currJointIndex].weights = currCluster->GetControlPointWeights();
            for(int i=0;i<currCluster->GetControlPointIndicesCount();i++){
                FbxVector4 ctrlPoint = currMesh->GetControlPoints()[currCluster->GetControlPointIndices()[i]];
                glm::vec3 ctrlPointConvert;
                ctrlPointConvert.x = ctrlPoint[0];
                ctrlPointConvert.y = ctrlPoint[1];
                ctrlPointConvert.z = ctrlPoint[2];
                
                Joint_Weight joint_weight(currCluster->GetControlPointWeights()[i], currJointIndex);
                map.insert(std::make_pair(ctrlPointConvert, joint_weight));
            }
            
            //Only supports one animation
            FbxAnimStack* currAnimStack = scene->GetCurrentAnimationStack();
            FbxString animStackName = currAnimStack->GetName();
            FbxTakeInfo * takeInfo = scene->GetTakeInfo(animStackName);
            FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
            FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
            
            //std::cout << start.GetFrameCount() << std::endl;
            //std::cout << end.GetFrameCount() << std::endl;
            node->EvaluateGlobalTransform(start);
            for(FbxLongLong i = start.GetFrameCount();i<end.GetFrameCount();i++){
                FbxTime currTime;
                currTime.SetFrame(i);
                FbxAMatrix temp = currCluster->GetLink()->EvaluateGlobalTransform(currTime);
                //PrintMatrix(temp);
                glm::mat4 temp2;
                for(int j=0;j<4;j++){
                    for(int k=0;k<4;k++){
                        temp2[j][k] = temp.Get(j, k);
                    }
                }
                rig.Joints[currJointIndex].frames.push_back(temp2);
            }
        }
    }
    
}

void FBXModel::SetGlobalBindInverseMatrices(Shader &ourShader){
    std::vector<glm::mat4> globalBindInverseMatrices;
    for(int i=0;i<rig.Joints.size();i++){
        globalBindInverseMatrices.push_back(rig.Joints[i].globalBindPoseInverse);
    }
    ourShader.setMultipleMat4("JointGlobalBindInverse", rig.Joints.size(), &globalBindInverseMatrices[0]);
}

void FBXModel::updateAnimation(Shader &ourShader, int frameIndex){
    //i is the index of the frame.
    std::vector<glm::mat4> jointTransformations;
    //Get joint transformations
    for(int j=0;j<rig.Joints.size();j++){
        jointTransformations.push_back(rig.Joints[j].frames[frameIndex]);
    }
    ourShader.setMultipleMat4("Animation",rig.Joints.size(), &jointTransformations[0]);
}

typename std::multimap<glm::vec3, Joint_Weight>::const_iterator find_pair(std::multimap<glm::vec3, Joint_Weight>& map, std::pair<glm::vec3, Joint_Weight>& pair)
{
    typedef typename std::multimap<glm::vec3, Joint_Weight>::const_iterator it;
    std::pair<it,it> range = map.equal_range(pair.first);
    for (it p = range.first; p != range.second; ++p)
        if ((p->second.JointIndex == pair.second.JointIndex) && (p->second.weight == pair.second.weight))
            return p;
    return map.end();
}

bool insert_if_not_present(std::multimap<glm::vec3, Joint_Weight>& map, std::pair<glm::vec3, Joint_Weight>& pair)
{
    if (find_pair(map, pair) == map.end()) {
        map.insert(pair);
        return true;
    }
    return false;
}

void FBXModel::SetJointIndices_Weights(){
    std::multimap<glm::vec3, Joint_Weight> removeDuplicatePair;
    for (std::map<glm::vec3, Joint_Weight>::iterator it=map.begin(); it!=map.end(); ++it){
        std::pair<glm::vec3, Joint_Weight> pair = std::make_pair(it->first, it->second);
        insert_if_not_present(removeDuplicatePair, pair);
    }
    for(int i=0;i<vertices.size();i++){
        
        float indices[4] = {};
        float weights[4] = {};
        float indices2[4] = {};
        float weights2[4] = {};
        
        glm::vec3 position = vertices[i].position;
        std::pair<std::multimap<glm::vec3, Joint_Weight>::iterator, std::multimap<glm::vec3, Joint_Weight>::iterator> result = removeDuplicatePair.equal_range(position);
        int count = 0;
        for(std::multimap<glm::vec3, Joint_Weight>::iterator it=result.first;it!=result.second;it++){
            Joint_Weight temp = it->second;
            if(count < 4){
                indices[count] = temp.JointIndex;
                weights[count] = temp.weight;
            }
            else{
                indices2[count-4] = temp.JointIndex;
                weights2[count-4] = temp.weight;
            }
            count++;
        }
        vertices[i].jointIndices = glm::vec4(indices[0], indices[1], indices[2], indices[3]);
        vertices[i].jointIndices2 = glm::vec4(indices2[0], indices2[1], indices2[2], indices2[3]);
        
        vertices[i].weights = glm::vec4(weights[0], weights[1], weights[2], weights[3]);
        vertices[i].weights2 = glm::vec4(weights2[0], weights2[1], weights2[2], weights2[3]);
        
    }
}

void FBXModel::SetBuffers_Textures(Shader& ourShader){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24*sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24*sizeof(float), (void *)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 24*sizeof(float), (void *)offsetof(Vertex, UV));
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 24*sizeof(float), (void *)offsetof(Vertex, jointIndices));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 24*sizeof(float), (void *)offsetof(Vertex, jointIndices2));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 24*sizeof(float), (void *)offsetof(Vertex, weights));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 24*sizeof(float), (void *)offsetof(Vertex, weights2));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glBindVertexArray(0);
    
    //Now generate textures
    unsigned int diffuseTexture;
    glGenTextures(1, &diffuseTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    
    unsigned char * data = stbi_load(diffuseFileName, &width, &height, &nrChannels, 4);
    if(data){
        std::cout << "Loaded texture!" << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    unsigned int specularTexture;
    glGenTextures(1, &specularTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    
    data = stbi_load(specularFileName, &width, &height, &nrChannels, 4);
    if(data){
        std::cout << "Loaded texture!" << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    ourShader.use();
    ourShader.setInt("diffuseTexture", 0);
    ourShader.setInt("specularTexture", 1);
}

void FBXModel::draw(){
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

int FBXModel::getFrameNum(){
    return rig.Joints[0].frames.size();
}

void FBXModel::ProcessTextures(FbxNode* node){
    FbxMesh * mesh = node->GetMesh();
    //Only supports one material now.
    FbxSurfaceMaterial * material = (FbxSurfaceMaterial *)node->GetSrcObject<FbxSurfaceMaterial>();
    if(material != NULL){
        FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
        FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxTexture>());
        diffuseFileName = fileTexture->GetFileName();
        prop = material->FindProperty(FbxSurfaceMaterial::sSpecular);
        fileTexture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxTexture>());
        specularFileName = fileTexture->GetFileName();
        std::cout << specularFileName << std::endl;
    }
}

FBXModel::FBXModel(const char * fileName, Shader& ourShader){
    
    FbxManager* lSdkManager = FbxManager::Create();
    
    FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);
    
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");
    
    if(!lImporter->Initialize(fileName, -1, lSdkManager->GetIOSettings())) {
        printf("Call to FBXModel::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }
    
    scene = FbxScene::Create(lSdkManager,"myScene");
    lImporter->Import(scene);
    FbxGeometryConverter geometryConverter(lSdkManager);
    geometryConverter.Triangulate(scene, true);
    lImporter->Destroy();
    
    FbxNode* rootNode = scene->GetRootNode();
    if(rootNode) {
        ProcessSkeletonHierarchy(rootNode);
        std::cout << rootNode->GetChildCount() << std::endl;
        std::cout << "root has " << rootNode->GetChildCount() << " children." << std::endl;
        for(int i = 0; i < rootNode->GetChildCount(); i++){
            //The childs all have to be mesh nodes. No SKELETON nodes.
            FbxNode * childNode = rootNode->GetChild(i);
            FbxMesh * mesh = childNode->GetMesh();
            if(mesh){
                ProcessMesh(childNode);
                ProcessTextures(childNode);
                ProcessJointsAndAnimation(childNode);
            }
        }
    }
    SetJointIndices_Weights();
    
    SetBuffers_Textures(ourShader);
    
    // Destroy the SDK manager and all the other objects it was handling.
    lSdkManager->Destroy();
}
