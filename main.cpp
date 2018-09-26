#include <QApplication>

#include "widgetcontroller.h"

int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    paths.append("sqldrivers");
    QCoreApplication::setLibraryPaths(paths);

    QApplication a(argc, argv);

//    QRect rec = QApplication::desktop()->screenGeometry();
    WidgetController w;
    w.move(0, 0);
    w.show();

    return a.exec();
}
