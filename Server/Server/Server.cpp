#include"CMyTcp.h"
#include <iostream>
#include"ThreadPool.h"
#include"CTask.h"


int main()
{
    CMyTcp mytcp;
    CMySQL mysql;
    int nRet = mytcp.Accept("127.0.0.1", 0x9527);
    nRet = (nRet == 1) ? mysql.Init("root", "toor", "exam1") : nRet;
    if (nRet != 1) {
        cout << "初始化失败" << endl;
        return 0;
    }
    vector<map<CStringA, CStringA>>vRe;
    mysql.Inquire("set names gb2312", vRe);
    CThreadPool tp;
    tp.CreateThreadPool(5);
    //循环接收信息
    while (1) {
        //1.收包头
        DataHead dh;
        mytcp.Recv((char*)&dh, sizeof(dh));
        char* szBuff = new char[dh.m_nLen];
        mytcp.Recv(szBuff, dh.m_nLen);
        //2.类型判断
        switch (dh.m_Opt) {
            //1.增
                                //增加学生
        case TASK_ADD_STU: {
            tp.AddTask(new CTaskAddStu(&mytcp, &mysql, szBuff));
            break;
        }
                         //增加班级
        case TASK_ADD_CLA: {
            tp.AddTask(new CTaskAddCla(&mytcp, &mysql, szBuff));
            break;
        }
                         //增加课程
        case TASK_ADD_COU: {
            tp.AddTask(new CTaskAddCou(&mytcp, &mysql, szBuff));
            break;
        }
                         //增加学生的课程-----包括成绩
        case TASK_ADD_STU_COU: {
            IdThree* it = (IdThree*)szBuff;
            tp.AddTask(new CTaskAddStuCou(&mytcp, &mysql, it->m_nOne, it->m_nTwo, it->m_nThree));
            delete szBuff;
            break;
        }
                             //增加学生的班级
        case TASK_ADD_STU_CLA: {
            IdTwo* it = (IdTwo*)szBuff;
            tp.AddTask(new CTaskAddStuCla(&mytcp, &mysql, it->m_nOne, it->m_nTwo));
            delete szBuff;
            break;
        }


                             //2.删
                                                 //删除学生          
        case TASK_DEL_STU: {
            tp.AddTask(new CTaskDelStu(&mytcp, &mysql, szBuff));
            break;
        }
                         //删除班级
        case TASK_DEL_CLA: {
            tp.AddTask(new CTaskDelCla(&mytcp, &mysql, szBuff));
            break;
        }
                         //删除课程
        case TASK_DEL_COU: {

            tp.AddTask(new CTaskDelCou(&mytcp, &mysql, szBuff));
            break;
        }
                         //删除学生课程
        case TASK_DEL_STU_COU: {
            IdTwo* it = (IdTwo*)szBuff;
            tp.AddTask(new CTaskDelStuCou(&mytcp, &mysql, it->m_nOne, it->m_nTwo));
            delete[] szBuff;
            break;
        }



                             //3.改


                                                      //修改学生姓名
        case TASK_CHA_STU_NAME: {

            tp.AddTask(new CTaskUpdStuName(&mytcp, &mysql, szBuff));
            break;
        }
                              //修改班级名      
        case TASK_CHA_CLA_NAME: {

            tp.AddTask(new CTaskUpdClaName(&mytcp, &mysql, szBuff));
            break;
        }
                              //修改课程名
        case TASK_CHA_COU_NAME: {
            tp.AddTask(new CTaskUpdCouName(&mytcp, &mysql, szBuff));
            break;
        }


                              //4.查
                                                        //查询班级的学生
        case TASK_SEE_CLA_STU: {
            tp.AddTask(new CTaskSeeClaStu(&mytcp, &mysql, szBuff));
            break;
        }

                             //查询课程的学生
        case TASK_SEE_COU_STU: {
            tp.AddTask(new CTaskSeeCouStu(&mytcp, &mysql, szBuff));
            break;
        }
                             //查询学生的课程
        case TASK_SEE_STU_COU: {
            tp.AddTask(new CTaskSeeStuCou(&mytcp, &mysql, szBuff));
            break;
        }
                             //查询班级数据库
        case TASK_SEE_CLA: {
            tp.AddTask(new CTaskSeeCla(&mytcp, &mysql));
            break;
        }
                         //查询学生数据库
        case TASK_SEE_STU: {
            tp.AddTask(new CTaskSeeStu(&mytcp, &mysql));
            break;
        }
                         //查询课程数据库
        case TASK_SEE_COU: {
            tp.AddTask(new CTaskSeeCou(&mytcp, &mysql));
            break;
        }
                         //新增
                                          //查询单一班级
        case TASK_SEE_ID_CLA: {
            tp.AddTask(new CTaskSeeIdCla(&mytcp, &mysql, szBuff));
            break;
        }
                            //查询单一课程
        case TASK_SEE_ID_COU: {
            tp.AddTask(new CTaskSeeIdCou(&mytcp, &mysql, szBuff));
            break;
        }
                            //查询单一学生
        case TASK_SEE_ID_STU: {
            tp.AddTask(new CTaskSeeIdStu(&mytcp, &mysql, szBuff));
            break;
        }


        //组合查询
                            //查看课程 id xx 成绩在 xx 到 xx的学生id
        case TASK_SEE_STU_MARK_ONE: {
            IdThree* it = (IdThree*)szBuff;
            tp.AddTask(new CTaskSeeStuMarkOne(&mytcp, &mysql, it->m_nOne, it->m_nTwo, it->m_nThree));
            delete []szBuff;
            break;
        }
                            //学生： id范围 为xx-xx 或者 名字为 xx
        case TASK_SEE_OR_STU: {
            tp.AddTask(new CTaskSeeOrStu(&mytcp, &mysql, szBuff));
            break;
        }
                            //学生： id范围 为xx-xx 或者 名字为 xx
        case TASK_SEE_OR_COU: {
            tp.AddTask(new CTaskSeeOrCou(&mytcp, &mysql, szBuff));
            break;
        }
                            //学生： id范围 为xx-xx 或者 名字为 xx
        case TASK_SEE_OR_CLA: {
            tp.AddTask(new CTaskSeeOrCla(&mytcp, &mysql, szBuff));
            break;
        }
                            //每门课都在范围内的同学
        case TASK_SEE_OR_STU_COU: {
            tp.AddTask(new CTaskOrStuCou(&mytcp, &mysql, szBuff));
            break;
        }
                            //学生-课程表
        case TASK_SEE_STU_AND_COU: {
            tp.AddTask(new CTaskSeeStuAndCou(&mytcp, &mysql));
            break;
        }
                            //班级-学生表
        case TASK_SEE_CLA_AND_STU: {
            tp.AddTask(new CTaskSeeClaAndStu(&mytcp, &mysql));
            break;
        }
        }
    }
}