#include "version.h"

#include <QApplication>
#include <QPushButton>
#include <iostream>
#include <map>
#include <vector>

int main(int argc, char** argv)
{
    std::cout << "Current version: " << version() << std::endl;
    QApplication app(argc, argv);
    QPushButton button("Hello world !");
    button.setText("My text");
    button.setToolTip("A tooltip");
    button.show();
    return app.exec();
    return 0;
}
