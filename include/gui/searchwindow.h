#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QWidget>

class SearchWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWindow(QWidget* parent = nullptr);

private:
    QStringList chooseDirectories();

    QPushButton* _searchButton;
    QPushButton* _addDirectoriesButton;
    QListWidget* _selectedDirectoriesView;
    QLabel* _selectedDirectoriesLabel;
    QCheckBox* _searchByHashBox;
    QCheckBox* _searchByMetaBox;
    QComboBox* _possibleHashesBox;

    QStringList _includeDirectories;
    bool _searchByHash = false;
    bool _searchByMeta = false;
    QString _hashType = "crc32";

    const QStringList _possibleHashes { "crc32", "md5", "sha1" };
    const QString _windowName = "duplicate_finder";
    const int _minW = 640;
    const int _minH = 400;

private slots:
    void addSearchDirectories();
    void deleteSearchDirectory();
    void toggleHashBox();
    void toggleMetaBox();
    void changeHash();
    void runSearch();
};