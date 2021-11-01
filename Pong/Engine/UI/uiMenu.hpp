//
//  uiMenu.hpp
//  Pong
//
//  Created by Joseph Gu on 10/24/21.
//

#ifndef uiMenu_hpp
#define uiMenu_hpp

#include <stdio.h>
#include "uiPiece.hpp"
#include "Observer.hpp"

class uiMenu : public uiPiece, public Observer {
protected:
    virtual void handleKeyPress(GLenum k);
    int selectedChild = 0;
public:
    uiMenu(glm::vec2 pos, glm::vec2 dimensions, const std::string& filePath);
    virtual void draw(Renderer* r) override;
    virtual void notify(const Subject& s, GameEvent ge) override;
};
#endif /* uiMenu_hpp */

// we want to have notifyable uipieces like uiMenu
// we want buttons or textboxes to have fire actions
