//
//  Relationship.cpp
//  Pong
//
//  Created by Joseph Gu on 9/22/21.
//

#include "Relationship.hpp"

Relationship::Relationship(std::string name1_, std::string name2_, float intensity, RelationShipType rst_, std::string nn1_, std::string nn2_) {
    name1 = name1_;
    name2 = name2_;
    rsIntensity = intensity;
    rst = rst_;
    nn1 = nn1_;
    nn2 = nn2_;
    switch (rst) { 
        case RS_FRIEND:{
            questionsFor1.push_back(QA("How are you today?", "Great, thanks!"));
            questionsFor1.push_back(QA("How much do you love me?", "Quite alot"));
            questionsFor1.push_back(QA("Am I cool?", "Yes"));
            questionsFor2.push_back(QA("How are you today?", "Great, thanks!"));
            questionsFor2.push_back(QA("How much do you love me?", "Quite alot"));
            questionsFor2.push_back(QA("Am I cool?", "Yes"));
            break;
        }
            
        case RS_ENEMY : {
            questionsFor1.push_back(QA("Why are you here?", "None of your business..."));
            questionsFor1.push_back(QA("Do you want to kill me?", "With an intensity of " + std::to_string(rsIntensity)));
            questionsFor1.push_back(QA("Am I cool?", "No"));
            questionsFor2.push_back(QA("Why are you here?", "None of your business..."));
            questionsFor2.push_back(QA("Do you want to kill me?", "With an intensity of " + std::to_string(rsIntensity)));
            questionsFor2.push_back(QA("Am I cool?", "No"));
            break;
        }
        case RS_EMPTY : {
            questionsFor1.push_back(QA("How are you?", "Fine..."));
            questionsFor1.push_back(QA("What's your name?", name1));
            questionsFor1.push_back(QA("Am I cool?", "Maybe"));
            questionsFor2.push_back(QA("How are you?", "Fine..."));
            questionsFor2.push_back(QA("What's your name?", name2));
            questionsFor2.push_back(QA("Am I cool?", "Maybe"));
            break;
        }
    }
} 

std::string Relationship::getName(RelationshipSide rs) {
    switch (rs) {
        case RS_ONE:
            return name1;
            
        case RS_TWO:
            return name2;
    }
}


std::string Relationship::getNickName(const std::string& myName) {
    if (myName == name1) {
        return nn2;
    } else if (myName == name2) {
        return nn1;
    }
    return "";
}


RelationShipType Relationship::getType() {
    return rst;
}

float Relationship::getIntensity() {
    return rsIntensity;
}
std::vector<QA> Relationship::getQuestionsFor(std::string idname) {
    if (idname == name1) {
        return questionsFor1;
    } else if (idname == name2){
        return questionsFor2;
    } else {
        std::vector<QA> v = {QA("Who are you?", "Not telling u"), QA("Can we be friends?", "Maybe..."), QA("Will you fight me?", "Possibly...")};
        return v;
    } 
}
 
