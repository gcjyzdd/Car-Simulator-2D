#include <QApplication>
#include "drawCar.h"

int main(int argc, char *argv[])
{

  QApplication app(argc, argv);

  Lines window;

  window.resize(280, 270);
  window.setWindowTitle("Lines");
  window.show();

  return app.exec();
}
