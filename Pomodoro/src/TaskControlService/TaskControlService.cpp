#include <vector>
#include <iostream>
#include <string>
namespace task
{
    /*Class der skal gemme task info, manipulere tasks og trofæer*/

    class ElClass {
        private:
            int amountOfTasks = 0;
            int trophys = 0;
            std::vector<std::string> taskName ={"hej"};

        public:
            void taskcomplete(){
                
                amountOfTasks--;
            }
            void newTrophy(){
                trophys++;
            }
            
    };
}