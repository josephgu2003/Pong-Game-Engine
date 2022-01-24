//
//  SphereRefresh.hpp
//  Pong
//
//  Created by Joseph Gu on 1/20/22.
//

#ifndef SphereRefresh_hpp
#define SphereRefresh_hpp

#include <stdio.h>
#include "PRefreshComponent.hpp"

class SphereRefresh : public PRefresh {
private:
    float radius;
    float minSpeed;
    float maxSpeed;
protected:
    void refreshParticle() override;
public:
    SphereRefresh(ParticleSystem& pe, float particleMaxDuration_, int ptcPerSecond, float refreshInterval, float radius, float minSpeed, float maxSpeed); 
};
#endif /* SphereRefresh_hpp */
