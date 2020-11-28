#include <QDir>
#include <QPointer>
#include <duplicate_finder.h>
#include <gtest/gtest.h>

TEST(duplicatesHash, ok)
{
    QDir workDir = QDir::current();
    QString testDir = workDir.absolutePath() + QDir::separator() + "FileTestDir";
    QDir dir(testDir);
    dir.removeRecursively();
    workDir.mkdir(testDir);
    QString path1 = testDir + QDir::separator() + "file1.txt";
    QString path2 = testDir + QDir::separator() + "file2.txt";
    QFile file1(path1);
    QFile file2(path2);
    const QString content = "File content";
    file1.open(QIODevice::WriteOnly | QIODevice::Text);
    file1.write(content.toUtf8());
    file1.close();
    file2.open(QIODevice::WriteOnly | QIODevice::Text);
    file2.write(content.toUtf8());
    file2.close();
    QString excludeDir = workDir.absolutePath() + QDir::separator() + "FileTestDir" + QDir::separator() + "exclude";
    workDir.mkdir(excludeDir);
    QString path3 = excludeDir + QDir::separator() + "file3.txt";
    QFile file3(path3);
    file3.open(QIODevice::WriteOnly | QIODevice::Text);
    file3.write(content.toUtf8());
    file3.close();
    QStringList includeDirs = { testDir };
    QStringList excludeDirs = { excludeDir };
    int depth = 2;
    QCryptographicHash::Algorithm algorithm = QCryptographicHash::Algorithm::Md5;
    bool searchByHash = true;
    bool searchByMeta = false;
    unsigned int minsize = 0;
    DuplicateFinder finder(searchByHash, searchByMeta, algorithm, depth, minsize, includeDirs, excludeDirs, {}, {});
    auto results = finder.Find();
    ASSERT_EQ(results.size(), 1);
    for (const auto& duplicates : results)
    {
        ASSERT_EQ(duplicates.second.size(), 1);
    }
}

TEST(duplicatesMeta, ok)
{
    QDir workDir = QDir::current();
    QString testDir1 = workDir.absolutePath() + QDir::separator() + "FileTestDir1";
    QString testDir2 = workDir.absolutePath() + QDir::separator() + "FileTestDir2";
    QDir dir1(testDir1);
    QDir dir2(testDir2);
    dir1.removeRecursively();
    dir2.removeRecursively();
    workDir.mkdir(testDir1);
    workDir.mkdir(testDir2);
    QString path1 = testDir1 + QDir::separator() + "file1.txt";
    QString path2 = testDir2 + QDir::separator() + "file1.txt";
    QFile file1(path1);
    QFile file2(path2);
    QString content1 = "File content";
    QString content2 = "Fole content";
    file1.open(QIODevice::WriteOnly | QIODevice::Text);
    file1.write(content1.toUtf8());
    file1.close();
    file2.open(QIODevice::WriteOnly | QIODevice::Text);
    file2.write(content2.toUtf8());
    file2.close();
    QStringList includeDirs = { testDir1, testDir2 };
    QStringList excludeDirs = {};
    int depth = 2;
    QCryptographicHash::Algorithm algorithm = QCryptographicHash::Algorithm::Md5;
    bool searchByHash = false;
    bool searchByMeta = true;
    unsigned int minsize = 0;
    DuplicateFinder finder(searchByHash, searchByMeta, algorithm, depth, minsize, includeDirs, excludeDirs, {}, {});
    auto results = finder.Find();
    ASSERT_EQ(results.size(), 1);
    for (const auto& duplicates : results)
    {
        ASSERT_EQ(duplicates.second.size(), 1);
    }
}