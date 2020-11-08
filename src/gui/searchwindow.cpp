#include "gui/searchwindow.h"

SearchWindow::SearchWindow(QWidget* parent)
{
    setParent(parent);
    setMinimumSize(_minW, _minH);
    setWindowTitle(_windowName);
    _searchButton = new QPushButton("Hi", this);
    _searchButton->setGeometry(10, 10, 80, 30);
}