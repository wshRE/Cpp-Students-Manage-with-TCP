#pragma once
#include"ThreadPool.h"
#include"CMySql.h"
#include"CMyTcp.h"
#include <string>


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

struct idTwoChar {
    int nOne;
    int nTwo;
    char szThree[1];
};

#pragma pack()



//后端----> 前端   
#pragma pack(1)
struct idName {
    int nOne;
    int nSize;
    char szTwo[1];
};


struct idNameTwo {
    int nOne;
    int nSize;
    int nMark;
    char szTwo[1];
};


struct idTwoSend {
    int nOne;
    int nTwo;
};

struct idThreeSend {
    int nOne;
    int nTwo;
    int nThree;
};
#pragma pack()


/// <summary>
/// 添加包头并发送
/// </summary>
/// <param name="CMyTcp">tcp</param>
/// <param name="nFlag">标志</param>
/// <param name="szBuff">数据</param>
/// <param name="nSize">数据大小</param>
void SendData(CMyTcp* mytcp, DataOpt nFlag, char* szBuff, int nSize) {
    //1.打包
    char* szBuf = new char[nSize + sizeof(DataHead)];
    ZeroMemory(szBuf, nSize + sizeof(DataHead));
    DataHead dh;
    dh.m_Opt = nFlag;
    dh.m_nLen = nSize;
    memcpy(szBuf, (char*)&dh, sizeof(dh));
    if (nSize != 0) {
        memcpy(szBuf + sizeof(dh), szBuff, nSize);
    }
    //2.发送
    mytcp->Send(szBuf, nSize + sizeof(dh));
    delete[] szBuf;
}



/*
1.增
增加学生
增加班级
增加课程
增加学生的课程
增加学生的班级
*/
//1.增加学生
class CTaskAddStu : public ITask {
public:
    CTaskAddStu(CMyTcp* pMyTcp, CMySQL* pMySQL, char* pData) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_pData = pData;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_pData == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, "insert into student values(NULL , \"%s\")", m_pData);//id自增，仅需名字
        m_pMySQL->Inquire("set names gb2312", vRe);
        int nCount = m_pMySQL->Inquire(szSql, vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
        }
        delete m_pData;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_pData = NULL;   //数据
};

//2.增加班级
class CTaskAddCla : public ITask {
public:
    CTaskAddCla(CMyTcp* pMyTcp, CMySQL* pMySQL, char* pData) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_pData = pData;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_pData == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        m_pMySQL->Inquire("set names gb2312", vRe);
        sprintf(szSql, "INSERT INTO class VALUES(null,\"%s\")", m_pData);
        int nCount = m_pMySQL->Inquire(szSql, vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }
        delete m_pData;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_pData = NULL;   //数据
};

//3.增加课程
class CTaskAddCou : public ITask {
public:
    CTaskAddCou(CMyTcp* pMyTcp, CMySQL* pMySQL, char* pData) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_pData = pData;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_pData == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        m_pMySQL->Inquire("set names gb2312", vRe);
        sprintf(szSql, "INSERT INTO course VALUES(null,\"%s\")", m_pData);
        int nCount = m_pMySQL->Inquire(szSql, vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }
        delete m_pData;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_pData = NULL;   //数据
};

//4.增加学生的课程
class CTaskAddStuCou : public ITask {
public:
    CTaskAddStuCou(CMyTcp* pMyTcp, CMySQL* pMySQL, int nCouId, int nStuId, int nCouMark) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_nCouId = nCouId;
        m_nStuId = nStuId;
        m_nCouMark = nCouMark;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_nCouId == NULL || m_nStuId == NULL || m_nCouMark == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, "INSERT INTO stu_cou(sid,cid,mark ) VALUES (%d,%d,%d )", m_nStuId, m_nCouId, m_nCouMark);
        int nCount = m_pMySQL->Inquire(szSql, vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    int m_nCouId = NULL;   //课程id
    int m_nStuId = NULL;   //学生id
    int m_nCouMark = NULL;   //课程分数
};

//5.增加学生的班级
class CTaskAddStuCla : public ITask {
public:
    CTaskAddStuCla(CMyTcp* pMyTcp, CMySQL* pMySQL, int nClaId, int nStuId) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_nClaId = nClaId;
        m_nStuId = nStuId;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_nClaId == NULL || m_nStuId == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, "INSERT INTO cla_stu(sid,cid) VALUES (%d,%d )", m_nStuId, m_nClaId);
        int nCount = m_pMySQL->Inquire(szSql, vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    int m_nClaId;   //班级id
    int m_nStuId;   //学生id
};


/*

2.删

删除学生
删除班级
删除课程
删除学生的某门课程

*/



//1.删除学生
class CTaskDelStu : public ITask {
public:
    CTaskDelStu(CMyTcp* pMyTcp, CMySQL* pMySQL, char* pData) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_pData = pData;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_pData == NULL) {
            return;
        }
                vector<map<CStringA, CStringA>>vRe;
        //char szSql[1024] = { 0 };
        //sprintf(szSql, "delete from student where id = %d", *(int*)m_pData);
        string szSql = "delete from student where id = ";
        szSql += m_pData;
        int nCount = m_pMySQL->Inquire(szSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }
        delete m_pData;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_pData = NULL;   //数据
};

//2.删除班级
class CTaskDelCla : public ITask {
public:
    CTaskDelCla(CMyTcp* pMyTcp, CMySQL* pMySQL, char* pData) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_pData = pData;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_pData == NULL) {
            return;
        }
                vector<map<CStringA, CStringA>>vRe;
        string sSql = "delete from class where id = ";
        sSql += m_pData;
        int nCount = m_pMySQL->Inquire(sSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }
        delete m_pData;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_pData = NULL;   //数据
};

//3.删除课程
class CTaskDelCou : public ITask {
public:
    CTaskDelCou(CMyTcp* pMyTcp, CMySQL* pMySQL, char* pData) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_pData = pData;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_pData == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        string szSql = "delete from course where id = ";
        szSql += m_pData;

        int nCount = m_pMySQL->Inquire(szSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }
        delete m_pData;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_pData = NULL;   //数据
};

//4.删除学生的某个课程
class CTaskDelStuCou : public ITask {
public:
    CTaskDelStuCou(CMyTcp* pMyTcp, CMySQL* pMySQL, int nCouId, int nStuId) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_nCouId = nCouId;
        m_nStuId = nStuId;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_nCouId == NULL || m_nStuId == NULL) {
            return;
        }
                vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, "DELETE from stu_cou where sid = %d and cid = %d ", m_nStuId, m_nCouId);
        int nCount = m_pMySQL->Inquire(szSql, vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    int m_nCouId = NULL;   //课程id
    int m_nStuId = NULL;   //学生id
};




/*
3.改
修改学生姓名
修改班级名
修改课程名

*/
//1.修改学生 名字
class CTaskUpdStuName : public ITask {
public:
    CTaskUpdStuName(CMyTcp* pMyTcp, CMySQL* pMySQL, char* szBuff) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_szBuff = szBuff;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_szBuff == NULL) {
            return;
        }
        idChar* ic = (idChar*)m_szBuff;
                vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, " update student set name = \"%s\" ", ic->szTwo);

        string sSql = szSql;
        sSql += "where id = ";
        sSql += to_string(ic->nOne);
        m_pMySQL->Inquire("set names gb2312", vRe);
        int nCount = m_pMySQL->Inquire(sSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }
        delete m_szBuff;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_szBuff = NULL;
};

//2.修改班级 名字
class CTaskUpdClaName : public ITask {
public:
    CTaskUpdClaName(CMyTcp* pMyTcp, CMySQL* pMySQL, char* szBuff) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_szBuff = szBuff;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_szBuff == NULL) {
            return;
        }
        idChar* ic = (idChar*)m_szBuff;
                vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, "update class set name = \"%s\"  ", ic->szTwo);

        string sSql = szSql;
        sSql += "where id = ";
        sSql += to_string(ic->nOne);

        m_pMySQL->Inquire("set names gb2312", vRe);
        int nCount = m_pMySQL->Inquire(sSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }
        delete m_szBuff;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_szBuff = NULL;   //班级id
};

//3.修改课程 名字
class CTaskUpdCouName : public ITask {
public:
    CTaskUpdCouName(CMyTcp* pMyTcp, CMySQL* pMySQL, char* szBuff) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_szBuff = szBuff;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_szBuff == NULL) {
            return;
        }
        idChar* ic = (idChar*)m_szBuff;
                vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, "update course set name = \"%s\" ", ic->szTwo);

        string sSql = szSql;
        sSql += "where id = ";
        sSql += to_string(ic->nOne);
        m_pMySQL->Inquire("set names gb2312", vRe);
        int nCount = m_pMySQL->Inquire(sSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
        }
        delete m_szBuff;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_szBuff;
};





/*
4.查询

查询班级的学生
查询课程的学生
查询学生的课程
查询班级---所有
查询学生---所有
查询课程---所有
*/

//查询班级的学生
class CTaskSeeClaStu : public ITask {
public:
    CTaskSeeClaStu(CMyTcp* pMyTcp, CMySQL* pMySQL, char* pClaId) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_pClaId = pClaId;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_pClaId == NULL) {
            return;
        }
                vector<map<CStringA, CStringA>>vRe;
        string sSql = "select id,name from student where id in (select sid from cla_stu where cid = ";
        sSql += m_pClaId;
        sSql += ")";


        int nCount = m_pMySQL->Inquire(sSql.c_str(), vRe); //执行sql

        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
            delete m_pClaId;
            return;
        }

        // ...待补充： 数据写入char* 并发送
        //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int)*2 + vRe[i]["name"].GetLength() + 1;
        }
        nSize += sizeof(int);
        //2.组装
        char* szBuff = new char[nSize  ];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount,sizeof(int));
        idName* pidName = (idName*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());
            pidName = (idName*)((char*)pidName + sizeof(int)*2 + pidName->nSize + 1);
        }

        //3.发送
        SendData(m_pMyTcp, TASK_SEE_CLA_STU, szBuff, nSize);
        delete m_pClaId;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_pClaId = NULL;   //课程id
};


//2.查询修了课程的学生
class CTaskSeeCouStu : public ITask {
public:
    CTaskSeeCouStu(CMyTcp* pMyTcp, CMySQL* pMySQL, char* pCouId) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_pCouId = pCouId;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_pCouId == NULL) {
            return;
        }
                vector<map<CStringA, CStringA>>vRe;
        //char szSql[1024] = { 0 };
        //sprintf(szSql, "select id,name from student where id in (select sid from stu_cou where cid = %d)", *(int*)m_pCouId);

        string sSql = "select st.id,st.name,sc.mark from student st inner join stu_cou sc on st.id = sc.sid and sc.cid = ";
        sSql += m_pCouId;

        int nCount = m_pMySQL->Inquire(sSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
        }

        // ...待补充： 数据写入char* 并发送
        //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int)*3 + vRe[i]["name"].GetLength() + 1;
        }
        //2.组装
        nSize += sizeof(int);//数量
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idNameTwo* pidName = (idNameTwo*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            pidName->nMark = atoi(vRe[i]["mark"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());

            pidName = (idNameTwo*)((char*)pidName + sizeof(int)*3 + pidName->nSize + 1);
        }

        //3.发送
        SendData(m_pMyTcp, TASK_SEE_COU_STU, szBuff, nSize);

        delete m_pCouId;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_pCouId = NULL;   //课程id
};


//3.查询学生的课程
class CTaskSeeStuCou : public ITask {
public:
    CTaskSeeStuCou(CMyTcp* pMyTcp, CMySQL* pMySQL, char* pStuId) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_pStuId = pStuId;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_pStuId == NULL) {
            return;
        }
                vector<map<CStringA, CStringA>>vRe;
        //char szSql[1024] = { 0 };
        //sprintf(szSql, "select id,name from course where id in (select cid from stu_cou where sid = %d)", *(int*)m_pStuId);
        
                
        string sSql = "select st.id,st.name,sc.mark from course st inner join stu_cou sc on st.id = sc.cid and sc.sid = ";
        sSql += m_pStuId;
        
        int nCount = m_pMySQL->Inquire(sSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
        }

        // ...待补充： 数据写入char* 并发送
        //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int) * 3 + vRe[i]["name"].GetLength() + 1;
        }
        //2.组装
        nSize += sizeof(int);//数量
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idNameTwo* pidName = (idNameTwo*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            pidName->nMark = atoi(vRe[i]["mark"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());

            pidName = (idNameTwo*)((char*)pidName + sizeof(int) * 3 + pidName->nSize + 1);
        }
        //3.发送
        SendData(m_pMyTcp, TASK_SEE_STU_COU, szBuff, nSize);


        delete m_pStuId;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_pStuId = NULL;   //学生id
};




//4.查询班级列表
class CTaskSeeCla : public ITask {
public:
    CTaskSeeCla(CMyTcp* pMyTcp, CMySQL* pMySQL) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL) {
            return;
        }
                vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, "select id,name from class");
        int nCount = m_pMySQL->Inquire(szSql, vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
        }

        // ...待补充： 数据写入char* 并发送
                //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int)*2 + vRe[i]["name"].GetLength() + 1;
        }
        nSize += sizeof(int);
        //2.组装
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idName* pidName = (idName*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());
            pidName = (idName*)((char*)pidName + sizeof(int) * 2 + pidName->nSize + 1);
        }
        //3.发送
        SendData(m_pMyTcp, TASK_SEE_CLA, szBuff, nSize);


    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
};


//5.查询课程列表
class CTaskSeeCou : public ITask {
public:
    CTaskSeeCou(CMyTcp* pMyTcp, CMySQL* pMySQL) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL) {
            return;
        }
                vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, "select id,name from course");
        int nCount = m_pMySQL->Inquire(szSql, vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
        }

        // ...待补充： 数据写入char* 并发送
                //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int) * 2 + vRe[i]["name"].GetLength() + 1;
        }
        nSize += sizeof(int);
        //2.组装
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idName* pidName = (idName*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());
            pidName = (idName*)((char*)pidName + sizeof(int) * 2 + pidName->nSize + 1);
        }

        //3.发送
        SendData(m_pMyTcp, TASK_SEE_COU, szBuff, nSize);


    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
};


//6.查询学生列表
class CTaskSeeStu : public ITask {
public:
    CTaskSeeStu(CMyTcp* pMyTcp, CMySQL* pMySQL) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL ) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, "select id,name from student");
        int nCount = m_pMySQL->Inquire(szSql, vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
        }

        // ...待补充： 数据写入char* 并发送
                   //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int) * 2 + vRe[i]["name"].GetLength() + 1;
        }
        nSize += sizeof(int);
        //2.组装
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idName* pidName = (idName*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());
            pidName = (idName*)((char*)pidName + sizeof(int) * 2 + pidName->nSize + 1);
        }

        //3.发送
        SendData(m_pMyTcp, TASK_SEE_STU, szBuff, nSize);


    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
};



//7.查询单一班级
class CTaskSeeIdCla : public ITask {
public:
    CTaskSeeIdCla(CMyTcp* pMyTcp, CMySQL* pMySQL, char* pData) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_pData = pData;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_pData == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        string szSql = "select id,name from class where id = ";
        szSql += m_pData;

        int nCount = m_pMySQL->Inquire(szSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
        }

        //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int) * 2 + vRe[i]["name"].GetLength() + 1;
        }
        nSize += sizeof(int);
        //2.组装
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idName* pidName = (idName*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());
            pidName = (idName*)((char*)pidName + sizeof(int) * 2 + pidName->nSize + 1);
        }
        //3.发送
        SendData(m_pMyTcp, TASK_SEE_ID_CLA, szBuff, nSize);



        delete m_pData;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_pData = NULL;   //数据
};


//8.查询单一课程
class CTaskSeeIdCou :public ITask{
public:
    CTaskSeeIdCou(CMyTcp* pMyTcp, CMySQL* pMySQL, char* pData) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_pData = pData;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_pData == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        string szSql = "select id,name from course where id = ";
        szSql += m_pData;

        int nCount = m_pMySQL->Inquire(szSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
            delete m_pData;
            return;
        }

        //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int) * 2 + vRe[i]["name"].GetLength() + 1;
        }
        nSize += sizeof(int);
        //2.组装
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idName* pidName = (idName*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());
            pidName = (idName*)((char*)pidName + sizeof(int) * 2 + pidName->nSize + 1);
        }

        //3.发送
        SendData(m_pMyTcp, TASK_SEE_ID_COU, szBuff, nSize);


        delete m_pData;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_pData = NULL;   //数据
};



//9.查询单一学生
class CTaskSeeIdStu :public ITask {
public:
    CTaskSeeIdStu(CMyTcp* pMyTcp, CMySQL* pMySQL, char* pData) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_pData = pData;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_pData == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        string szSql = "select id,name from student where id = ";
        szSql += m_pData;

        int nCount = m_pMySQL->Inquire(szSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
            delete m_pData;
            return;
        }

        //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int) * 2 + vRe[i]["name"].GetLength() + 1;
        }
        nSize += sizeof(int);
        //2.组装
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idName* pidName = (idName*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());
            pidName = (idName*)((char*)pidName + sizeof(int) * 2 + pidName->nSize + 1);
        }

        //3.发送
        SendData(m_pMyTcp, TASK_SEE_ID_STU, szBuff, nSize);


        delete m_pData;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_pData = NULL;   //数据
};




//查看课程 id xx 成绩在 xx 到 xx的学生
//TASK_SEE_STU_MARK_ONE,
//查询x⻔以上不及格课程的同学的学号及其平均成绩
//TASK_SEE_STU_MARK_TWO,
//查询课程编号为xxx且课程成绩在(xx,xx)的学⽣的学号和姓名； 
//TASK_SEE_ALL_STU_CLA



//1. 查看课程 id xx 成绩在 xx 到 xx的学生
class CTaskSeeStuMarkOne :public ITask {
public:
    CTaskSeeStuMarkOne(CMyTcp* pMyTcp, CMySQL* pMySQL, int nCouId, int nMarkL, int nMarkR) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL  ;
        m_nCouId = nCouId;  
        m_nMarkL = nMarkL;  
        m_nMarkR = nMarkR;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_nCouId == NULL || m_nMarkL == NULL || m_nMarkR == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, "select sid id,(select name from student where id = stu_cou.sid) name ,mark from stu_cou where cid = %d and mark >= %d and mark <= %d", m_nCouId, m_nMarkL, m_nMarkR);

        int nCount = m_pMySQL->Inquire(szSql, vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }


        //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int) * 3 + vRe[i]["name"].GetLength() + 1;
        }
        //2.组装
        nSize += sizeof(int);//数量
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idNameTwo* pidName = (idNameTwo*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            pidName->nMark = atoi(vRe[i]["mark"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());

            pidName = (idNameTwo*)((char*)pidName + sizeof(int) * 3 + pidName->nSize + 1);
        }
        //3.发送
        SendData(m_pMyTcp, TASK_SEE_STU_COU, szBuff, nSize);


    }
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    int m_nCouId = NULL;   //课程id
    int m_nMarkL = NULL;   //成绩左
    int m_nMarkR = NULL;   //成绩右
};



//2.查询x⻔以上不及格课程的同学的学号及其平均成绩
class CTaskSeeStuMarkTwo :public ITask {
public:
    CTaskSeeStuMarkTwo(CMyTcp* pMyTcp, CMySQL* pMySQL, int nMark, int nCouNum) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_nMark   = nMark  ;
        m_nCouNum = nCouNum;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_nMark == NULL || m_nCouNum == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };

        sprintf(szSql, "select (select id,name from student where id = stu_cou.sid),avg(mark) mark  from stu_cou   where mark < %d group by sid having count(mark) >= %d ", m_nMark, m_nCouNum);
        int nCount = m_pMySQL->Inquire(szSql, vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }

        //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int) * 3 + vRe[i]["name"].GetLength() + 1;
        }
        //2.组装
        nSize += sizeof(int);//数量
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idNameTwo* pidName = (idNameTwo*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            pidName->nMark = atoi(vRe[i]["mark"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());

            pidName = (idNameTwo*)((char*)pidName + sizeof(int) * 3 + pidName->nSize + 1);
        }
        //3.发送
        SendData(m_pMyTcp, TASK_SEE_STU_COU, szBuff, nSize);


    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    int m_nMark  = NULL;   //平均成句
    int m_nCouNum = NULL;   //门数

};


//查看所有学生的所有班级
class CTaskSeeAllStuCla :public ITask {
    
};


/*
//1.学生： id范围 为xx-xx 或者 名字为 xx
TASK_SEE_OR_STU,
//2.学生： id范围 为xx-xx 或者 名字为 xx 不去重
TASK_SEE_OR_STU_RE,
//3.课程： id范围 为xx-xx 或者 名字为 xx
TASK_SEE_OR_COU,
//4.班级： id范围 为xx-xx 或者 名字为 xx
TASK_SEE_OR_CLA

*/

//1.学生： id范围 为xx-xx 或者 名字为 xx
class CTaskSeeOrStu :public ITask {
public:
    CTaskSeeOrStu(CMyTcp* pMyTcp, CMySQL* pMySQL, char* szBuff) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_szBuff = szBuff;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_szBuff == NULL) {
            return;
        }
        idTwoChar* ic = (idTwoChar*)m_szBuff;
        vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, " select id,name from student where name = \"%s\" ", ic->szThree);

        string sSql = szSql;
        sSql += "union ";
        sSql += "select id,name from student where id >= ";
        sSql += to_string(ic->nOne);
        sSql += " and  id <=";
        sSql += to_string(ic->nTwo);


        m_pMySQL->Inquire("set names gb2312", vRe);
        int nCount = m_pMySQL->Inquire(sSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }



        //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int) * 2 + vRe[i]["name"].GetLength() + 1;
        }
        nSize += sizeof(int);
        //2.组装
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idName* pidName = (idName*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());
            pidName = (idName*)((char*)pidName + sizeof(int) * 2 + pidName->nSize + 1);
        }
        //3.发送
        SendData(m_pMyTcp, TASK_SEE_OR_STU, szBuff, nSize);

        delete m_szBuff;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_szBuff = NULL;
};


//3.课程： id范围 为xx-xx 或者 名字为 xx
class CTaskSeeOrCou :public ITask {
public:
    CTaskSeeOrCou(CMyTcp* pMyTcp, CMySQL* pMySQL, char* szBuff) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_szBuff = szBuff;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_szBuff == NULL) {
            return;
        }
        idTwoChar* ic = (idTwoChar*)m_szBuff;
        vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, " select id,name from course where name = \"%s\" ", ic->szThree);

        string sSql = szSql;
        sSql += "union ";
        sSql += "select id,name from course where id >=  ";
        sSql += to_string(ic->nOne);
        sSql += " and id <=  ";
        sSql += to_string(ic->nTwo);


        m_pMySQL->Inquire("set names gb2312", vRe);
        int nCount = m_pMySQL->Inquire(sSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }



        //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int) * 2 + vRe[i]["name"].GetLength() + 1;
        }
        nSize += sizeof(int);
        //2.组装
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idName* pidName = (idName*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());
            pidName = (idName*)((char*)pidName + sizeof(int) * 2 + pidName->nSize + 1);
        }
        //3.发送
        SendData(m_pMyTcp, TASK_SEE_OR_COU, szBuff, nSize);

        delete m_szBuff;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_szBuff = NULL;
};


//4.班级： id范围 为xx-xx 或者 名字为 xx
class CTaskSeeOrCla :public ITask {
public:
    CTaskSeeOrCla(CMyTcp* pMyTcp, CMySQL* pMySQL, char* szBuff) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_szBuff = szBuff;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_szBuff == NULL) {
            return;
        }
        idTwoChar* ic = (idTwoChar*)m_szBuff;
        vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, " select id,name from class where name = \"%s\" ", ic->szThree);

        string sSql = szSql;
        sSql += "union ";
        sSql += "select id,name from class where id >=  ";
        sSql += to_string(ic->nOne);
        sSql += " and id <=  ";
        sSql += to_string(ic->nTwo);


        m_pMySQL->Inquire("set names gb2312", vRe);
        int nCount = m_pMySQL->Inquire(sSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            //SendData(m_pMyTcp, OPT_NULL, NULL, 0);
        }



        //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int) * 2 + vRe[i]["name"].GetLength() + 1;
        }
        nSize += sizeof(int);
        //2.组装
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idName* pidName = (idName*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());
            pidName = (idName*)((char*)pidName + sizeof(int) * 2 + pidName->nSize + 1);
        }
        //3.发送
        SendData(m_pMyTcp, TASK_SEE_OR_COU, szBuff, nSize);

        delete m_szBuff;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_szBuff = NULL;
};


//每门课都在范围内的同学
//TASK_SEE_OR_STU_COU
class CTaskOrStuCou :public ITask {
public:
    CTaskOrStuCou(CMyTcp* pMyTcp, CMySQL* pMySQL, char* pData) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
        m_pData = pData;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL || m_pData == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        string szSql = "SELECT id,name from student where id in (select distinct sid from stu_cou where sid not in (SELECT sid from stu_cou where mark is  null   or not mark<= ";
        szSql += m_pData;
        szSql += "))";


        int nCount = m_pMySQL->Inquire(szSql.c_str(), vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
            delete m_pData;
            return;
        }

        //1.求数据长度
        int nSize = 0;
        for (int i = 0; i < nCount; i++) {
            nSize = nSize + sizeof(int) * 2 + vRe[i]["name"].GetLength() + 1;
        }
        nSize += sizeof(int);
        //2.组装
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));
        idName* pidName = (idName*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["id"].GetString());
            pidName->nSize = strlen(vRe[i]["name"].GetString());
            strcpy(pidName->szTwo, vRe[i]["name"].GetString());
            pidName = (idName*)((char*)pidName + sizeof(int) * 2 + pidName->nSize + 1);
        }

        //3.发送
        SendData(m_pMyTcp, TASK_SEE_OR_STU_COU, szBuff, nSize);


        delete m_pData;
    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
    char* m_pData = NULL;   //数据
};




//学生-课程表   TASK_SEE_STU_AND_COU
class CTaskSeeStuAndCou : public ITask {
public:
    CTaskSeeStuAndCou(CMyTcp* pMyTcp, CMySQL* pMySQL) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, "select sid,cid,mark from stu_cou");
        int nCount = m_pMySQL->Inquire(szSql, vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
        }


        //1.求数据长度
        int nSize = sizeof(int) * 3 * nCount;

        //2.组装
        nSize += sizeof(int);//数量
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));

        idThreeSend* pidName = (idThreeSend*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["sid"].GetString());
            pidName->nTwo = atoi(vRe[i]["cid"].GetString());
            pidName->nThree = atoi(vRe[i]["mark"].GetString());

            pidName = (idThreeSend*)((char*)pidName + sizeof(int) * 3);
        }
        //3.发送
        SendData(m_pMyTcp, TASK_SEE_STU_AND_COU, szBuff, nSize);

    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
};


//班级-学生表   TASK_SEE_CLA_AND_STU
class CTaskSeeClaAndStu : public ITask {
public:
    CTaskSeeClaAndStu(CMyTcp* pMyTcp, CMySQL* pMySQL) {
        m_pMyTcp = pMyTcp;
        m_pMySQL = pMySQL;
    }
    virtual void ExceuteTask() {
        if (m_pMyTcp == NULL || m_pMySQL == NULL) {
            return;
        }
        vector<map<CStringA, CStringA>>vRe;
        char szSql[1024] = { 0 };
        sprintf(szSql, "select sid,cid from cla_stu");
        int nCount = m_pMySQL->Inquire(szSql, vRe); //执行sql
        if (nCount == -1) {
            //sql执行失败
            SendData(m_pMyTcp, ERR, NULL, 0);
        }

        //1.求数据长度
        int nSize = sizeof(int) * 2 * nCount;

        //2.组装
        nSize += sizeof(int);//数量
        char* szBuff = new char[nSize];
        ZeroMemory(szBuff, nSize);
        memcpy(szBuff, (char*)&nCount, sizeof(int));

        idTwoSend* pidName = (idTwoSend*)(szBuff + sizeof(int));
        for (int i = 0; i < nCount; i++) {
            pidName->nOne = atoi(vRe[i]["sid"].GetString());
            pidName->nTwo = atoi(vRe[i]["cid"].GetString());

            pidName = (idTwoSend*)((char*)pidName + sizeof(int) * 2);
        }
        //3.发送
        SendData(m_pMyTcp, TASK_SEE_CLA_AND_STU, szBuff, nSize);


    }
private:
    CMyTcp* m_pMyTcp = NULL; //服务端指针
    CMySQL* m_pMySQL = NULL;//数据库指针
};