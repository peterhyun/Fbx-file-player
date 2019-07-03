//
//  FBXModel.hpp
//  FBXLoader
//
//  Created by Jeehoon Hyun on 11/06/2019.
//  Copyright © 2019 Jeehoon Hyun. All rights reserved.
//

#ifndef FBXModel_hpp
#define FBXModel_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fbxsdk.h>
#include <vector>
#include <map>
#include <iostream>
#include <Shader.h>

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 UV;
    glm::vec4 jointIndices = glm::vec4(0.0);
    glm::vec4 jointIndices2 = glm::vec4(0.0);
    glm::vec4 weights = glm::vec4(0.0);
    glm::vec4 weights2 = glm::vec4(0.0);
};

struct Joint_Weight{
    double weight;
    int JointIndex;
    Joint_Weight(double weight, int JointIndex): weight(weight), JointIndex(JointIndex){
        
    };
};

//With no doubt one of the most important class.
struct Joint{
    int mParentIndex;
    const char * mName;
    glm::mat4 globalBindPoseInverse;
    FbxNode* node;
    int * controlPointIndices;
    double * weights;
    //Stores animation data here.
    std::vector<glm::mat4> frames;
};

struct Skeleton{
    std::vector<Joint> Joints;
};

namespace glm{
    inline bool operator<(const glm::vec3& lhs, const glm::vec3& rhs)
    {
        return lhs.x < rhs.x ||
        (lhs.x == rhs.x && (lhs.y < rhs.y || lhs.y == rhs.y && lhs.z < rhs.z));
    }
}

class FBXModel{
private:
    std::vector<Vertex> vertices;
    unsigned int VAO, VBO;
    Skeleton rig;
    void ReadPosition(FbxMesh* mesh, int ctrlPointIndex, glm::vec3& position);
    void ReadNormal(FbxMesh* mesh, int inCtrlPointIndex, int inVertexCounter, glm::vec3& outNormal);
    void ProcessTextures(FbxNode* node);
    void ProcessMesh(FbxNode* node);
    void ReadUV(FbxMesh* mesh, int inCtrlPointIndex, int textureUVIndex, glm::vec2& UV);
    void ProcessSkeletonHierarchy(FbxNode* node);
    void ProcessSkeletonHierarchyRecursively(FbxNode* node, int myindex, int ParentIndex);
    void ProcessJointsAndAnimation(FbxNode* node);
    int findJointIndexUsingName(const char * jointName);
    FbxScene* scene;
    std::multimap<glm::vec3, Joint_Weight> map;
    void SetBuffers_Textures(Shader& ourShader);
    void SetJointIndices_Weights();
    const char * diffuseFileName;
    const char * specularFileName;
    //Map for each control point -> Find the joints_weight pair affecting it.
public:
    FBXModel(const char * filename, Shader& ourShader);
    void SetGlobalBindInverseMatrices(Shader& ourShader);
    void updateAnimation(Shader& ourShader, int frameIndex);
    void draw();
    int getFrameNum();
};


#endif /* FBXModel_hpp */
