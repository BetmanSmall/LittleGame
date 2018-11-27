#include <QApplication>
#include <QIcon>

#include "src/head/widgetcontroller.h"

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

    QRect rec = QApplication::desktop()->screenGeometry();
//     height = rec.height();
    int width = rec.width();

    WidgetController w;
    w.move(width/4, 0);
    w.show();

    return a.exec();
}
