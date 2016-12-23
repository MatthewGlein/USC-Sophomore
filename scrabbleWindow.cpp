#include <iostream>
#include "scrabbleWindow.h"


void scrabbleWindow::readConfigFile (string config_file_name,
           string & dictionary_file_name,
           string & board_file_name,
           string & bag_file_name,
           unsigned int & hand_size)
{
  ifstream configFile (config_file_name.c_str());
  string line;
    bool number = false, brd = false, tiles = false, dictionary = false;

  while (getline (configFile, line))
  {
    stringstream ss (line);
    string parameter;
    ss >> parameter;
    if (parameter == "NUMBER:")
      { ss >> hand_size; number = true; }
    else if (parameter == "BOARD:")
        { ss >> board_file_name; brd = true; }
    else if (parameter == "TILES:")
      { ss >> bag_file_name; tiles = true; }
    else if (parameter == "DICTIONARY:")
      { ss >> dictionary_file_name; dictionary = true; }
  }
}

void scrabbleWindow::run ()
{
    string dictionaryFileName, boardFileName, bagFileName;
    unsigned int numTiles;

    readConfigFile ("config.txt",
            dictionaryFileName, boardFileName, bagFileName,
            numTiles);
    board = new Board(boardFileName);
    bag = new Bag(bagFileName, 10); // second argument is random seed

}

scrabbleWindow::scrabbleWindow (QWidget *parent) : QDialog (parent)
{
  run();
  buttonBoard = new QGridLayout();
  for(int i = 0; i < 15; i++){
    for(int j = 0; j < 15; j++){
      scrabbleButton *button = new scrabbleButton(this);
      button->setRow(i+1);
      button->setCol(j+1);
      button->setText(board->returnLetter(i,j));
      connect (button, SIGNAL(clicked()), this, SLOT(boardPressed()));
      buttonBoard->addWidget(button, j, i); 
      buttonArray[i][j] = button;
    }
  }
  setButtonColors();
  //makes layout under the board
  layout = new QGridLayout;
  passButton = new QPushButton("PASS");
  exchangeButton = new QPushButton("EXCHANGE");
  buffer = new QLabel(" ");
  horBut = new QLineEdit();
  horBut->sizePolicy().setHorizontalStretch(0);

  //Tiles
  tilesLabel = new QLabel("");
  std::string tilesNum = patch::to_string(bag->tilesRemaining());
  QString qstr = QString::fromUtf8(tilesNum.c_str());
  tilesLeftinBag = new QLabel(qstr);

  layout->addWidget(new QLabel("Tiles: "), 1, 0);
  layout->addWidget(tilesLabel, 1, 1);
  layout->addWidget(passButton, 2, 4);
  connect (passButton, SIGNAL(clicked()), this, SLOT(passPressed()));
  layout->addWidget(new QLabel("Tiles left in bag:"),1,3);
  layout->addWidget(tilesLeftinBag,1,4);

  exchangeLetters = new QLineEdit();
  layout->addWidget(new QLabel("Tiles to replace: "), 3, 0);
  layout->addWidget(exchangeLetters, 3, 1);
  layout->addWidget(buffer, 3, 2);
  layout->addWidget(exchangeButton, 3, 4);
  connect (exchangeButton, SIGNAL(clicked()), this, SLOT(exchangePressed()));
  

  placeLayout = new QHBoxLayout();
  placeButton = new QPushButton("PLACE");
  horzButton = new QPushButton("-");
  vertButton = new QPushButton("|");
  placeLetters = new QLineEdit();
  placeLayout->addWidget(new QLabel("Tiles to place: "), 1);
  placeLayout->addWidget(placeLetters, 1);
  placeLayout->addWidget(buffer, 1);
  placeLayout->addWidget(new QLabel("Direction: "), 1);
  placeLayout->addWidget(horzButton, 1);
  placeLayout->addWidget(vertButton, 1);
  placeLayout->addWidget(buffer, 1);
  placeLayout->addWidget(placeButton, 1);
  connect (placeButton, SIGNAL(clicked()), this, SLOT(placePressed()));
  connect (horzButton, SIGNAL(clicked()), this, SLOT(horzPressed()));
  connect (vertButton, SIGNAL(clicked()), this, SLOT(vertPressed()));


  mainLayout = new QVBoxLayout ();
  //mainLayout->addLayout(topBanner);
  mainLayout->addLayout (buttonBoard);
  mainLayout->addLayout(layout);
  mainLayout->addLayout(placeLayout);

  setLayout(mainLayout);
}

scrabbleWindow::~scrabbleWindow ()
{
  delete board;
  delete mainLayout;

}

void scrabbleWindow::quitPressed ()
{
  std::cout << "Quit was pressed!\n";
  close ();
}

void scrabbleWindow::boardPressed(){
  scrabbleButton *button = qobject_cast<scrabbleButton*>(sender());
  moveRow = button->getRow();
  moveCol = button->getCol(); 
}
void scrabbleWindow::passPressed(){
  passesInARow++;
  if(passesInARow == 6){
    close();
  }
  turnChange();
}
void scrabbleWindow::placePressed(){
    std::string ltr = (placeLetters->text()).toLocal8Bit().constData();
  if((moveRow != 0 && moveCol != 0) && ((toggleSideways && !toggleUpDown) 
    || (!toggleSideways && toggleUpDown)) && players[playerNum].hasAllTiles(ltr)){
    if(bag->tilesRemaining() < 0){
      errorBox(-7);
    }
    else{
      std::string row = patch::to_string(moveRow);
      std::string col = patch::to_string(moveCol);
      if(toggleSideways){
        Scored = (*board).Move("-", row, col, ltr, players[playerNum].getTiles());
      }
      else if(toggleUpDown){
        Scored = (*board).Move("|", row, col, ltr, players[playerNum].getTiles());
      }
      if(Scored <= 0){
        errorBox(Scored);
      }
      else if(Scored != 0){
        players[playerNum].exchangeTilesWhenPlacing(ltr, *bag);
        turnChange();
        if(bag->tilesRemaining() == 0){
          finalScoreWindow();
        }
      }
    }
    passesInARow = 0;   
  }
  else
    errorBox(-1);
}
void scrabbleWindow::exchangePressed(){
  passesInARow = 0;
  std::string ltr = (exchangeLetters->text()).toLocal8Bit().constData();
  if(players[playerNum].hasAllTiles(ltr) && ltr != ""){
    players[playerNum].exchangeTiles(ltr, *bag);
    turnChange();
  }
  else{
    errorBox(-1);
  }
}
void scrabbleWindow::horzPressed(){
  toggleSideways = true;
  toggleUpDown = false;
}
void scrabbleWindow::vertPressed(){
  toggleSideways = false;
  toggleUpDown = true;
}
void scrabbleWindow::closeEvent (QCloseEvent *event)
{
  std::cout << "Bye Bye.\n";
  event->accept();
  QWidget::closeEvent (event);
}
void scrabbleWindow::setPlayerNames(std::vector<std::string> *playerNames){
  names = playerNames;
    //adds Banner below board
  topBanner = new QHBoxLayout();
  currPlayer = new QLabel();

  scoreboard = new QGridLayout();
  topBanner->addWidget(new QLabel("Current Player: "), 1);
  topBanner->addWidget(currPlayer, 1);
  topBanner->addWidget(new QLabel(" "), 1);
  topBanner->addWidget(new QLabel("Scores: "), 1);
  for(int i = 0; i < (int)((*names).size()); i++){
    std::string nm = (*names)[i];
    QString qstr2 = QString::fromUtf8(nm.c_str());
    scoreboard->addWidget(new QLabel(qstr2), 0, i);
    Player * player = new Player(nm, 0, bag->drawTiles(7));
    players.push_back(*player);

    std::string str = players[i].getScoreAsString();
    QString qstr = QString::fromUtf8(str.c_str());
    scoreLabel = new QLabel(qstr);
    scoreLabels.push_back(scoreLabel);
    scoreboard->addWidget(scoreLabel, 1, i);
    if(i == 0)
      currPlayer->setText(qstr2);
  }
  topBanner->addLayout(scoreboard, 1);
  std::string str = players[0].printTiles();
  QString qstr = QString::fromStdString(str);
  tilesLabel->setText(qstr);
  quitButton = new QPushButton ("Quit");
  topBanner->addWidget(quitButton);

  connect (quitButton, SIGNAL(clicked()), this, SLOT(quitPressed()));
  mainLayout->addLayout(topBanner);
  updateButtons();
}
void scrabbleWindow::setNumPlayers(int *num){
  numPlayers = num;
}
void scrabbleWindow::updateButtons(){
  for(int i = 0; i < 15; i++){
    for(int j = 0; j < 15; j++){
      /*
        QLayoutItem *helper = buttonBoard->itemAtPosition(i,j);
        QWidget *helperW = helper->widget();
      */
        std::string str = board->returnLetter(i,j);
        QString qstr = QString::fromStdString(str);
        buttonArray[i][j]->setText(qstr);;
        if(str != "." && str != "d" && str != "t" && str != "2" && str != "3"){
          QPalette pal = buttonArray[i][j]->palette();
          pal.setColor(QPalette::Button, QColor(Qt::red));
          buttonArray[i][j]->setAutoFillBackground(true);
          buttonArray[i][j]->setPalette(pal);
          buttonArray[i][j]->update();
        }
    }
  }
  std::string str = patch::to_string(bag->tilesRemaining());
  QString qstr = QString::fromStdString(str);
  tilesLeftinBag->setText(qstr);

  str = players[playerNum].getName();
  qstr = QString::fromStdString(str);
  currPlayer->setText(qstr);

  str = players[playerNum].printTiles();
  qstr = QString::fromStdString(str);
  tilesLabel->setText(qstr);

  qstr = QString::fromStdString("");
  exchangeLetters->setText(qstr);

  qstr = QString::fromStdString("");
  placeLetters->setText(qstr);

  Scored = 0;

}
void scrabbleWindow::turnChange(){
    std::string str = patch::to_string(Scored + players[playerNum].getCurrScore());
    players[playerNum].addScore(Scored);
    QString qstr = QString::fromStdString(str);
    scoreLabels[playerNum]->setText(qstr);
  if(playerNum == players.size() - 1){
    playerNum = 0;
  }
  else if(playerNum < players.size()){
    playerNum++;
  }
  updateButtons();
}
void scrabbleWindow::errorBox(int command){
  QMessageBox::StandardButton reply;
  if(command == -1){
    reply = QMessageBox::warning(this, "ERROR", 
      "You don't have those tiles", QMessageBox::Ok);   
  }
  if(command == -2){
    reply = QMessageBox::warning(this, "ERROR", 
      "Not a valid word", QMessageBox::Ok);   
  }
  if(command == -3){
    reply = QMessageBox::warning(this, "ERROR", 
      "Your word must connect with another", QMessageBox::Ok);   
  }
  if(command == -4){
    reply = QMessageBox::warning(this, "ERROR", 
      "One letter must be on the middle tile", QMessageBox::Ok);   
  }
  if(command == -5){
    reply = QMessageBox::warning(this, "ERROR", 
      "Out of Bounds", QMessageBox::Ok); 
  }
  if(command == -6){
    reply = QMessageBox::warning(this, "ERROR", 
      "There is already a letter at your first letter", QMessageBox::Ok);   
  }
  
}

void scrabbleWindow::finalScoreWindow(){
  QMessageBox::StandardButton reply;
  int max = 0;
  int maxSpot;
  for(int i = 0; i < players.size(); i++){
      if(players[i].getCurrScore() > max){
        maxSpot = i;
        max = players[i].getCurrScore();
      }
  }
  std::string scrString = "Winner: " + players[maxSpot].getName() + " : " + patch::to_string(max) + "points";
  QString qstr = QString::fromStdString(scrString);
  reply = QMessageBox::information(this, "Game Over", 
  qstr, QMessageBox::Ok); 
  if (reply == QMessageBox::Ok) {
    qDebug() << "Yes was clicked";
    close();
  } else {
    qDebug() << "Yes was *not* clicked";
  }
}

void scrabbleWindow::setButtonColors(){
  for(int i = 0; i < 15; i++){
    for(int j = 0; j < 15; j++){
      QPalette pal = buttonArray[i][j]->palette();
      if(board->returnLetter(i,j) == "."){
        pal.setColor(QPalette::Button, QColor(Qt::gray));  
      }
      if(board->returnLetter(i,j) == "d"){
        pal.setColor(QPalette::Button, QColor(Qt::blue));  
      }
      if(board->returnLetter(i,j) == "t"){
        pal.setColor(QPalette::Button, QColor(Qt::green));  
      }
      if(board->returnLetter(i,j) == "3"){
        pal.setColor(QPalette::Button, QColor(Qt::cyan));  
      }
      if(board->returnLetter(i,j) == "2"){
        pal.setColor(QPalette::Button, QColor(Qt::yellow));  
      }
      buttonArray[i][j]->setAutoFillBackground(true);
      buttonArray[i][j]->setPalette(pal);
      buttonArray[i][j]->update();
    }
  }
}