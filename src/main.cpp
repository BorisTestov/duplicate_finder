#include "gui/searchwindow.h"
#include "version.h"

#include <QApplication>
#include <iostream>
#include <map>

int main(int argc, char** argv)
{
    std::cout << "Current version: " << version() << std::endl;
    QApplication app(argc, argv);
    SearchWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
