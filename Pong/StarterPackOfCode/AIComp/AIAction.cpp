//
//  AIAction.cpp
//  Pong
//
//  Created by Joseph Gu on 10/2/21.
//

#include "AIAction.hpp"

aiAction::aiAction(float interest_) {
    interest = interest_;
}


bool aiAction::isDone() {
    if (interest <= 0.0) {
        return true;
    }
    return false;
}
