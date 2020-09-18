cmake_minimum_required(VERSION 3.9.5)

if (NOT DEFINED VERSION)
    set(VERSION 1.2.2)
endif ()

#最外层一个工程，单项目中不添加工程，只添加执行文件
project(deepin-calculator)

#源码程序目录
add_subdirectory(src)

#开关默认UT测试关闭
#QT左侧栏项目中找到USE_TEST键值，点击打开即可测试
#若无法点击开关，尝试工具-选项-环境-Theme切换至dark
option(USE_TEST "UT" no)
if (USE_TEST)
add_subdirectory(tests)  #tests为ut测试文件夹名
endif(USE_TEST)
