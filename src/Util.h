#pragma once

namespace Util
{
    std::optional<fs::path> GetDocumentsPath();
    std::optional<fs::path> GetDatabasePath();

    std::vector<std::string> StringSplit(const std::string& text, char sep);
}
