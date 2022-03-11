#include <stdio.h>
#include "httplib.h"//包含那个头文件的库

void func(const httplib::Request& req, httplib::Response& resp)
{
	resp.set_content("<html>123</html>", 15, "test/html");
	printf("recv abc\n");
}


int main()
{
	//创建httplib当中发server类对象，使用该类队形，模拟创建一个http服务器
	httplib::Server svr;
	//回调函数
	svr.Get("/abc", func);

	//指定侦听ip
	svr.listen("0.0.0.0", 18989);
	
	return 0;
}
