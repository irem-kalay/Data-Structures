#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iterator>
#include "doublelinklist.h"
#include "objects.h"
#include <cstdlib> 


using namespace std;

//I create a stack that stores Gamestates
struct Stack    
{
    DoublyList<GameState> successful_states;
    GameState pop();
    void push(GameState);
};

GameState Stack::pop()
{
    GameState the_state = successful_states.get(0);
    successful_states.removeFront();
    return the_state;
}

void Stack::push(GameState new_state)
{
    successful_states.addFront(new_state);
}


Stack my_stack;    //Declaring the stack which has the type of GameStack

int main()
{
    GameState first_state;
    first_state.create_init_state();
    //Initial state was added to stack
    my_stack.push(first_state); 

    int action, object;

    while (1)
    {
        first_state.print_situation();

        //Creating Random action and object
        action = (rand() %5) + 1;
        int max_object = first_state.rooms.get(first_state.room_id)->room_objects.elemcount - 1;  //Checks the amount of objects in that room
        object = rand() % (max_object + 1);

        GameState next_state = first_state;

        int result = next_state.advance(action, object);  //checks the state

        //If the result of advance function is a positive integer, push that successful state to stack
        if(result > 0)
        {
            my_stack.push(next_state);  //Pushes the successfull state to my_stack
        }
        first_state = next_state;

        //If the current state looses the game, pop the previous state and continue trying a way
        if(result == -1)
        {
            first_state = my_stack.pop();
        }
       //If it finds out the way to escape, get out from the loop
        if(first_state.win)
        {
            break;
        }
    }


    return 0;

}
