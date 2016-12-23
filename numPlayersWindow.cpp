#include <iostream>
#include "numPlayersWindow.h"

numPlayersWindow::numPlayersWindow(QWidget *parent):QDialog(parent)
{
	getPlayers = new QStackedWidget();

	prompt = new QLabel("How many players are in your game?");

	enterNumPlayers = new QLineEdit();

	okButton = new QPushButton("OK");
	connect (okButton, SIGNAL(clicked()), this, SLOT(getNumPlayers()));

	firstWidget = new QWidget();
	askNum = new QHBoxLayout();
	askNum->addWidget(prompt,1);
	askNum->addWidget(enterNumPlayers,1);
	askNum->addWidget(okButton,1);
	firstWidget->setLayout(askNum);

	enterName = new QLineEdit();
	okButton2 = new QPushButton("OK");
	prompt2 = new QLabel("What is your name?");
	secondWidget = new QWidget();
	askName = new QHBoxLayout();
	askName->addWidget(prompt2,1);
	askName->addWidget(enterName,1);
	askName->addWidget(okButton2,1);
	connect (okButton2, SIGNAL(clicked()), this, SLOT(getNames()));
	secondWidget->setLayout(askName);

	getPlayers->addWidget(firstWidget);
	getPlayers->addWidget(secondWidget);

	mainLayout = new QHBoxLayout();
	mainLayout->addWidget(getPlayers);

	setLayout(mainLayout);
}
numPlayersWindow::~numPlayersWindow(){
	delete getPlayers;
}

void numPlayersWindow::getNumPlayers(){
	std::string stringPlayers = (enterNumPlayers->text()).toLocal8Bit().constData();
	int nPlayers = atoi(stringPlayers.c_str());
	compareNum = nPlayers;
	numPlayers = &nPlayers;
	
	getPlayers->setCurrentIndex(1);

}

void numPlayersWindow::getNames(){
	std::string name = (enterName->text()).toLocal8Bit().constData();
	(*names).push_back(name);
	if(count+1 == (compareNum)){
		close();	
	}
		enterName->setText("");
		count++;
}

int *numPlayersWindow::returnNumPlayers(){
	return &compareNum;
}
std::vector<std::string> *numPlayersWindow::returnNames(){
	return names;
}

void numPlayersWindow::closeEvent (QCloseEvent *event)
{
  std::cout << "Bye Bye.\n";
  event->accept();

  QWidget::closeEvent(event);
}