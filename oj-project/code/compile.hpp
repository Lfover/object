#pragma once
#include <iostream>
#include <string>
#include <atomic> 
#include <json/json.h>
#include "tools.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
enum ErrorNo
{
    OK = 0;
    PRAM_ERROR,
    INTERNAL_ERROR,
    COMPILE_ERROR,
    RUN_ERROR
};
class Compiler
{
    public:

    static void CompileAndRun(Json::Value Req, Json::Value* Resp)
    {
        //1.参数是否是错误的，json串当中的code 是否为空
        if(Req["code"].empty())
        {
            (*Resp)["errorno"] = PRAM_ERROR;
            (*Resp)["reason"] = "Pram error";
            return;
        }
        //2.将代码写道文件当中去
        std::string code = Req["code"].asString();
        //文件命名约定的时候，tem_时间戳_src.cpp
        //用来区分不同的请求 
        std::string file_nameheader = WriteTmpFile(code);
        if(file_nameheader == "")
        {
            (*Resp)["errorno"] = INTERNAL_ERROR;
            (*Resp)["reason"] = "write file failed";
            return;
        }

        //3.编译
        if(!Compile(file_nameheader))
        {
            (*Resp)["errorno"] = COMPILE_ERROR;
            std::string reason;
            FileUtil::ReadFile(CompileErrorPath(file_nameheader), &reason)
            (*Resp)["reason"] = reason;
            return;
        }


        //4.运行
        int ret = Run(file_nameheader)
        //-1 -2 大于0
        if(ret != 0)
        {
            (*Resp)["errorno"] = RUN_ERROR
            (*Resp)["reason"] == "program exit by sig:" + std::to_string(ret);
            return; 

        //5.构造响应
        (*Resp)["errorno"] = OK;
        (*Resp)["reason"] = "Compile and Run ok";
        std::string stdout_str;
        FileUtil::ReadFile(StderrPath(file_nameheader), &stdout_str);
        std::cout << stdout_str << std::endl;
        (*Resp)["stdout"] = stdout_str;

        std::string stderr_str;
        FileUtil::ReadFile(StderrPath(file_nameheader), &stderr_str);
        std::cout << stderr_str << std::endl;
        (*Resp)["stderr"] = stderr_str;
         return;
         //6.删除临时文件
        Clean(file_nameheader);
         
         return;
    }
    private:

     //一个删除临时文件的函数
     static void Clean(const std::string& filename)
     {
         unlink(SrcPath(filename).c_str());
         unlink(CompileErrorPath(filename).c_str());
         unlink(ExePath(filename).c_str());
         unlink(StdoutPath(filename).c_str());
         unlink(StderrPath(filename).c_str());

     }
    static int Run(const std::string& file_name)
    {
        //1.创建子进程
        //2.子进程程序替换
        int pid = fork(0;
        if(pid < 0)
        {
            return -1;
        }
        else if(pid > 0)
        {
            //father
            int status = 0;
            waitpid(pid, &status, 0)
            //终止信号
            return status & 0x7f
        }
        else
        {
            //child
            //进程程序替换，替换创建编译出来的可执行程序
            int stdout_fd = open(StdoutPath(file_name).c_str, O_CREAT | O_WRONLY, 0666);
            if(stdout_fd < 0)
            {
                return -2;
            }
            //重定向，将标准输入1重定向到stdin_fd中
            dup2(stderr_fd, 1);

            int stderr_fd = open(StdoutPath(file_name).c_str, O_CREAT | O_WRONLY, 0666);
            if(stderr_fd < 0)
            {
                return -2;
            }
            //重定向，将标准错误1重定向到stderr_fd中
            dup2(stderr_fd, 2);
            execl(ExePath(file_name).c_str(), ExePath(file_name).c_str(), NULL);
            exit(0);
        }
        return 0;
    }

    static bool Compile(const std::string& file_name)
    {
        //1.创建一个子进程
        // 1.1父进程进行进程等待
        //  1.2子进程进行进程程序替换
        //
        int pid = fork();
        if(pid > 0)
        {
            waitpid(pid, NULL, 0);
        }
        else if(pid == 0)
        {
            //child
            //进程程序替换----》g++ SrcPath(filename) -o ExePath(filename) "-std=c++11"
            int fd = open(CompileErrorPath(file_name).c_str(), O_CREAT | O_WRONLY, 0666);
            if(fd < 0)
            {
                return false;
            }
            //将标准错误重定向为fd,标准错误的输出就会输出在文件当中
            dup2(fd, 2);

            execlp("g++", "g++", SrcPath(filename).c_str(), "-o", ExePath(file_name).c_str(), "-std=c++11", "-D", "CompileOnline", NULL);
            //如果替换失败了，就直接让子进程退出了， 如果替换成功了，不会走该逻辑
            exit(0);

        }
        else
        {
            return false;
        }
        //如果编译成功了，在tmp_file这个文件夹下,一定会差生一个可执行程序，
        //如果当前代码走到这里，判断有该可执行程序，则我们认为g++之心发成功了， 否则，认为执行失败
        //1.判断是否产生可执行程序, 2.是文件状态
        struct stat st;
        int ret = stat(ExePath(file_name).c_str(), &st);
        if(ret < 0)
        {
            return false;
        }
        return true;
    }
    static std::string StdoutPath(const std::string& filename)
    {
        return "./tmp_file/" + filename + ".stdout";
    }
    static std::string StderrPath(const std::string& filename)
    {
        return "./tmp_file/" + filename + ".stderr";
    }
    static std::string CompileErrorPath(const std::string& filename)
    {
        return "./tmp_file/" + filename + ".Compilerr";
    }
    static std::string ExePath(const std::string& filename)
    {
        return "./tmp_file/" + filename + ".executable";
    }
    static std::string SrcPath(const std::string& filename)
    {
        //./tmp_file/ + "tmp_" + std::to_string(TimeUtil::GetTimeStampMs()) + "." + std::to_string(id) + .cpp
        return "./tmp_file/" + filename + ".cpp";
    }

    static std::string WriteTmpFile(const std::string& code)
    {
        //1.组织文件名称，区分源码文件，以及后面生成的可执行程序文件
        static std::atomic_uint id(0);
        std::string tmp_filename = "tmp_" + std::to_string(TimeUtil::GetTimeStampMs()) + "." + std::to_string(id);
        id++;
        //2.code写到文件当中去
        //tmp_stamp.cpp
        FileUtil::WriteFile(SrcPath(tmp_filename), code);
        return tmp_filename;
    }
};
