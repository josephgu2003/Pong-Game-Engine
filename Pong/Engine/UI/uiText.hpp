//
//  uiText.hpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#ifndef uiText_hpp
#define uiText_hpp

#include <stdio.h>
#include "uiPiece.hpp"

inline float DEFAULT_FONTSIZE = 0.0003f;
inline float DEFAULT_LINESPACE = 233.0f;
 
class Game; 

typedef void (*fireCallback) (Game*);

class uiText : public uiPiece {
private:
    Game* g = nullptr; //spaggetttiiiii
    fireCallback callback;
    float fontsize;
    float linespace;
    float maxLineLength;
public: 
    uiText(const std::string& s, float x, float y, float fontsize, float linespace);
    uiText(const std::string& s, float x, float y, float fontsize, float linespace, float maxLineLength);
    void setFireCallback(fireCallback f, Game *g );
    void setText(const std::string& s);
    void hover(bool b);
    void fire();
};
#endif /* uiText_hpp */
