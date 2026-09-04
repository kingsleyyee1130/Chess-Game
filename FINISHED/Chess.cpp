#include <iostream>
#include <windows.h>
#include <conio.h>

#include "gameFlow.h"


int main()
{
    // Enable UTF-8 mode in Windows Console for chess symbols
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int choice;

    showWelcomeScreen();

    while(true)
    {
        choice = showMainMenu();

        switch(choice)
        {
            case 1:
                startNewGame();
                break;

            case 2:
                continueGame();
                break;

            case 3:
                chooseStatistic();
                break;
            
            case 4:
                exitScreen();
                return 0;
        }
    }

    return 0;
}

