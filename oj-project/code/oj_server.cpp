#include <iostream>
#include <cstdio>
#include "httplib.h"
int main()
{
	using namespace httplib;//包含了这个命名空间后，后面使用就不需要加httplib::
	//1.初始化httplib库的server对象
	Server svr;
	
	//2.提供三个接口，分别处理三个请求
	//2.1获取整个试题列表，get
	svr.Get("/all_questions",[](const Request& rep, Response& resp){
		//返回试题列表
		resp.set_content("<html>57-linux</html>", 21, "text/html");//21是前面传递的长度	
	});//all_questions是资源路径；lamdar表达式[]是传递的参数，（）是参数，{}是lamdar表达式的实现
	//服务端监听起来
	svr.listen("0.0.0.0", 17878);
	return 0;
}
