#pragma once

#include <QListWidget>
#include <QPointer>
#include <QPushButton>
#include <QWidget>
#include <unordered_map>
#include <unordered_set>

class ResultWindow : public QWidget
{
    Q_OBJECT
public:
    ResultWindow(std::unordered_map<std::string, std::unordered_set<std::string>> duplicates);

private:
    std::unordered_map<std::string, std::unordered_set<std::string>> _duplicates;

    const QString _windowName = "duplicate_finder_results";
    const int _windowWidth = 500;
    const int _windowHeight = 600;

    QPointer<QListWidget> _duplicatesWidget;
    QPointer<QPushButton> _deleteAllButton;
    QPointer<QPushButton> _deleteSelectedButton;
    QPointer<QPushButton> _clearSelectionButton;

    void createDuplicatesWidget();
    void readWidgetContent();
    void setLastItemCheckable();
    void setLastItemBold();
    void createDeleteAllButton();
    void createDeleteSelectedButton();
    void createClearSelectionButton();

private slots:
    void deleteAllDuplicates();
    void deleteSelectedDuplicates();
    void clearSelection();
};