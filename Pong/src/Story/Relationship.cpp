//
//  Relationship.cpp
//  Pong
//
//  Created by Joseph Gu on 9/22/21.
//

#include "Relationship.hpp"

Relationship::Relationship(std::string name1_, std::string name2_, float intensity, RelationShipType rst_) {
    name1 = name1_;
    name2 = name2_;
    rsIntensity = intensity;
    rst = rst_;
}

std::string Relationship::getName(RelationshipSide rs) {
    switch (rs) {
        case RS_ONE:
            return name1;
            
        case RS_TWO:
            return name2;
    }
}

RelationShipType Relationship::getType() {
    return rst;
}

float Relationship::getIntensity() {
    return rsIntensity;
}
