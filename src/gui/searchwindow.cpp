#include "gui/searchwindow.h"

#include "duplicate_finder.h"

#include <QCryptographicHash>
#include <QDesktopServices>
#include <QMessageBox>
#include <QShortcut>
#include <QTreeView>

SearchWindow::SearchWindow()
{
    _searchByMeta = false;
    _searchByHash = false;
    _hashType = _possibleHashes.at(0);
    _searchDepth = "0";
    _minFileSize = "0";
    _minFileSizeDimension = _possibleFileSizeDimensions.at(0);

    setFixedSize(_windowWidth, _windowHeight);
    setWindowTitle(_windowName);

    createIncludeDirectoriesView();
    createIncludeDirectoriesLabel();
    createIncludeDirectoriesButton();

    createExcludeDirectoriesView();
    createExcludeDirectoriesLabel();
    createExcludeDirectoriesButton();

    createIncludeMasksView();
    createIncludeMasksLabel();
    createIncludeMasksEdit();
    createIncludeMasksButton();
    createIncludeMasksLink();

    createExcludeMasksView();
    createExcludeMasksLabel();
    createExcludeMasksEdit();
    createExcludeMasksButton();
    createExcludeMasksLink();

    createHashSearchBox();
    createPossibleHashesComboBox();

    createMetaSearchBox();

    createAdditionalSettingsButton();

    createSearchDepthLabel();
    createSearchDepthEdit();

    createMinFileSizeLabel();
    createMinFileSizeEdit();
    createMinFileSizeDimensionComboBox();

    createSearchButton();

    createSeparators();
}

QStringList SearchWindow::chooseDirectories()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    auto list = dialog.findChild<QListView*>("listView");
    if (list)
    {
        list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    auto tree = dialog.findChild<QTreeView*>();
    if (tree)
    {
        tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    QStringList directories;
    if (dialog.exec())
    {
        directories = dialog.selectedFiles();
    }
    return directories;
}

void SearchWindow::createIncludeDirectoriesView()
{
    _includeDirectoriesView = new QListWidget(this);
    _includeDirectoriesView->setGeometry(20, 20, 480, 100);
    QPointer<QShortcut> shortcut = new QShortcut(QKeySequence(Qt::Key_Delete),
                                                 _includeDirectoriesView.data(),
                                                 nullptr,
                                                 nullptr,
                                                 Qt::WidgetShortcut);
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteSearchDirectory()));
    _includeDirectoriesView->addItems(_includeDirectories);
}

void SearchWindow::createIncludeDirectoriesLabel()
{
    _includeDirectoriesLabel = new QLabel("Directories for scanning", this);
    _includeDirectoriesLabel->setGeometry(200, 0, _windowWidth, 20);
}

void SearchWindow::createIncludeDirectoriesButton()
{
    _includeDirectoriesButton = new QPushButton("Add directories", this);
    _includeDirectoriesButton->setGeometry(520, 40, 100, 40);
    connect(_includeDirectoriesButton.data(), SIGNAL(released()), this, SLOT(addSearchDirectories()));
}

void SearchWindow::createExcludeDirectoriesView()
{
    _excludedDirectoriesView = new QListWidget(this);
    _excludedDirectoriesView->setGeometry(20, 160, 480, 100);
    QPointer<QShortcut> shortcut = new QShortcut(QKeySequence(Qt::Key_Delete),
                                                 _excludedDirectoriesView.data(),
                                                 nullptr,
                                                 nullptr,
                                                 Qt::WidgetShortcut);
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteExcludeDirectory()));
    _excludedDirectoriesView->addItems(_excludeDirectories);
}

void SearchWindow::createExcludeDirectoriesLabel()
{
    _excludedDirectoriesLabel = new QLabel("Directories to exclude", this);
    _excludedDirectoriesLabel->setGeometry(200, 140, _windowWidth, 20);
}

void SearchWindow::createExcludeDirectoriesButton()
{
    _excludeDirectoriesButton = new QPushButton("Exclude directories", this);
    _excludeDirectoriesButton->setGeometry(510, 180, 120, 40);
    connect(_excludeDirectoriesButton.data(), SIGNAL(released()), this, SLOT(addExcludeDirectories()));
}

void SearchWindow::createIncludeMasksView()
{
    _includeMasksView = new QListWidget(this);
    _includeMasksView->setParent(this);
    _includeMasksView->setGeometry(20, 300, 440, 100);
    QPointer<QShortcut> shortcut = new QShortcut(QKeySequence(Qt::Key_Delete),
                                                 _includeMasksView.data(),
                                                 nullptr,
                                                 nullptr,
                                                 Qt::WidgetShortcut);
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteIncludeMask()));
    _includeMasksView->addItems(_includeMasks);
}

void SearchWindow::createIncludeMasksLabel()
{
    _includeMasksLabel = new QLabel("Include masks", this);
    _includeMasksLabel->setGeometry(200, 280, _windowWidth, 20);
}

void SearchWindow::createIncludeMasksEdit()
{
    _includeMasksEdit = new QLineEdit(this);
    _includeMasksEdit->setGeometry(470, 350, 160, 30);
}

void SearchWindow::createIncludeMasksButton()
{
    _includeMasksButton = new QPushButton("Add mask", this);
    _includeMasksButton->setGeometry(510, 300, 80, 40);
    connect(_includeMasksButton.data(), SIGNAL(released()), this, SLOT(addIncludeMask()));
}

void SearchWindow::createIncludeMasksLink()
{
    _includeMasksLink = new QLabel("<a href=\"help\">How to create a mask</a>", this);
    _includeMasksLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    _includeMasksLink->setGeometry(485, 380, 160, 40);
    _includeMasksLink->openExternalLinks();
    connect(_includeMasksLink.data(), SIGNAL(linkActivated(QString)), this, SLOT(openMaskHelp()));
}

void SearchWindow::createExcludeMasksView()
{
    _excludeMasksView = new QListWidget(this);
    _excludeMasksView->setGeometry(20, 440, 440, 100);
    QPointer<QShortcut> shortcut = new QShortcut(QKeySequence(Qt::Key_Delete),
                                                 _excludeMasksView.data(),
                                                 nullptr,
                                                 nullptr,
                                                 Qt::WidgetShortcut);
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteExcludeMask()));
    _excludeMasksView->addItems(_excludeMasks);
}

void SearchWindow::createExcludeMasksLabel()
{
    _excludeMasksLabel = new QLabel("Exclude masks", this);
    _excludeMasksLabel->setGeometry(200, 420, _windowWidth, 20);
}

void SearchWindow::createExcludeMasksEdit()
{
    _excludeMasksEdit = new QLineEdit(this);
    _excludeMasksEdit->setGeometry(470, 490, 160, 30);
}

void SearchWindow::createExcludeMasksButton()
{
    _excludeMasksButton = new QPushButton("Add mask", this);
    _excludeMasksButton->setGeometry(510, 440, 80, 40);
    connect(_excludeMasksButton.data(), SIGNAL(released()), this, SLOT(addExcludeMask()));
}

void SearchWindow::createExcludeMasksLink()
{
    _excludeMasksLink = new QLabel("<a href=\"help\">How to create a mask</a>", this);
    _excludeMasksLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    _excludeMasksLink->setGeometry(485, 520, 160, 40);
    _excludeMasksLink->openExternalLinks();
    connect(_excludeMasksLink.data(), SIGNAL(linkActivated(QString)), this, SLOT(openMaskHelp()));
}

void SearchWindow::createHashSearchBox()
{
    _hashSearchBox = new QCheckBox("Use hash: ", this);
    _hashSearchBox->setGeometry(40, 580, 90, 20);
    connect(_hashSearchBox.data(), SIGNAL(clicked(bool)), this, SLOT(toggleHashBox()));
}

void SearchWindow::createPossibleHashesComboBox()
{
    _possibleHashesComboBox = new QComboBox(this);
    _possibleHashesComboBox->addItems(_possibleHashes);
    _possibleHashesComboBox->setGeometry(130, 580, 80, 20);
    connect(_possibleHashesComboBox.data(), SIGNAL(activated(QString)), this, SLOT(changeHash()));
}

void SearchWindow::createMetaSearchBox()
{
    _metaSearchBox = new QCheckBox("Search by name and size", this);
    _metaSearchBox->setGeometry(40, 600, 200, 20);
    connect(_metaSearchBox.data(), SIGNAL(clicked(bool)), this, SLOT(toggleMetaBox()));
}

void SearchWindow::createAdditionalSettingsButton()
{
    _additionalSettingsButton = new QPushButton("Show additional settings", this);
    _additionalSettingsButton->setGeometry(230, 580, 180, 20);
    connect(_additionalSettingsButton.data(), SIGNAL(released()), this, SLOT(toggleAdditionalSettings()));
}

void SearchWindow::createSearchDepthLabel()
{
    _searchDepthLabel = new QLabel("Search depth", this);
    _searchDepthLabel->setGeometry(230, 610, 180, 20);
    _searchDepthLabel->hide();
}

void SearchWindow::createSearchDepthEdit()
{
    _searchDepthEdit = new QLineEdit(this);
    _searchDepthEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    _searchDepthEdit->setGeometry(480, 610, 50, 20);
    _searchDepthEdit->setText(_searchDepth);
    _searchDepthEdit->hide();
}

void SearchWindow::createMinFileSizeLabel()
{
    _minFileSizeLabel = new QLabel("Min file size (0 - unlimited, max 1Gb)", this);
    _minFileSizeLabel->setGeometry(230, 650, 230, 20);
    _minFileSizeLabel->hide();
}

void SearchWindow::createMinFileSizeEdit()
{
    _minFileSizeEdit = new QLineEdit(this);
    _minFileSizeEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    _minFileSizeEdit->setGeometry(480, 650, 50, 20);
    _minFileSizeEdit->setText(_minFileSize);
    _minFileSizeEdit->hide();
}

void SearchWindow::createMinFileSizeDimensionComboBox()
{
    _minFileSizeDimensionComboBox = new QComboBox(this);
    _minFileSizeDimensionComboBox->addItems(_possibleFileSizeDimensions);
    _minFileSizeDimensionComboBox->setGeometry(530, 650, 50, 20);
    connect(_minFileSizeDimensionComboBox.data(), SIGNAL(activated(QString)), this, SLOT(changeFileSizeDimension()));
    _minFileSizeDimensionComboBox->hide();
}

void SearchWindow::createSearchButton()
{
    _searchButton = new QPushButton("Search", this);
    _searchButton->setGeometry(0, _windowHeight - 50, _windowWidth, 50);
    connect(_searchButton.data(), SIGNAL(released()), this, SLOT(runSearch()));
}

void SearchWindow::createSeparators()
{
    QPointer<QFrame> includeSeparator = new QFrame(this);
    includeSeparator->setFrameShape(QFrame::HLine);
    includeSeparator->setGeometry(0, 140, _windowWidth, 5);

    QPointer<QFrame> excludeSeparator = new QFrame(this);
    excludeSeparator->setFrameShape(QFrame::HLine);
    excludeSeparator->setGeometry(0, 280, _windowWidth, 5);

    QPointer<QFrame> includeMaskSeparator = new QFrame(this);
    includeMaskSeparator->setFrameShape(QFrame::HLine);
    includeMaskSeparator->setGeometry(0, 420, _windowWidth, 5);

    QPointer<QFrame> excludeMaskSeparator = new QFrame(this);
    excludeMaskSeparator->setFrameShape(QFrame::HLine);
    excludeMaskSeparator->setGeometry(0, 560, _windowWidth, 5);
}

void SearchWindow::addSearchDirectories()
{
    QStringList directories = chooseDirectories();
    for (const auto& directory : directories)
    {
        if (not _includeDirectories.contains(directory))
        {
            _includeDirectories.append(directory);
            _includeDirectoriesView->addItem(directory);
        }
    }
}

void SearchWindow::deleteSearchDirectory()
{
    if (_includeDirectories.empty())
    {
        return;
    }
    auto item = _includeDirectoriesView->currentItem();
    _includeDirectories.removeOne(item->text());
    delete _includeDirectoriesView->currentItem();
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

void SearchWindow::deleteExcludeDirectory()
{
    if (_excludeDirectories.empty())
    {
        return;
    }
    auto item = _excludedDirectoriesView->currentItem();
    _excludeDirectories.removeOne(item->text());
    delete _excludedDirectoriesView->currentItem();
}

void SearchWindow::addIncludeMask()
{
    auto mask = _includeMasksEdit->text();
    if (mask == "")
    {
        return;
    }
    if (not _includeMasks.contains(mask))
    {
        _includeMasks.append(mask);
        _includeMasksView->addItem(mask);
    }
    _includeMasksEdit->setText("");
}

void SearchWindow::deleteIncludeMask()
{
    if (_includeMasks.empty())
    {
        return;
    }
    auto item = _includeMasksView->currentItem();
    _includeMasks.removeOne(item->text());
    delete _includeMasksView->currentItem();
}

void SearchWindow::addExcludeMask()
{
    auto mask = _excludeMasksEdit->text();
    if (mask == "")
    {
        return;
    }
    if (not _excludeMasks.contains(mask))
    {
        _excludeMasks.append(mask);
        _excludeMasksView->addItem(mask);
    }
    _excludeMasksEdit->setText("");
}

void SearchWindow::deleteExcludeMask()
{
    if (_excludeMasks.empty())
    {
        return;
    }
    auto item = _excludeMasksView->currentItem();
    _excludeMasks.removeOne(item->text());
    delete _excludeMasksView->currentItem();
}

void SearchWindow::toggleHashBox()
{
    _searchByHash = _hashSearchBox->isChecked();
}

void SearchWindow::toggleMetaBox()
{
    _searchByMeta = _metaSearchBox->isChecked();
}

void SearchWindow::changeHash()
{
    _hashType = _possibleHashesComboBox->currentText();
}

void SearchWindow::toggleAdditionalSettings()
{
    if (_searchDepthEdit->isHidden())
    {
        _searchDepthEdit->show();
        _searchDepthLabel->show();
        _minFileSizeLabel->show();
        _minFileSizeEdit->show();
        _minFileSizeDimensionComboBox->show();
        _additionalSettingsButton->setText("Hide additional settings");
    }
    else
    {
        _searchDepthEdit->hide();
        _searchDepthLabel->hide();
        _minFileSizeLabel->hide();
        _minFileSizeEdit->hide();
        _minFileSizeDimensionComboBox->hide();
        _additionalSettingsButton->setText("Show additional settings");
    }
}

void SearchWindow::changeFileSizeDimension()
{
    _minFileSizeDimension = _minFileSizeDimensionComboBox->currentText();
}
void SearchWindow::openMaskHelp()
{
    QDesktopServices::openUrl(QUrl("https://www.boost.org/doc/libs/1_31_0/libs/regex/doc/syntax.html"));
}

void SearchWindow::runSearch()
{
    if (_includeDirectories.empty())
    {
        QMessageBox msgBox;
        msgBox.setText(
            "Please choose at least one directory for search");
        msgBox.exec();
        return;
    }
    if (_searchByMeta or _searchByHash)
    {
        _searchDepth = _searchDepthEdit->text();
        _minFileSize = _minFileSizeEdit->text();
        _minFileSizeDimension = _minFileSizeDimensionComboBox->currentText();
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
        QCryptographicHash::Algorithm algorithm = QCryptographicHash::Md5;
        if (_hashType == "md5")
        {
            algorithm = QCryptographicHash::Md5;
        }
        else if (_hashType == "sha1")
        {
            algorithm = QCryptographicHash::Sha1;
        }
        else if (_hashType == "sha512")
        {
            algorithm = QCryptographicHash::Sha512;
        }

        DuplicateFinder finder(_searchByHash,
                               _searchByMeta,
                               algorithm,
                               depth,
                               size,
                               _includeDirectories,
                               _excludeDirectories,
                               _includeMasks,
                               _excludeMasks);
        std::unordered_map<std::string, std::unordered_set<std::string>> duplicates = finder.Find();
        if (duplicates.empty())
        {
            QMessageBox msgBox;
            msgBox.setText("No duplicates were found!");
            msgBox.exec();
            return;
        }
        _resultWindow = QPointer<ResultWindow>(new ResultWindow(duplicates));
        _resultWindow->show();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(
            "Please choose at least one:\n"
            "Use hash or Search by name and size");
        msgBox.exec();
    }
}
