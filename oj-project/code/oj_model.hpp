#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "tools.hpp"
#include <vector>

#include <unordered_map>//无符号的map来保存多个问题
//描述问题的信息
struct Question{
	std::string id_;//题目ID
	std::string title_;//题目标题
	std::string star_;//题目的难易程度
	std::string path_;//题目路径
    std::string desc_;//题目的描述
    std::string header_cpp_;//题目预定义的头
    std::string tail_cpp_;//题目的尾，包含测试用例以及调用逻辑
};

class OjModel
{
    public:
    OjModel()
    {
        Load("./ob_data/oj_config.cfg");
    }
    ~OjModel()
    {

    }

    //从文件当中获取题目信息
    bool Load(std::string& filename)
    {
        //fopen open c++ fstream
        //文件数去流
        std::ifstream file(filename.c_str());
        //如果打开失败了
        if(!file.is_open())
        {
            std::cout << "config file open failed" << std::endl;
        }
        //1.打开文件成功的情况
        //1.1从文件中获取一行信息
        //1.1.1对于每一行信息，还需要获取题号，一幕名曾，题目难易程度，题目路径
        //1.1.2保存在结构体当中
        //2。把多个question，组织在map当中 
        std::string line;
        while(std::getline(file,line))
        {
            //boost::spilt
            //切割一行的信息
            //切割后的信息存在vec_中
            std::vector<std::string> vec;
            StringUtil::Split(line, "/t", &vec);//按/t分割
            //is_any_of:支持多个字符作为分隔符
            // token_compress_off：是否将多个分隔字符看作是一个
            boost::split(vec, line, boost::is_any_of(" "), boost::token_compress_off);
            
            Question ques;
            ques.id_ = vec[0];
            ques.title_ = vec[1];
            ques.star_ = vec[2];
            ques.path_ = vec[3];

            //临时变量存储刚刚读回来文件路径
            std::string dir = vec[3];
            FileUtil::ReadFile(dir + "/desc.txt", &ques.desc_);
            FileUtil::ReadFile(dir + "/header.cpp", &ques.header_cpp_);
            FileUtil::ReadFile(dir + "/tail.cpp", &ques.tail_cpp_);

            //将ques保存
            ques_map_[ques.id_] = ques;
            }
            file.close();
            return true;
   }
    //提供给上层调用着一个获取所有试题的接口
    bool GetAllQuestion()
    {
        //1.遍历无序的map，将试题信息返回给调用者
        //对于每个试题，都是一个question结构体对象
        for(const auto& kv:ques_map_)
        {
            questions->push_back(kv.second);//无序插入
        }
        //2.想实现顺序的排列，需要排序
        std::sort(questions->begin(), questions->end(),[](const Question& l, const Question& r){
            //比较Question当中的题目编号，按照升序排列
            return std::stoi(l.id_) < std::stoi(r.id_);
            });
        return true;
    }
    //提供给上层调用者一个获取单个试题的接口
    //id:输出条件，查找题目的ID
    //ques:输出参数，将查到的结果返回给调用者
    bool GetOneQuestion(const std::string& id, Question* ques)
    {
        auto it = ques_map_.find(id);
        if(it == ques_map_.end())
        {
            return false;
        }
        *ques = it->second;
        return true;
    }

    private:
    std::unordered_map<std::string, Question> ques_map_;//一个题目id，对应一个题目，题目用结构体表示
};

