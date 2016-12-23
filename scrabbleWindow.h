#ifndef SCRABBLEWINDOW_H
#define SCRABBLEWINDOW_H
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QCloseEvent>
#include <QString>
#include <QDialog>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>
#include "Player.h"
#include "Bag.h"
#include "scrabbleButton.h"
#include "Board.h"
#include "Dictionary.h"
#include <fstream>
#include <QMessageBox>


class scrabbleWindow : public QDialog
{
  Q_OBJECT

 public:
  scrabbleWindow (QWidget* parent = NULL);
  ~scrabbleWindow ();

  QSize sizeHint () const { return QSize (200, 75); }

  QSize minimumSizeHint () const { return QSize (100, 40); }

  void closeEvent (QCloseEvent *event);

  void setPlayerNames(std::vector<std::string> *playerNames);

  void setNumPlayers(int *num);

  void updateButtons();

  void turnChange();

  void errorBox(int command);

  void setButtonColors();

  void finalScoreWindow();

  void readConfigFile (string config_file_name,
           string & dictionary_file_name,
           string & board_file_name,
           string & bag_file_name,
           unsigned int & hand_size);

  void run();

  private slots:
    void quitPressed();
    void boardPressed();
    void passPressed();
    void placePressed();
    void exchangePressed();
    void horzPressed();
    void vertPressed();
    //void buttonClicked ();
 
 protected:

 private:
  QPushButton *buttonArray[100][100];
  QVBoxLayout *mainLayout;
  QWidget *centralWidget;
  QGridLayout *buttonBoard;
  QHBoxLayout *topBanner;
  QGridLayout *formGroupBox;
  QGridLayout *layout;
  QGridLayout *scoreboard;
  QHBoxLayout *placeLayout;
  QPushButton *quitButton;
  QPushButton *passButton;
  QPushButton *exchangeButton;
  QPushButton *placeButton;
  QPushButton *horzButton;
  QPushButton *vertButton;
  QLineEdit *placeLetters;
  QLineEdit *horBut;
  QLineEdit *exchangeLetters;
  QLabel *buffer;
  QLabel *scoreLabel;
  QLabel *tilesLeftinBag;
  std::vector<std::string> *names;
  std::vector<QLabel*> scoreLabels;
  int *numPlayers;
  std::vector<Player> players;
  QLabel *tilesLabel;
  Bag *bag;
  QLabel *currPlayer;
  int passesInARow = 0;
  QMessageBox *falseMove;
  bool toggleSideways = false;
  bool toggleUpDown = false;
  int moveRow = 0;
  int moveCol = 0;
  Board *board;
  int playerNum = 0;
  int Scored;

};

#endif
