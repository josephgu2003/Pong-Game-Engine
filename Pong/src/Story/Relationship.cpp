//
//  Relationship.cpp
//  Pong
//
//  Created by Joseph Gu on 9/22/21.
//

#include "Relationship.hpp"

Relationship::Relationship(std::string name1_, std::string name2_) {
    name1 = name1_;
    name2 = name2_;
}

std::string Relationship::getName(RelationshipSide rs) {
    switch (rs) {
        case RS_ONE:
            return name1;
            
        case RS_TWO:
            return name2;
    }
}
