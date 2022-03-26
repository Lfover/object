#include <stdio.h>
#include "httplib.h"

//回调函数
//Request和Response都是在httplib.h中定义的，因此加上httplib::
void func(const httplib::Request& req, httplib::Response& resp)
{
    //给响应体一个返回值
    resp.set_content("<html>99</html>", 15, "text/html");

    printf("recv abc\n ");
}
int main()
{
    //创建httplib当中的sever类对象，使用该类对象，模拟创建一个http服务器
    httplib::Server svr;
    svr.Get("/abc",func);
    //监听
    svr.listen("0.0.0.0", 18989);

    return 0;
}
