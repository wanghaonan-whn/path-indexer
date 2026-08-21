//
// Created by qzq on 2026/8/21.
//

#ifndef LEARN_PATHPARSER_H
#define LEARN_PATHPARSER_H

#pragma once
#include <string>
#include <vector>
#include <map>
#include <regex>

// 用法：
//   auto p1 = PathParser::byDelimiter('_', {"channel", "train_num", "index"});
//   auto r1 = p1.parse("C1_1234_0001.jpg");
//
//   auto p2 = PathParser::byRegex(R"((\w+)_(\d+)_(\d+))", {"channel", "train_num", "index"});
//   auto r2 = p2.parse("C1_1234_0001.jpg");
//
//   if (r1.success) { r1.fields["channel"]; }
class PathParser
{
public:
    struct Result
    {
        bool success = false;
        std::map<std::string, std::string> fields;
    };

    static PathParser byDelimiter(char delimiter, std::vector<std::string> field_names);
    static PathParser byRegex(const std::string& pattern, std::vector<std::string> field_names);

    Result parse(const std::string& path) const;

private:
    enum class Mode { Delimiter, Regex };

    Mode mode_ = Mode::Delimiter;
    char delimiter_ = '_';
    std::regex regex_;
    std::vector<std::string> field_names_;

    PathParser() = default;

    Result parseByDelimiter(const std::string& path) const;
    Result parseByRegex(const std::string& path) const;
};

#endif //LEARN_PATHPARSER_H
