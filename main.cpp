#include <QApplication>
#include <QIcon>

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
    srand(time(0));

    WidgetController w;
    w.move(0, 0);
    w.show();

    return a.exec();
}
