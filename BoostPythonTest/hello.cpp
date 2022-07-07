#include <iostream>
#include <string>
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp> // Boost.Python

int main(int argc, char* argv[])
{
    Py_Initialize(); // 初始化 Python 解释器

    try
    {
        // 添加模块代码所在路径（Python脚本文件的路径）
        boost::python::import("sys").attr("path").attr("append")(".");

        // 1. 导入模块 hello（hello.py）
        boost::python::object hello = boost::python::import("hello");

        // 2. 获取属性（函数、类等）
        boost::python::object greeting = hello.attr("greeting");

        // 3. 执行Python函数
        boost::python::object res = greeting("2022.04.23");
        // 其中第2步和第3步可以合并为
        // boost::python::object res = hello.attr("greeting")("2022.04.23");

        // 4. 打印返回结果（使用 boost::python::extract 转换）
        std::string tmp = boost::python::extract<std::string>(res);
        std::cout << tmp << " ... C++" << std::endl;
    }
    catch (const boost::python::error_already_set&)
    {
        PyErr_Print();
        return 1;
    }

    Py_Finalize(); // 终止 Python 解释器
    return 0;
}
