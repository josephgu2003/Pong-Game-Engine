//
//  MyLevelSerializer.cpp
//  Pong
//
//  Created by Joseph Gu on 12/31/21.
//

#include "MyLevelSerializer.hpp"

void MyLevelSerializer::convertPrimitive(World* g, const std::string& type, int primitiveEnum, const glm::vec3& pos) {
    if (type == "Actor") convertActor(g, primitiveEnum, pos);
    if (type == "Prop") convertProp(g, primitiveEnum, pos);
}
