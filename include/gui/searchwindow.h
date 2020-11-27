#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPointer>
#include <QPushButton>
#include <QRegExpValidator>
#include <QWidget>

class SearchWindow : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор окна с настройками поиска
     */
    SearchWindow();

private:
    /**
     * @brief Открыть окно с выбором директоорий
     * @return Список выбранных директорий
     */
    static QStringList chooseDirectories();

    /**
     * @brief Создать панель просмотра выбранных директорий для поиска
     */
    void createIncludeDirectoriesView();

    /**
     * @brief Создать текст для обозначения блока настроек выбранных директорий для поиска
     */
    void createIncludeDirectoriesLabel();

    /**
     * @brief Создать кнопку для добавления новых директорий для поиска
     */
    void createIncludeDirectoriesButton();

    /**
     * @brief Создать панель просмотра исключеннных из поиска директорий
     */
    void createExcludeDirectoriesView();

    /**
     * @brief Создать текст для обозначения блока настроек исключеннных из поиска директорий
     */
    void createExcludeDirectoriesLabel();

    /**
     * @brief Создать кнопку для добавления новой директории в список исключенных из поиска
     */
    void createExcludeDirectoriesButton();

    /**
     * @brief Создать панель просмотра включенных в поиск масок имен файлов
     */
    void createIncludeMasksView();

    /**
     * @brief Создать текст для обозначения блока настроек включенных в поиск масок имен файлов
     */
    void createIncludeMasksLabel();

    /**
     * @brief Создать поле ввода включенной в поиск маски имен файлов
     */
    void createIncludeMasksEdit();

    /**
     * @brief Создать кнопку для добавления включенной в поиск маски имен файлов
     */
    void createIncludeMasksButton();

    /**
     * @brief Создать текст-ссылку для открытия справки с описанием синтаксиса регулярных выражений
     */
    void createIncludeMasksLink();

    /**
     * @brief Создать панель просмотра исключенных из поиска масок имен файлов
     */
    void createExcludeMasksView();

    /**
     * @brief Создать текст для обозначения блока настроек исключенных из поиска масок имен файлов
     */
    void createExcludeMasksLabel();

    /**
     * @brief Создать поле ввода исключенной из поиска маски имен файлов
     */
    void createExcludeMasksEdit();

    /**
     * @brief Создать кнопку для добавления исключенной из поиска маски имен файлов
     */
    void createExcludeMasksButton();

    /**
     * @brief Создать текст-ссылку для открытия справки с описанием синтаксиса регулярных выражений
     */
    void createExcludeMasksLink();

    /**
     * @brief Создать чекбокс для выбора поиска по хешу
     */
    void createHashSearchBox();

    /**
     * @brief Создать комбобокс для выбора функции хеширования
     */
    void createPossibleHashesComboBox();

    /**
     * @brief Создать чекбокс для выбора поиска по метаданным
     */
    void createMetaSearchBox();

    /**
     * @brief Создать кнопку для отображения/скрытия дополнительных настроек
     */
    void createAdditionalSettingsButton();

    /**
     * @brief Создать текст для обозначения настройки глубины поиска
     */
    void createSearchDepthLabel();

    /**
     * @brief Создать поле ввода значения глубины поиска
     */
    void createSearchDepthEdit();

    /**
     * @brief Создать текст для обозначения настройки минимального размера файла
     */
    void createMinFileSizeLabel();

    /**
     * @brief Создать поле ввода значения минимального размера файла
     */
    void createMinFileSizeEdit();

    /**
     * @brief Создать комбобокс для выбора единиц измерения вводимого значения минимального размера файла
     */
    void createMinFileSizeDimensionComboBox();

    /**
     * @brief Создать кнопку поиска
     */
    void createSearchButton();

    /**
     * @brief Создать визуальные разделители
     */
    void createSeparators();

    QPointer<QListWidget> _includeDirectoriesView;
    QPointer<QLabel> _includeDirectoriesLabel;
    QPointer<QPushButton> _includeDirectoriesButton;

    QPointer<QListWidget> _excludedDirectoriesView;
    QPointer<QLabel> _excludedDirectoriesLabel;
    QPointer<QPushButton> _excludeDirectoriesButton;

    QPointer<QListWidget> _includeMasksView;
    QPointer<QLabel> _includeMasksLabel;
    QPointer<QLineEdit> _includeMasksEdit;
    QPointer<QPushButton> _includeMasksButton;
    QPointer<QLabel> _includeMasksLink;

    QPointer<QListWidget> _excludeMasksView;
    QPointer<QLabel> _excludeMasksLabel;
    QPointer<QLineEdit> _excludeMasksEdit;
    QPointer<QPushButton> _excludeMasksButton;
    QPointer<QLabel> _excludeMasksLink;

    QPointer<QCheckBox> _hashSearchBox;
    QPointer<QComboBox> _possibleHashesComboBox;

    QPointer<QCheckBox> _metaSearchBox;

    QPointer<QPushButton> _additionalSettingsButton;

    QPointer<QLabel> _searchDepthLabel;
    QPointer<QLineEdit> _searchDepthEdit;

    QPointer<QLabel> _minFileSizeLabel;
    QPointer<QLineEdit> _minFileSizeEdit;
    QPointer<QComboBox> _minFileSizeDimensionComboBox;

    QPointer<QPushButton> _searchButton;

    QStringList _includeDirectories;
    QStringList _excludeDirectories;
    QStringList _includeMasks;
    QStringList _excludeMasks;

    bool _searchByHash;
    bool _searchByMeta;
    QString _hashType;
    QString _searchDepth;
    QString _minFileSize;
    QString _minFileSizeDimension;

    const QStringList _possibleHashes { "md5", "sha1", "sha512" };
    const QStringList _possibleFileSizeDimensions { "b", "Kb", "Mb", "Gb" };
    const QString _windowName = "duplicate_finder";
    const int _windowWidth = 640;
    const int _windowHeight = 750;

private slots:
    /**
     * @brief Добавить директорию для поиска
     */
    void addSearchDirectories();

    /**
     * @brief Удалить директорию для поиска
     */
    void deleteSearchDirectory();

    /**
     * @brief Добавить директорию, исключающуюся из поиска
     */
    void addExcludeDirectories();
    /**
     * @brief Удалить директорию, исключающуюся из поиска
     */
    void deleteExcludeDirectory();

    /**
     * @brief Добавить поисковую маску
     */
    void addIncludeMask();

    /**
     * @brief Удалить поисковую маску
     */
    void deleteIncludeMask();

    /**
     * Добавить маску, исключающуюся из поиску
     */
    void addExcludeMask();

    /**
     * @brief Удалить маску, исключающуюся из поиска
     */
    void deleteExcludeMask();

    /**
     * Переключить опцию поиска по хешу
     */
    void toggleHashBox();

    /**
     * @brief Переключить опцию поиска по метаданным
     */
    void toggleMetaBox();

    /**
     * @brief Поменять тип используемой хеш-функции
     */
    void changeHash();

    /**
     * @brief Показать/спрятать дополнительные настройки
     */
    void toggleAdditionalSettings();

    /**
     * @brief Поменять единицы измерения минимального размера файла
     */
    void changeFileSizeDimension();

    /**
     * @brief Открыть ссылку с описанием синтаксиса регулярных выражений
     */
    static void openMaskHelp();

    /**
     * Запустить поиск дубликатов с заданными параметрами
     */
    void runSearch();
};
