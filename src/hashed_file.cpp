#include "hashed_file.h"

#include <QFile>

bool HashNode::operator==(const HashNode& other) const
{
    return data == other.data;
}

bool HashNode::operator!=(const HashNode& other) const
{
    return data != other.data;
}

HashedFile::HashedFile(boost::filesystem::path path, uintmax_t hash_blocksize, std::shared_ptr<IHash> hasher) :
    filepath_(path),
    filesize_(boost::filesystem::file_size(filepath_)),
    max_blocks_amount_((filesize_ + hash_blocksize - 1) / hash_blocksize),
    blocksize_(hash_blocksize),
    hasher_(hasher)
{
}

HashedFile::HashedFile(const HashedFile& file) :
    filepath_(file.GetFilePath()),
    filesize_(file.filesize_),
    max_blocks_amount_(file.max_blocks_amount_),
    blocksize_(file.blocksize_),
    hasher_(file.hasher_)
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

//void HashedFile::OpenHandle()
//{
//    if (file_handle_ == nullptr)
//    {
//        file_handle_ = std::make_unique<std::ifstream>(filepath_.string());
//        file_handle_->seekg(hash_data_.size() * blocksize_);
//    }
//}
//
//void HashedFile::CloseHandle()
//{
//    if (file_handle_ != nullptr)
//    {
//        file_handle_->close();
//        file_handle_.reset(nullptr);
//    }
//}

std::unique_ptr<char[]> HashedFile::GetNextBlock()
{
    uintmax_t size = blocksize_;
    //        uintmax_t remain_size = filesize_ - blocksize_ * hash_data_.size();
    //    if (remain_size < size)
    //    {
    //        size = remain_size;
    //    }
    //    OpenHandle();
    //    QFile file(QString::fromStdString(filepath_.string()));
    //    file.seek(blocksize_ * hash_data_.size());
    //    QByteArray data = file.read(blocksize_);
    //    std::vector<unsigned int> f(data.begin(), data.end());
    auto buffer = std::make_unique<char[]>(size);
    //    file_handle_->read(buffer.get(), size);
    //    CloseHandle();
    //    std::cout << std::string(buffer.get()) << std::endl;
    return buffer;
}

void HashedFile::CalcHashUntil(size_t block_index)
{
    while (hash_data_.size() <= block_index)
    {
        std::cout << block_index << std::endl;
        //        char* data = GetNextBlock().get();
        QFile file(QString::fromStdString(filepath_.string()));
        file.open(QIODevice::ReadOnly);
        file.seek(blocksize_ * hash_data_.size());
        QByteArray data = file.read(blocksize_);
        file.close();
        std::cout << GetFilePath() << " : " << std::string(data.data()).size() << " " << std::string(data.data()) << std::endl;
        //        if (std::string(data).size() == 0)
        //            return;
        //        std::vector<unsigned int> hashdata = hasher_->Hash(data.data());
        auto hdata = QCryptographicHash::hash(data, QCryptographicHash::Md4);
        std::vector<unsigned int> hashdata(hdata.begin(), hdata.end());
        HashNode node { hashdata };
        hash_data_.emplace_back(node);
    }
}

HashNode HashedFile::GetHashNode(size_t block_index)
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