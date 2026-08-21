#include "../include/path_indexer/PathParser.h"
#include <sstream>

PathParser PathParser::byDelimiter(char delimiter, std::vector<std::string> field_names)
{
    PathParser p;
    p.mode_ = Mode::Delimiter;
    p.delimiter_ = delimiter;
    p.field_names_ = std::move(field_names);
    return p;
}

PathParser PathParser::byRegex(const std::string& pattern, std::vector<std::string> field_names)
{
    PathParser p;
    p.mode_ = Mode::Regex;
    p.regex_ = std::regex(pattern);
    p.field_names_ = std::move(field_names);
    return p;
}

PathParser::Result PathParser::parse(const std::string& path) const
{
    return mode_ == Mode::Delimiter ? parseByDelimiter(path) : parseByRegex(path);
}

PathParser::Result PathParser::parseByDelimiter(const std::string& path) const
{
    Result result;
    std::vector<std::string> tokens;
    std::stringstream ss(path);
    std::string token;
    while (std::getline(ss, token, delimiter_))
    {
        tokens.push_back(token);
    }

    if (tokens.size() != field_names_.size())
    {
        return result;
    }

    for (size_t i = 0; i < field_names_.size(); ++i)
    {
        result.fields[field_names_[i]] = tokens[i];
    }
    result.success = true;
    return result;
}

PathParser::Result PathParser::parseByRegex(const std::string& path) const
{
    Result result;
    std::smatch match;
    if (!std::regex_search(path, match, regex_) || match.size() - 1 != field_names_.size())
    {
        return result;
    }

    for (size_t i = 0; i < field_names_.size(); ++i)
    {
        result.fields[field_names_[i]] = match[i + 1].str();
    }
    result.success = true;
    return result;
}
