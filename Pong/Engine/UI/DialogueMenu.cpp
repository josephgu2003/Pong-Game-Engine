//
//  DialogueMenu.cpp
//  Pong
//
//  Created by Joseph Gu on 10/26/21.
//

#include "DialogueMenu.hpp"
#include "uiText.hpp"

DialogueMenu::DialogueMenu(glm::vec2 pos, glm::vec2 dimensions, const std::string& filePath,const std::shared_ptr<aiDialogueAction>& d) : uiMenu(pos, dimensions, filePath) {
    dialogue = d;
    std::vector<QA> v = d->getQuestionsForTarget();
    for (int i = 0; i < v.size(); i++) {
        questionsOfMenu.push_back(v.at(i).question);
        insertChild(std::make_shared<uiText>(v.at(i).question, pos.x+dimensions.x*0.1, pos.y+dimensions.y-0.2-0.1*i));
    }
} 
 

void DialogueMenu::handleKeyPress(GLenum k) {
   switch (k) { 
       case GLFW_KEY_DOWN: {
           std::static_pointer_cast<uiText>(children.at(selectedChild))->hover(false);
           int newIndex = selectedChild + 1;
           
           if (newIndex < children.size()) {
               selectedChild = newIndex;
           } else {
               selectedChild = 0;
           }
           // hover over selectedChild
           std::static_pointer_cast<uiText>(children.at(selectedChild))->hover(true);
           break;
       }
       case GLFW_KEY_ENTER: {
           if (std::shared_ptr<aiDialogueAction> x = dialogue.lock()) {
               x->askQuestion(questionsOfMenu.at(selectedChild));
           }
           break;
       }
            
       case GLFW_KEY_UP: {
           std::static_pointer_cast<uiText>(children.at(selectedChild))->hover(false);
           int newIndex = selectedChild-1;
           
           if (newIndex >= 0 ) {
               selectedChild = newIndex;
           } else {
               selectedChild = children.size()-1;
           }
           std::static_pointer_cast<uiText>(children.at(selectedChild))->hover(true);
           break;
       }
   }
}
