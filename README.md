# Cpp-Students-Manage-with-TCP

C++自实现TCP 学生管理系统

1. 项目基本功能：
   1. 录入学生基本信息，字段（学号，姓名）
   2. 录入课程基本信息，字段（课程号，课程名）
   3. 录入班级基本信息，字段（班级id，班级名）
   4. 学生、课程、班级支持增加、修改、删除和查询，要求能支持组合查询。
   5. 一个学生只能隶属于一个班级，一个班级可以有多个学生，一个学生可以选修多门课程，一个课程可以被多个学生选修。
   6. 学生姓名、班级名称、课程名称都可能重复。
2. 三层结构，使用socket完成客户端与服务端的通讯,由服务端操作数据库。
3. 线程池管理大量客户端请求的问题。
4. 使用UDP协议完成可靠传输（完成包排序，包校验,超时重传，三次握手）。
5. 除了UI外无使用第三方产品的二次开发接口。

