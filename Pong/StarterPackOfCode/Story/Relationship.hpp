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
#include <vector>
 
enum RelationshipSide {
    RS_ONE,
    RS_TWO
};

enum RelationShipType {
    RS_EMPTY,
    RS_FRIEND,
    RS_ENEMY
};

struct QA {
    std::string question;
    std::string answer;
    QA(std::string q, std::string a) { question = q; answer = a;}
};
class Relationship {
private:
    std::string name1; 
    std::string name2;
    float rsIntensity;
    RelationShipType rst;
    std::string nn1;
    std::string nn2;
    std::vector<QA> questionsFor1;
    std::vector<QA> questionsFor2;
public:
    Relationship(std::string name1, std::string name2, float intensity, RelationShipType rst, std::string nn1, std::string nn2);
    std::string getName(RelationshipSide rs);
    std::string getNickName(const std::string& myName);
    RelationShipType getType();
    float getIntensity();
    std::vector<QA> getQuestionsFor(std::string idname);
};

#endif /* Relationship_hpp */
