#include <QApplication>
#include "MainWindow.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QColor firstColor = Qt::black;
    QColor secondColor = Qt::white;

    MainWindow window;
    window.Draw();
    window.Update(firstColor, secondColor);
    window.show();

    return app.exec();
}

