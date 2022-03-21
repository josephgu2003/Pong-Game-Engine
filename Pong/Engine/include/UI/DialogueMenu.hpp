//
//  DialogueMenu.hpp
//  Pong
//
//  Created by Joseph Gu on 10/26/21.
//

#ifndef DialogueMenu_hpp
#define DialogueMenu_hpp

#include <stdio.h>
#include <memory>
#include "uiMenu.hpp"
#include "aiDialogueAction.hpp"
class DialogueMenu : public uiMenu {
private:
    std::vector<std::string> questionsOfMenu;
    std::weak_ptr<aiDialogueAction> dialogue;
    virtual void handleKeyPress(GLenum k) override;
public:
    DialogueMenu(glm::vec2 pos, glm::vec2 dimensions, const std::string& filePath, const std::shared_ptr<aiDialogueAction>& dialogue);
};
#endif /* DialogueMenu_hpp */
 
