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
class Relationship {
private:
    std::string name1; 
    std::string name2;
    float rsIntensity;
public:
    Relationship(std::string name1, std::string name2);
    std::string getName(RelationshipSide rs);
};

#endif /* Relationship_hpp */
