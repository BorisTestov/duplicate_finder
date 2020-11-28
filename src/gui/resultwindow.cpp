#include "gui/resultwindow.h"

#include <QFile>
#include <QFlags>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QStandardItemModel>

ResultWindow::ResultWindow(std::unordered_map<std::string, std::unordered_set<std::string>> duplicates)
{
    setFixedSize(_windowWidth, _windowHeight);
    setWindowTitle(_windowName);

    _duplicates = duplicates;
    createDuplicatesWidget();
    createDeleteAllButton();
    createClearSelectionButton();
    createDeleteSelectedButton();
}

void ResultWindow::createDuplicatesWidget()
{
    _duplicatesWidget = new QListWidget(this);
    _duplicatesWidget->setGeometry(0, 0, _windowWidth, _windowHeight - 50);
    loadWidgetContent();
}

void ResultWindow::loadWidgetContent()
{
    for (const auto& duplicate : _duplicates)
    {
        if (duplicate.second.empty())
        {
            continue;
        }
        _duplicatesWidget->addItem("Original: ");
        setLastItemBold();
        QString path = QString::fromStdString(duplicate.first);
        _duplicatesWidget->addItem(path);
        _duplicatesWidget->addItem("Duplicates: ");
        setLastItemBold();
        for (const auto& file : duplicate.second)
        {
            path = QString::fromStdString(file);
            _duplicatesWidget->addItem(path);
            setLastItemCheckable();
        }
        _duplicatesWidget->addItem("__________________________________________________");
    }
}

void ResultWindow::setLastItemCheckable()
{
    auto item = _duplicatesWidget->item(_duplicatesWidget->count() - 1);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
}

void ResultWindow::setLastItemBold()
{
    auto item = _duplicatesWidget->item(_duplicatesWidget->count() - 1);
    auto font = item->font();
    font.setBold(true);
    item->setFont(font);
}

void ResultWindow::createDeleteAllButton()
{
    _deleteAllButton = new QPushButton(this);
    _deleteAllButton->setText("Delete all duplicates");
    _deleteAllButton->setGeometry(350, 550, 150, 50);
    connect(_deleteAllButton.data(), SIGNAL(released()), this, SLOT(deleteAllDuplicates()));
}

void ResultWindow::createDeleteSelectedButton()
{
    _deleteSelectedButton = new QPushButton(this);
    _deleteSelectedButton->setText("Delete selected\nduplicates");
    _deleteSelectedButton->setGeometry(175, 550, 150, 50);
    connect(_deleteSelectedButton.data(), SIGNAL(released()), this, SLOT(deleteSelectedDuplicates()));
}

void ResultWindow::createClearSelectionButton()
{
    _clearSelectionButton = new QPushButton(this);
    _clearSelectionButton->setText("Clear selection");
    _clearSelectionButton->setGeometry(0, 550, 150, 50);
    connect(_clearSelectionButton.data(), SIGNAL(released()), this, SLOT(clearSelection()));
}

void ResultWindow::deleteAllDuplicates()
{
    int totalDuplicates = 0;
    for (auto& duplicate : _duplicates)
    {
        totalDuplicates += duplicate.second.size();
    }
    if (totalDuplicates == 0)
    {
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete all duplicates", QString("All duplicates (%1) will be removed.\n"
                                                                         "Continue?")
                                                                     .arg(totalDuplicates),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        for (auto& duplicate : _duplicates)
        {
            if (not duplicate.second.empty())
            {
                for (auto& path : duplicate.second)
                {
                    QString filepath = QString::fromStdString(path);
                    QFile file(filepath);
                    if (file.exists())
                    {
                        file.remove();
                    }
                }
                duplicate.second.clear();
            }
        }
        _duplicatesWidget->clear();
        createDuplicatesWidget();
    }
}

void ResultWindow::deleteSelectedDuplicates()
{
    if (_duplicatesWidget.isNull())
    {
        return;
    }
    int totalSelected = 0;
    for (int i = 0; i < _duplicatesWidget->count(); i++)
    {
        auto item = _duplicatesWidget->item(i);
        if (item->checkState() == Qt::Checked)
        {
            totalSelected++;
        }
    }
    if (totalSelected == 0)
    {
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete all duplicates", QString("All selected duplicates (%1) will be removed.\n"
                                                                         "Continue?")
                                                                     .arg(totalSelected),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        for (int i = 0; i < _duplicatesWidget->count(); i++)
        {
            auto item = _duplicatesWidget->item(i);
            if (item->checkState() == Qt::Checked)
            {
                QString filepath = item->text();
                QFile file(filepath);
                if (file.exists())
                {
                    file.remove();
                }
                for (auto& duplicate : _duplicates)
                {
                    if (duplicate.second.find(filepath.toStdString()) != duplicate.second.end())
                    {
                        duplicate.second.erase(filepath.toStdString());
                        break;
                    }
                }
            }
        }
        _duplicatesWidget->clear();
        loadWidgetContent();
    }
}

void ResultWindow::clearSelection()
{
    if (_duplicatesWidget.isNull())
    {
        return;
    }
    for (int i = 0; i < _duplicatesWidget->count(); i++)
    {
        auto item = _duplicatesWidget->item(i);
        if (item->checkState() == Qt::Checked)
        {
            item->setCheckState(Qt::Unchecked);
        }
    }
}
