#include <QApplication>
#include <QSurfaceFormat>

#include "CMainWindow.hpp"

int main(int argc, char** argv)
{
    QApplication Application(argc, argv);
    QSurfaceFormat SurfaceFormat;
    SurfaceFormat.setVersion(4, 3);
    SurfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(SurfaceFormat);

    CMainWindow MainWindow;
    MainWindow.show();

    return Application.exec();
}
