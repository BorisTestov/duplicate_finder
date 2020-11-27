#include "hashed_file.h"

#include <QFile>
#include <QFileInfo>

HashedFile::HashedFile(boost::filesystem::path path, QCryptographicHash::Algorithm hash_method) :
    filepath_(path),
    filesize_(boost::filesystem::file_size(filepath_)),
    max_blocks_amount_((filesize_ + blocksize_ - 1) / blocksize_),
    birthTime_(QFileInfo(QString::fromStdString(filepath_.string())).created()),
    hash_method_(hash_method)
{
}

bool HashedFile::Equal(HashedFile& other)
{
    if (filesize_ != other.filesize_)
    {
        return false;
    }
    bool isEqual = true;
    for (size_t i = 0; i < max_blocks_amount_; i++)
    {
        if (GetHashNode(i) != other.GetHashNode(i))
        {
            isEqual = false;
            break;
        }
    }
    return isEqual;
}

boost::filesystem::path HashedFile::GetFilePath() const
{
    return filepath_;
}

void HashedFile::CalcHashUntil(size_t block_index)
{
    while (hash_data_.size() <= block_index)
    {
        QFile file(QString::fromStdString(filepath_.string()));
        file.open(QIODevice::ReadOnly);
        file.seek(blocksize_ * hash_data_.size());
        QByteArray data = file.read(blocksize_);
        file.close();
        QByteArray hash = QCryptographicHash::hash(data, hash_method_);
        hash_data_.emplace_back(hash);
    }
}

QByteArray HashedFile::GetHashNode(size_t block_index)
{
    if (block_index >= max_blocks_amount_)
    {
        throw std::logic_error("Invalid block index value!");
    }
    if (block_index < hash_data_.size())
    {
        return hash_data_[block_index];
    }
    CalcHashUntil(block_index);
    return hash_data_[block_index];
}