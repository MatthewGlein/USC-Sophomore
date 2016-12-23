#ifndef GETNAMESWINDOW_H
#define GETNAMESWINDOW_H
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
#include "numPlayersWindow.h"

class getNamesWindow : public QDialog
{
  Q_OBJECT

 public:
  getNamesWindow (QWidget* parent = NULL);
  ~getNamesWindow ();

  QSize sizeHint () const { return QSize (300, 100); }

  QSize minimumSizeHint () const { return QSize (200, 150); }

  void closeEvent (QCloseEvent *event);

  void setNumPlayers(int *num);

  std::vector<std::string> *returnNames();

  private slots:
    void getNames();
    //void buttonClicked ();
 
 protected:

 private:
  QHBoxLayout *mainLayout;
  QLabel *prompt;
  QLineEdit *enterPlayerName;
  QPushButton *okButton;
  int *numberOfPlayers;//numPlayersWindow.returnNumPlayers();
  std::vector<std::string> *names = new std::vector<std::string>;
  int count = 0;
};

#endif
