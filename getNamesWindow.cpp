#include <iostream>
#include "getNamesWindow.h"

getNamesWindow::getNamesWindow(QWidget *parent):QDialog(parent)
{
	prompt = new QLabel("What's your name?");

	enterPlayerName = new QLineEdit();

	okButton = new QPushButton("OK");
	connect (okButton, SIGNAL(clicked()), this, SLOT(getNames()));


	mainLayout = new QHBoxLayout();
	mainLayout->addWidget(prompt,1);
	mainLayout->addWidget(enterPlayerName,1);
	mainLayout->addWidget(okButton,1);

	setLayout(mainLayout);
}
getNamesWindow::~getNamesWindow(){
	delete mainLayout;
}

void getNamesWindow::getNames(){
	std::string name = (enterPlayerName->text()).toLocal8Bit().constData();
	std::cerr<<"made it";
	(*names).push_back(name);
	if(count+1 == (*numberOfPlayers)){
		close();	
	}
		enterPlayerName->setText("");
		count++;
}

void getNamesWindow::closeEvent (QCloseEvent *event)
{
  std::cout << "Bye Bye.\n";
  event->accept();
  QWidget::closeEvent(event);
}

void getNamesWindow::setNumPlayers(int *num){
	numberOfPlayers = *&num;
}

std::vector<std::string> *getNamesWindow::returnNames(){
	return names;
}