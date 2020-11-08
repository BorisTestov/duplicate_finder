#pragma once

#include <QPushButton>
#include <QWidget>

class SearchWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWindow(QWidget* parent = nullptr);

private:
    QPushButton* _searchButton;

    const QString _windowName = "duplicate_finder";
    const int _minW = 600;
    const int _minH = 400;
};