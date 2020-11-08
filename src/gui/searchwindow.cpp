#include "gui/searchwindow.h"

#include "duplicate_finder.h"

#include <QShortcut>
#include <QTreeView>

SearchWindow::SearchWindow(QWidget* parent)
{
    setParent(parent);
    setFixedSize(_minW, _minH);
    setWindowTitle(_windowName);
    _searchButton = new QPushButton("Search", this);
    _searchButton->setGeometry((_minW - 80) / 2, _minH - 50, 80, 30);
    connect(_searchButton, SIGNAL(released()), this, SLOT(runSearch()));

    _selectedDirectoriesView = new QListWidget(this);
    _selectedDirectoriesView->setGeometry(20, 20, 480, 100);
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), _selectedDirectoriesView);
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteSearchDirectory()));
    _selectedDirectoriesView->addItems(_includeDirectories);

    _selectedDirectoriesLabel = new QLabel("Directories for scanning", this);
    _selectedDirectoriesLabel->setGeometry(200, 0, _minW, 20);

    _addDirectoriesButton = new QPushButton("Add directories", this);
    _addDirectoriesButton->setGeometry(520, 40, 100, 40);
    connect(_addDirectoriesButton, SIGNAL(released()), this, SLOT(addSearchDirectories()));

    auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setGeometry(0, 140, _minW, 5);

    _searchByHashBox = new QCheckBox("Use hash: ", this);
    _searchByHashBox->setGeometry(40, 160, 90, 20);
    connect(_searchByHashBox, SIGNAL(clicked(bool)), this, SLOT(toggleHashBox()));

    _searchByMetaBox = new QCheckBox("Search by name and size", this);
    _searchByMetaBox->setGeometry(40, 180, 200, 20);
    connect(_searchByMetaBox, SIGNAL(clicked(bool)), this, SLOT(toggleMetaBox()));

    _possibleHashesBox = new QComboBox(this);
    _possibleHashesBox->addItems(_possibleHashes);
    _possibleHashesBox->setGeometry(130, 160, 80, 20);
    connect(_possibleHashesBox, SIGNAL(activated(QString)), this, SLOT(changeHash()));
}

QStringList SearchWindow::chooseDirectories()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    // For multiselection
    // Reference: https://www.qtcentre.org/threads/34226-QFileDialog-select-multiple-directories
    QListView* l = dialog.findChild<QListView*>("listView");
    if (l)
    {
        l->setSelectionMode(QAbstractItemView::MultiSelection);
    }
    QTreeView* t = dialog.findChild<QTreeView*>();
    if (t)
    {
        t->setSelectionMode(QAbstractItemView::MultiSelection);
    }
    QStringList directories;
    if (dialog.exec())
    {
        directories = dialog.selectedFiles();
    }
    return directories;
}

void SearchWindow::addSearchDirectories()
{
    QStringList directories = chooseDirectories();
    for (const auto& directory : directories)
    {
        if (not _includeDirectories.contains(directory))
        {
            _includeDirectories.append(directory);
            _selectedDirectoriesView->addItem(directory);
        }
    }
}

void SearchWindow::deleteSearchDirectory()
{
    auto item = _selectedDirectoriesView->currentItem();
    _includeDirectories.removeOne(item->text());
    delete _selectedDirectoriesView->currentItem();
}

void SearchWindow::toggleHashBox()
{
    _searchByHash = _searchByHashBox->isChecked();
}

void SearchWindow::toggleMetaBox()
{
    _searchByMeta = _searchByMetaBox->isChecked();
}

void SearchWindow::runSearch()
{
    if (_searchByMeta or _searchByHash)
    {
        DuplicateFinder finder(_searchByHash, _searchByMeta, _hashType.toStdString());
    }
}

void SearchWindow::changeHash()
{
    _hashType = _possibleHashesBox->currentText();
}
