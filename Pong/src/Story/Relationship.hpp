//
//  Relationship.hpp
//  Pong
//
//  Created by Joseph Gu on 9/22/21.
//

#ifndef Relationship_hpp
#define Relationship_hpp

#include <stdio.h>
#include <string>
 
enum RelationshipSide {
    RS_ONE,
    RS_TWO
};

enum RelationShipType {
    RS_EMPTY,
    RS_FRIEND,
    RS_ENEMY
};

class Relationship {
private:
    std::string name1; 
    std::string name2;
    float rsIntensity;
    RelationShipType rst;
public:
    Relationship(std::string name1, std::string name2, float intensity, RelationShipType rst);
    std::string getName(RelationshipSide rs);
    RelationShipType getType();
    float getIntensity();
};

#endif /* Relationship_hpp */
