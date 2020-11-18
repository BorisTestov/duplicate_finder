#include "gui/searchwindow.h"

#include "duplicate_finder.h"

#include <QDesktopServices>
#include <QShortcut>
#include <QTreeView>

SearchWindow::SearchWindow(QWidget* parent)
{
    _searchByMeta = false;
    _searchByHash = false;
    _hashType = _possibleHashes.at(0);
    _searchDepth = "0";
    _minFileSize = "0";
    _minFileSizeDimension = _possibleFileSizeDimensions.at(0);

    setParent(parent);
    setFixedSize(_minW, _minH);
    setWindowTitle(_windowName);

    _searchButton = new QPushButton("Search", this);
    _searchButton->setGeometry(0, _minH - 50, _minW, 50);
    connect(_searchButton, SIGNAL(released()), this, SLOT(runSearch()));

    _selectedDirectoriesView = new QListWidget(this);
    _selectedDirectoriesView->setGeometry(20, 20, 480, 100);
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete),
                                        _selectedDirectoriesView,
                                        nullptr,
                                        nullptr,
                                        Qt::WidgetShortcut);
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

    _excludedDirectoriesView = new QListWidget(this);
    _excludedDirectoriesView->setGeometry(20, 160, 480, 100);
    QShortcut* excludeShortcut = new QShortcut(QKeySequence(Qt::Key_Delete),
                                               _excludedDirectoriesView,
                                               nullptr,
                                               nullptr,
                                               Qt::WidgetShortcut);
    connect(excludeShortcut, SIGNAL(activated()), this, SLOT(deleteExcludeDirectory()));
    _excludedDirectoriesView->addItems(_excludeDirectories);

    _excludedDirectoriesLabel = new QLabel("Directories to exclude", this);
    _excludedDirectoriesLabel->setGeometry(200, 140, _minW, 20);

    _excludeDirectoriesButton = new QPushButton("Exclude directories", this);
    _excludeDirectoriesButton->setGeometry(510, 180, 120, 40);
    connect(_excludeDirectoriesButton, SIGNAL(released()), this, SLOT(addExcludeDirectories()));

    auto excludeSeparator = new QFrame(this);
    excludeSeparator->setFrameShape(QFrame::HLine);
    excludeSeparator->setGeometry(0, 280, _minW, 5);

    _includeMasksLabel = new QLabel("Include masks", this);
    _includeMasksLabel->setGeometry(200, 280, _minW, 20);

    _includeMasksView = new QListWidget(this);
    _includeMasksView->setGeometry(20, 300, 440, 100);
    QShortcut* includeMaskShortcut = new QShortcut(QKeySequence(Qt::Key_Delete),
                                                   _includeMasksView,
                                                   nullptr,
                                                   nullptr,
                                                   Qt::WidgetShortcut);
    connect(includeMaskShortcut, SIGNAL(activated()), this, SLOT(deleteIncludeMask()));
    _includeMasksView->addItems(_includeMasks);

    _addIncludeMaskButton = new QPushButton("Add mask", this);
    _addIncludeMaskButton->setGeometry(510, 300, 80, 40);
    connect(_addIncludeMaskButton, SIGNAL(released()), this, SLOT(addIncludeMask()));

    _includeMaskEdit = new QLineEdit(this);
    _includeMaskEdit->setGeometry(470, 350, 160, 30);

    _howToCreateIncludeMaskLabel = new QLabel("<a href=\"help\">How to create a mask</a>", this);
    _howToCreateIncludeMaskLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    _howToCreateIncludeMaskLabel->setGeometry(485, 380, 160, 40);
    _howToCreateIncludeMaskLabel->openExternalLinks();
    connect(_howToCreateIncludeMaskLabel, SIGNAL(linkActivated(QString)), this, SLOT(openMaskHelp()));

    auto includeMaskSeparator = new QFrame(this);
    includeMaskSeparator->setFrameShape(QFrame::HLine);
    includeMaskSeparator->setGeometry(0, 420, _minW, 5);

    _excludeMasksLabel = new QLabel("Exclude masks", this);
    _excludeMasksLabel->setGeometry(200, 420, _minW, 20);

    _excludeMasksView = new QListWidget(this);
    _excludeMasksView->setGeometry(20, 440, 440, 100);
    QShortcut* excludeMaskShortcut = new QShortcut(QKeySequence(Qt::Key_Delete),
                                                   _excludeMasksView,
                                                   nullptr,
                                                   nullptr,
                                                   Qt::WidgetShortcut);
    connect(excludeMaskShortcut, SIGNAL(activated()), this, SLOT(deleteExcludeMask()));
    _excludeMasksView->addItems(_excludeMasks);

    _addExcludeMaskButton = new QPushButton("Add mask", this);
    _addExcludeMaskButton->setGeometry(510, 440, 80, 40);
    connect(_addExcludeMaskButton, SIGNAL(released()), this, SLOT(addExcludeMask()));

    _excludeMaskEdit = new QLineEdit(this);
    _excludeMaskEdit->setGeometry(470, 490, 160, 30);

    _howToCreateExcludeMaskLabel = new QLabel("<a href=\"help\">How to create a mask</a>", this);
    _howToCreateExcludeMaskLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    _howToCreateExcludeMaskLabel->setGeometry(485, 520, 160, 40);
    _howToCreateExcludeMaskLabel->openExternalLinks();
    connect(_howToCreateExcludeMaskLabel, SIGNAL(linkActivated(QString)), this, SLOT(openMaskHelp()));

    auto excludeMaskSeparator = new QFrame(this);
    excludeMaskSeparator->setFrameShape(QFrame::HLine);
    excludeMaskSeparator->setGeometry(0, 560, _minW, 5);

    _searchByHashBox = new QCheckBox("Use hash: ", this);
    _searchByHashBox->setGeometry(40, 580, 90, 20);
    connect(_searchByHashBox, SIGNAL(clicked(bool)), this, SLOT(toggleHashBox()));

    _searchByMetaBox = new QCheckBox("Search by name and size", this);
    _searchByMetaBox->setGeometry(40, 600, 200, 20);
    connect(_searchByMetaBox, SIGNAL(clicked(bool)), this, SLOT(toggleMetaBox()));

    _possibleHashesBox = new QComboBox(this);
    _possibleHashesBox->addItems(_possibleHashes);
    _possibleHashesBox->setGeometry(130, 580, 80, 20);
    connect(_possibleHashesBox, SIGNAL(activated(QString)), this, SLOT(changeHash()));

    _showAdditionalSettings = new QPushButton("Show additional settings", this);
    _showAdditionalSettings->setGeometry(230, 580, 180, 20);
    connect(_showAdditionalSettings, SIGNAL(released()), this, SLOT(additionalSettings()));

    _searchDepthEdit = new QLineEdit(this);
    _searchDepthEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    _searchDepthEdit->setGeometry(480, 610, 50, 20);
    _searchDepthEdit->setText(_searchDepth);
    _searchDepthEdit->hide();

    _searchDepthLabel = new QLabel("Search depth (0 - unlimited)", this);
    _searchDepthLabel->setGeometry(230, 610, 180, 20);
    _searchDepthLabel->hide();

    _minFileSizeLabel = new QLabel("Min file size (0 - unlimited, max 1Gb)", this);
    _minFileSizeLabel->setGeometry(230, 650, 230, 20);
    _minFileSizeLabel->hide();

    _minFileSizeEdit = new QLineEdit(this);
    _minFileSizeEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    _minFileSizeEdit->setGeometry(480, 650, 50, 20);
    _minFileSizeEdit->setText(_minFileSize);
    _minFileSizeEdit->hide();

    _minFileSizeDimensionBox = new QComboBox(this);
    _minFileSizeDimensionBox->addItems(_possibleFileSizeDimensions);
    _minFileSizeDimensionBox->setGeometry(530, 650, 50, 20);
    connect(_minFileSizeDimensionBox, SIGNAL(activated(QString)), this, SLOT(changeFileSizeDimension()));
    _minFileSizeDimensionBox->hide();
}

QStringList SearchWindow::chooseDirectories()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    QListView* l = dialog.findChild<QListView*>("listView");
    if (l)
    {
        l->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    QTreeView* t = dialog.findChild<QTreeView*>();
    if (t)
    {
        t->setSelectionMode(QAbstractItemView::ExtendedSelection);
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

void SearchWindow::addExcludeDirectories()
{
    QStringList directories = chooseDirectories();
    for (const auto& directory : directories)
    {
        if (not _excludeDirectories.contains(directory))
        {
            _excludeDirectories.append(directory);
            _excludedDirectoriesView->addItem(directory);
        }
    }
}

void SearchWindow::deleteSearchDirectory()
{
    if (_includeDirectories.size() == 0)
    {
        return;
    }
    auto item = _selectedDirectoriesView->currentItem();
    _includeDirectories.removeOne(item->text());
    delete _selectedDirectoriesView->currentItem();
}

void SearchWindow::deleteExcludeDirectory()
{
    if (_excludeDirectories.size() == 0)
    {
        return;
    }
    auto item = _excludedDirectoriesView->currentItem();
    _excludeDirectories.removeOne(item->text());
    delete _excludedDirectoriesView->currentItem();
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
    if (_includeDirectories.size() == 0)
    {
        return;
    }
    if (_searchByMeta or _searchByHash)
    {
        _searchDepth = _searchDepthEdit->text();
        _minFileSize = _minFileSizeEdit->text();
        _minFileSizeDimension = _minFileSizeDimensionBox->currentText();
        const int BYTES_IN_KB = 1024;
        const int KB_IN_MB = 1024;
        const int MB_IN_GB = 1024;
        size_t depth = _searchDepth.toInt();
        unsigned long long int size = _minFileSize.toULongLong();
        if (_minFileSizeDimension == "b")
        {
            if (size > 1 * MB_IN_GB * KB_IN_MB * BYTES_IN_KB)
            {
                size = 1 * MB_IN_GB * KB_IN_MB * BYTES_IN_KB;
            }
        }
        else if (_minFileSizeDimension == "Kb")
        {
            if (size > 1 * MB_IN_GB * KB_IN_MB)
            {
                size = 1 * MB_IN_GB * KB_IN_MB;
            }
            size *= BYTES_IN_KB;
        }
        else if (_minFileSizeDimension == "Mb")
        {
            if (size > 1 * MB_IN_GB)
            {
                size = 1 * MB_IN_GB;
            }
            size *= BYTES_IN_KB * KB_IN_MB;
        }
        else if (_minFileSizeDimension == "Gb")
        {
            if (size > 1)
            {
                size = 1;
            }
            size *= BYTES_IN_KB * KB_IN_MB * MB_IN_GB;
        }

        DuplicateFinder finder(_searchByHash,
                               _searchByMeta,
                               _hashType.toStdString(),
                               depth,
                               size,
                               _includeDirectories,
                               _excludeDirectories,
                               _includeMasks,
                               _excludeMasks);
    }
}

void SearchWindow::changeHash()
{
    _hashType = _possibleHashesBox->currentText();
}

void SearchWindow::additionalSettings()
{
    if (_searchDepthEdit->isHidden())
    {
        _searchDepthEdit->show();
        _searchDepthLabel->show();
        _minFileSizeLabel->show();
        _minFileSizeEdit->show();
        _minFileSizeDimensionBox->show();
        _showAdditionalSettings->setText("Hide additional settings");
    }
    else
    {
        _searchDepthEdit->hide();
        _searchDepthLabel->hide();
        _minFileSizeLabel->hide();
        _minFileSizeEdit->hide();
        _minFileSizeDimensionBox->hide();
        _showAdditionalSettings->setText("Show additional settings");
    }
}

void SearchWindow::changeFileSizeDimension()
{
    _minFileSizeDimension = _minFileSizeDimensionBox->currentText();
}

void SearchWindow::deleteIncludeMask()
{
    if (_includeMasks.size() == 0)
    {
        return;
    }
    auto item = _includeMasksView->currentItem();
    _includeMasks.removeOne(item->text());
    delete _includeMasksView->currentItem();
}

void SearchWindow::deleteExcludeMask()
{
    if (_excludeMasks.size() == 0)
    {
        return;
    }
    auto item = _excludeMasksView->currentItem();
    _excludeMasks.removeOne(item->text());
    delete _excludeMasksView->currentItem();
}

void SearchWindow::addIncludeMask()
{
    auto mask = _includeMaskEdit->text();
    if (mask == "")
    {
        return;
    }
    if (not _includeMasks.contains(mask))
    {
        _includeMasks.append(mask);
        _includeMasksView->addItem(mask);
    }
    _includeMaskEdit->setText("");
}

void SearchWindow::addExcludeMask()
{
    auto mask = _excludeMaskEdit->text();
    if (mask == "")
    {
        return;
    }
    if (not _excludeMasks.contains(mask))
    {
        _excludeMasks.append(mask);
        _excludeMasksView->addItem(mask);
    }
    _excludeMaskEdit->setText("");
}

void SearchWindow::openMaskHelp()
{
    QDesktopServices::openUrl(QUrl("https://www.boost.org/doc/libs/1_31_0/libs/regex/doc/syntax.html"));
}