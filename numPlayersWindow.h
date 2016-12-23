#ifndef NUMPLAYERSWINDOW_H
#define NUMPLAYERSWINDOW_H
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
#include <stdlib.h>
#include <QDialog>
#include <QWidget>
#include <QStackedWidget>

class numPlayersWindow : public QDialog
{
  	Q_OBJECT
 public:
  	numPlayersWindow (QWidget* parent = NULL);
  	~numPlayersWindow ();

	QSize sizeHint () const { return QSize (100, 33); }

	QSize minimumSizeHint () const { return QSize (50, 25); }

	void closeEvent (QCloseEvent *event);

	int *returnNumPlayers();

  std::vector<std::string> *returnNames();


	private slots:
  	void getNumPlayers();
    void getNames();
    	//void buttonClicked ();
 
 	protected:

	private:

  	QHBoxLayout *mainLayout;
  	QLabel *prompt;
    QLabel *prompt2;
  	QPushButton *okButton;
    QPushButton *okButton2;
  	QLineEdit *enterNumPlayers;
    QLineEdit *enterName;
  	int *numPlayers;
    int compareNum;
    QStackedWidget *getPlayers;
    QHBoxLayout *askNum;
    QHBoxLayout *askName;
    QWidget *firstWidget;
    QWidget *secondWidget;
    std::vector<std::string> *names = new std::vector<std::string>;
    int count = 0;
};

#endif
