#include "gui/resultwindow.h"

ResultWindow::ResultWindow(std::unordered_map<std::string, std::unordered_set<std::string>> duplicates)
{
    _duplicates = duplicates;
}