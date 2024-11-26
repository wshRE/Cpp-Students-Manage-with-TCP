#pragma once
enum DataOpt
{
    //1.增--6
    TASK_ADD_STU,           //增加学生
    TASK_ADD_CLA,           //增加班级
    TASK_ADD_COU,           //增加课程
    TASK_ADD_STU_COU,       //增加学生的课程
    TASK_ADD_STU_CLA,       //增加学生的班级

//2.删--4
TASK_DEL_STU,           //删除学生
TASK_DEL_CLA,           //删除班级
TASK_DEL_COU,           //删除课程
TASK_DEL_STU_COU,       //删除学生的某门课程


//3.改--3
TASK_CHA_STU_NAME,      //修改学生姓名
TASK_CHA_CLA_NAME,      //修改班级名
TASK_CHA_COU_NAME,      //修改课程名

//4.查---6
TASK_SEE_CLA_STU,       //查询班级的学生
TASK_SEE_COU_STU,       //查询课程的学生
TASK_SEE_STU_COU,       //查询学生的课程
TASK_SEE_CLA,           //查询所有班级
TASK_SEE_STU,           //查询所有学生
TASK_SEE_COU,           //查询所有课程

//新增
TASK_SEE_ID_CLA,       //查询单一班级
TASK_SEE_ID_COU,       //查询单一课程
TASK_SEE_ID_STU,       //查询单一学生

//未实现
TASK_SEE_NAME_STU,     //模糊学生查询
TASK_SEE_NAME_COU,     //模糊课程查询
TASK_SEE_NAME_CLA,     //模糊班级查询

ERR,                     //错误码



//组合查询
    //查看课程 id xx 成绩在 xx 到 xx的学生
    TASK_SEE_STU_MARK_ONE,
    //查询x⻔以上不及格课程的同学的学号及其平均成绩
    TASK_SEE_STU_MARK_TWO,
    //查询所有学生的班级
    TASK_SEE_ALL_STU_CLA, //未实现



    //1.学生： id范围 为xx-xx 或者 名字为 xx
    TASK_SEE_OR_STU,
    //1.学生： id范围 为xx-xx 或者 名字为 xx 不去重
    TASK_SEE_OR_STU_RE,
    //1.课程： id范围 为xx-xx 或者 名字为 xx
    TASK_SEE_OR_COU,
    //1.班级： id范围 为xx-xx 或者 名字为 xx
    TASK_SEE_OR_CLA,

    //每门课都大于xx的同学
    TASK_SEE_OR_STU_COU,

    //学生-课程表
    TASK_SEE_STU_AND_COU,

    //班级-学生表
    TASK_SEE_CLA_AND_STU

};
//数据报包头
#pragma pack(1)
struct DataHead {
    DataOpt m_Opt;
    int m_nLen = 0;
};
#pragma pack()

//发送前端的数据结构
#pragma pack(1)
/*
1.添加学生 课程  ---- 课程 学生 成绩
*/
struct IdThree {
    int m_nOne;
    int m_nTwo;
    int m_nThree;
};

/*前端-----> 后端
1.添加学生班级 --- 班级  学生
*/
struct IdTwo {
    int m_nOne;
    int m_nTwo;
};

/*前端-----> 后端
1. 修改学生名字
2.修改班级名字
3.修改课程名字
*/
struct idChar {
    int nOne;
    char szTwo[1];
};
#pragma pack()



//后端----> 前端   
#pragma pack(1)
struct idName {
    int nOne;
    int nSize;
    char szTwo[1];
};
#pragma pack()