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
    /**
     * @brief Конструктор окна с результатами поиска
     * @param duplicates - результаты поиска
     */
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

    /**
     * @brief Создать виджет со списком дубликатов
     */
    void createDuplicatesWidget();

    /**
     * @brief Загрузить список дубликатов в виджет
     */
    void loadWidgetContent();

    /**
     * @brief Добавить чекбокс в последний элемент виджета
     */
    void setLastItemCheckable();

    /**
     * @brief Сделать текст последнего элемента виджета жирным
     */
    void setLastItemBold();

    /**
     * @brief Создать кнопку для удаления всех дубликатов
     */
    void createDeleteAllButton();

    /**
     * @brief Создать кнопку для удаления выбранных дубликатов
     */
    void createDeleteSelectedButton();

    /**
     * @brief Создать кнопку для очищения выбора
     */
    void createClearSelectionButton();

private slots:
    /**
     * @brief Удалить все дубликаты
     */
    void deleteAllDuplicates();

    /**
     * @brief Удалить выбранные дубликаты
     */
    void deleteSelectedDuplicates();

    /**
     * @brief Очистить выбор
     */
    void clearSelection();
};