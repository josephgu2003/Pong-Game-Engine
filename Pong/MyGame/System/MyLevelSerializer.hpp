//
//  MyLevelSerializer.hpp
//  Pong
//
//  Created by Joseph Gu on 12/31/21.
//

#ifndef MyLevelSerializer_hpp
#define MyLevelSerializer_hpp

#include <stdio.h>
#include "LevelSerializer.hpp"
#include "MyActorFactory.hpp"
#include "PropFactory.hpp"

class MyLevelSerializer : public LevelSerializer {
private:
    PropFactory pf;
    MyActorFactory maf;
    void convertPrimitive(World* g, const std::string& type, int primitiveEnum, const glm::vec3& pos) override;

    inline void convertActor(World* g, int primitiveEnum, const glm::vec3& pos) {
        std::shared_ptr<Actor> a = maf.makeActor(primitiveEnum);
        a->setPos(pos);
        g->insert<Actor>(a);
    }
    inline void convertProp(World* g, int primitiveEnum, const glm::vec3& pos) {
        std::shared_ptr<Prop> a = pf.makeProp(primitiveEnum);
        a->setPos(pos);
        g->insert<Prop>(a);
    }

};

#endif /* MyLevelSerializer_hpp */
