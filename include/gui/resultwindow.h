#pragma once

#include <QWidget>
#include <unordered_map>
#include <unordered_set>

class ResultWindow : public QWidget
{
    Q_OBJECT
public:
    ResultWindow(std::unordered_map<std::string, std::unordered_set<std::string>> duplicates);

private:
    std::unordered_map<std::string, std::unordered_set<std::string>> _duplicates;
};