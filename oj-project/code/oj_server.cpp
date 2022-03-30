#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <jsoncpp/json/json.h>
#include "httplib.h"
#include "oj_model.hpp"
#include "tools.hpp"
#include "oj_view.hpp"
#include "compile.hpp"
using namespace httplib;
int main()
{   //包含了这个命名空间后，后面使用就不需要加httplib::
	OjModel model;//创建一个模板对象
    //1.初始化httplib库的server对象
	Server svr;
	
	//2.提供三个接口，分别处理三个请求
	//2.1获取整个试题列表，get
    //"第一个参数"就是资源路径
	svr.Get("/all_questions",[&model](const Request& rep, Response& resp){
		//返回试题列表
        std::vector<Question> questions;
        //输出
        model.GetAllQuestion(&questions);
        //填充，提供所有试题
        std::string html;
        OjView::DrawAllQuestions(questions, &html);//当前这个接口返回html

		resp.set_content(html, "text/html");//21是前面传递的长度	
	});//all_questions是资源路径；lamdar表达式[]是传递的参数，（）是参数，{}是lamdar表达式的实现
	//2.2获取单个试题
	//如何标识浏览器想要获取的是哪一个试题
    //\d:表示一个数字[0, 9]
    //\d+：表示多位数字
	svr.Get(R"(/question/(\d+))", [&model](const Request& req, Response& resp){
        //1.获取url当中关于试题的数字,获取单个试题的信息
        //std::cout << req.matches[0] << " " << req.matches[1] << std::endl;
         //std::cout << req.version << " " << req.method << std::endl;
         //std::cout << req.path << std::endl;
        Question ques;
        model.GetOneQuestion(req.matches[1].str(), &ques);
        //2.   渲染模板的html文件
        std::string html;
        OjView::DrawOneQuestion(ques, &html);
        resp.set_content(html, "text/html");
	});
	//2.3编译运行
	//也需要知道我是想提交哪一个试题进行编译
    //1.获取试题编号
    //2.通过 试题编号 进而获取 题目内容 填充到ques中
    //3.用户点击submit后从浏览器获取代码，将代码进行decode
    //4.将代码与tail.cpp合并，成src文件
    //5.编译模块进行编译
    //  编译src文件的技术：创建子进程，使用子进程程序替换称为g++来编译源码文件
    //  成功
    //  失败
    //  运行编译后的文件技术：创建子进程，使用子进程程序替换称为编译出来的可执行程序
    //  成功
    //  失败
    //
	svr.Post(R"(/compile/(\d+))", [&model](const Request& req, Response& resp){
        //1.获取试题
        //获取当前题目的所有信息
        //1.获取题目id
        std::string ques_id = req.matches[1].str();
        //获取题目内容
        Question ques;
        model.GetOneQuestion(req.matches[1].str(), &ques);
        //ques.tail_cpp_----->main函数调用+测试用例
        //post方法在提交代码的时候，是经过encode的，要想正常获取浏览器提交的内容，需要进行decode
        //decode，使用decode完成的代码和tail.cpp进行合并，产生待编译的源码
        //key:value方式，Key是code,value是内容 
        //vec保存切割的内容
        std::unordered_map<std::string, std::string> body_kv;
        UrlUtil::PraseBody(req.body, &body_kv);
        //2.构造json对象，交给编译运行模块
        std::string user_code = body_kv["code"];
        Json::Value req_json;
        req_json["code"] = user_code + ques.tail_cpp_;
        req_json["stdin"]="";
        std::cout << req_json["code"].asString() << std::endl; 
        Json::Value resp_json;
        Compiler::CompileAndRun(req_json, &resp_json);
        //获取的返回结果都在resp_json当中
        std::string err_no = resp_json["errorno"].asString();
        std::string case_result = resp_json["stdout"].asString();
        std::string reason = resp_json["reason"].asString();
        //填充
        std::string html;
        OjView::DrawCaseReault(err_no, case_result, reason, &html);
        resp.set_content(html, "text/html");
	});
   // LOG(INFO, "listen_port") << ": 17878" << std::endl;
    svr.set_base_dir("./www");
	//服务端监听起来
	svr.listen("0.0.0.0", 17878);
	return 0;
}
