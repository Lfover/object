#include <iostream>
#include <cstdio>
#include "httplib.h"
#include "oj_model.hpp"
#include "tools.hpp"
int main()
{
	using namespace httplib;//包含了这个命名空间后，后面使用就不需要加httplib::
	OjModel model;//创建一个模板对象
    
    //1.初始化httplib库的server对象
	Server svr;
	
	//2.提供三个接口，分别处理三个请求
	//2.1获取整个试题列表，get
	svr.Get("/all_questions",[&model](const Request& rep, Response& resp){
		//返回试题列表
        std::vector<Question> questions;
        //输出
        model.GetAllQuestion(&questions);
        for(int i = 0; i < questions.size(); i ++)
        {
            std::cout << questions[i].id_ <<" " << questions[i].title_ <<" "<<  questions[i].star_ << " " << questions[i].path_ << std:: endl;
            std::cout << questions[i].title_ << std::endl;
        }
		resp.set_content("<html>57-linux</html>", 21, "text/html");//21是前面传递的长度	
	});//all_questions是资源路径；lamdar表达式[]是传递的参数，（）是参数，{}是lamdar表达式的实现
	//2.2获取单个试题
	//如何标识浏览器想要获取的是哪一个试题
	svr.Get("/signle_ques", [](const Request& rep, Response& resp){
	    
	
	});
	//2.3编译运行
	//也需要知道我是想提交哪一个试题进行编译
	svr.Post("/test-no", [](const Request& rep, Response& resp){

	
	});
	
	//服务端监听起来
	svr.listen("0.0.0.0", 17878);
	return 0;
}
