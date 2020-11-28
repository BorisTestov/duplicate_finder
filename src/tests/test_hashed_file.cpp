#include "hashed_file.h"

#include <QDir>
#include <QPointer>
#include <gtest/gtest.h>
TEST(filesEqual, ok)
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
    QPointer<HashedFile> hashedFile1
        = QPointer<HashedFile>(new HashedFile(boost::filesystem::path(path1.toStdString()),
                                              QCryptographicHash::Algorithm::Md5));
    QPointer<HashedFile> hashedFile2
        = QPointer<HashedFile>(new HashedFile(boost::filesystem::path(path2.toStdString()),
                                              QCryptographicHash::Algorithm::Md5));
    bool isEqual = hashedFile1->Equal(hashedFile2);
    ASSERT_EQ(isEqual, true);
}