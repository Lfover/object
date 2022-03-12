#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp> 
#include <vector>


class FileUtil
{
    public:
    //读文件接口
    //file_name: 文件名称
    //content: 读到的内容，输出参数, 返还给调用者
    static bool ReadFile(const std::string& file_name, std::string* content)
    {
        //1.清空content当中的内容
        content->clear();
        std::ifstream file(file_name.c_str());
        //如果打开失败了
        if(!file.is_open())
        {
            return false;
        }
        //文件被打开了
        std::string line;
        while(std::getline(file, line))
        {
            (*content) += line + "\n";
        }
        file.close();
        return true;
    }
class StringUtil
{
    public:
    //提供一个切割函数
    static void Split(const std:: string& input, const std::string& split_char, std::vector<std::string>* output)
    {
        boost::split(*output, input, boost::is_any_of(split_char), boost::token_compress_off);

    }
