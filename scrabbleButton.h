#include <QPushButton>
#include <QString>

class scrabbleButton : public QPushButton
{
  Q_OBJECT

 public:
  scrabbleButton (QWidget *parent = NULL);

  QSize sizeHint () const { return QSize (100, 100); }

  QSize minimumSizeHint () const { return QSize (50, 20); }

  int getRow () { return row; }
  int getCol() 	{ return col;  }

  void setRow (int rw) { row = rw;}
  void setCol (int cl) { col = cl;}

  void setText(std::string str){QString qstr = QString::fromStdString(str);
  	QPushButton::setText(qstr); repaint();}

 protected:
  //void paintEvent (QPaintEvent *event);

 private:
  std::string string;
  int row;
  int col;
};
