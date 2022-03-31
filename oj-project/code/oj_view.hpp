//专门用来填充模板文件的
#pragma once
#include <iostream>
#include "ctemplate/template.h"
#include <vector>
#include "oj_model.hpp"


class OjView
{
    public:
    static void DrawAllQuestions(std::vector<Question>& questions, std::string* html)
    {
        //1.创建template 字典的对象
        ctemplate::TemplateDictionary dict("all_questions");
        //2.遍历vector，遍历vector就相当于遍历每一个试题，每一个试题构造一个字典
        //一个大字典里有许多小字典
        for(const auto& ques : questions)
        {
            //TemplateDictionary* AddSectionDictionary(const TemplateString section_name);
            ctemplate::TemplateDictionary* sub_dict = dict.AddSectionDictionary("question");
            //为了往谷歌下的一个模板里填充题目的信息
            //void SetValue(const TemplateString variable, const TemplateString value);
            /*
             *variable:指定的是在预定义html当中的变量名称
             * value:替换的值
             */
            sub_dict->SetValue("id", ques.id_);
            sub_dict->SetValue("id", ques.id_);
            sub_dict->SetValue("title", ques.title_);
            sub_dict->SetValue("star", ques.star_);

        }
        //3.填充
        ctemplate::Template* tl = ctemplate::Template::GetTemplate("./template/all_questions.html", ctemplate::DO_NOT_STRIP);
        //渲染方式
        
        tl->Expand(html, &dict);

    }

    static void DrawOneQuestion(const Question& ques, std::string* html)
    {
        ctemplate::TemplateDictionary dict("question");
        dict.SetValue("id", ques.id_);
        dict.SetValue("title", ques.title_);
        dict.SetValue("star", ques.star_);
        dict.SetValue("desc", ques.desc_);
        dict.SetValue("id", ques.id_);
        dict.SetValue("code", ques.header_cpp_);
        ctemplate::Template* tl = ctemplate::Template::GetTemplate("./template/question.html", ctemplate::DO_NOT_STRIP);
             //渲染    
        tl->Expand(html, &dict);

    }
    static void DrawCaseResult(const std::string& err_no, const std::string& q_result, const std::string& reason, std::string* html)
    {
        ctemplate::TemplateDictionary dict("question");
        //填充
        dict.SetValue("errno", err_no);
        dict.SetValue("compile_result", reason);
        dict.SetValue("case_result", q_result);
        //渲染
        ctemplate::Template* tl = ctemplate::Template::GetTemplate("./template/case_result.html", ctemplate::DO_NOT_STRIP);
        tl->Expand(html,&dict);
    }
};
