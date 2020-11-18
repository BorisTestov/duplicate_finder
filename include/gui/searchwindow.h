#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QRegExpValidator>
#include <QWidget>

class SearchWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWindow(QWidget* parent = nullptr);

private:
    QStringList chooseDirectories();

    QLabel* _selectedDirectoriesLabel;
    QListWidget* _selectedDirectoriesView;
    QPushButton* _addDirectoriesButton;

    QLabel* _excludedDirectoriesLabel;
    QListWidget* _excludedDirectoriesView;
    QPushButton* _excludeDirectoriesButton;

    QLabel* _includeMasksLabel;
    QListWidget* _includeMasksView;
    QLineEdit* _includeMaskEdit;
    QPushButton* _addIncludeMaskButton;
    QLabel* _howToCreateIncludeMaskLabel;

    QLabel* _excludeMasksLabel;
    QListWidget* _excludeMasksView;
    QLineEdit* _excludeMaskEdit;
    QPushButton* _addExcludeMaskButton;
    QLabel* _howToCreateExcludeMaskLabel;

    QCheckBox* _searchByHashBox;
    QComboBox* _possibleHashesBox;
    QCheckBox* _searchByMetaBox;

    QPushButton* _showAdditionalSettings;
    QLabel* _searchDepthLabel;
    QLineEdit* _searchDepthEdit;
    QLabel* _minFileSizeLabel;
    QLineEdit* _minFileSizeEdit;
    QComboBox* _minFileSizeDimensionBox;

    QPushButton* _searchButton;

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

    const QStringList _possibleHashes { "crc32", "md5", "sha1" };
    const QStringList _possibleFileSizeDimensions { "b", "Kb", "Mb", "Gb" };
    const QString _windowName = "duplicate_finder";
    const int _minW = 640;
    const int _minH = 750;

private slots:
    void additionalSettings();
    void addSearchDirectories();
    void deleteSearchDirectory();
    void addExcludeDirectories();
    void deleteExcludeDirectory();
    void addIncludeMask();
    void deleteIncludeMask();
    void addExcludeMask();
    void deleteExcludeMask();
    void toggleHashBox();
    void toggleMetaBox();
    void changeHash();
    void changeFileSizeDimension();
    void openMaskHelp();
    void runSearch();
};