#include <QApplication>
#include <iostream>
#include "scrabbleWindow.h"
#include "numPlayersWindow.h"
#include "getNamesWindow.h"

int main (int argc, char *argv[])
{
  QApplication app (argc, argv);

  numPlayersWindow* np = new numPlayersWindow();
  np->exec();
  int *numPlayers = np->returnNumPlayers();
  std::vector<std::string> *playerNames = np->returnNames();

  scrabbleWindow* sw = new scrabbleWindow();
  sw->setPlayerNames(playerNames);
  sw->setNumPlayers(numPlayers);
  sw->exec();

  

  return app.exec ();
}

