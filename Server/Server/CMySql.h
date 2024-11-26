#pragma once
//#include"CMyTcp.h"
#pragma comment(lib, "libmysql.lib")
#include <Windows.h>
#include <atlstr.h>
#include <vector>
#include <map>
#include <mysql.h>
using namespace std;
class CMySQL {
public:
    //初始化
    int Init(const char* szUser, const char* szPass, const char* szName) {
        memset(&this->m_sqlSelf, 0, sizeof(MYSQL));
        mysql_init(&this->m_sqlSelf);
        this->m_mConnection = mysql_real_connect(&m_sqlSelf, "localhost", szUser, szPass, szName, 3306, 0, 0);//连接数据库 包括密码账号
        if (this->m_mConnection == NULL) {
            return -1;
        }
        return 1;
    }
    //关闭
    void Close() {
        if (this->m_mConnection != NULL) {
            mysql_close(this->m_mConnection); //断开和musq1的连接
            this->m_mConnection = NULL;
        }
        memset(&this->m_sqlSelf, 0, sizeof(MYSQL));
    }
    //查询
    int Inquire(const char* szSql, vector<map<CStringA, CStringA>>& vRe) {
        //数据库未连接
        if (this->m_mConnection == NULL) {
            return -1;
        }
        //查询
        if (mysql_query(this->m_mConnection, szSql) != 0) {
            return -1;
        }
        MYSQL_RES* result = mysql_store_result(this->m_mConnection); // 获取上条查询语句得到的结果
        if (result == NULL) {
            return 0;
        }
        //遍历列
        vector<CStringA> vTemp;
        int fieldCount = (int)result->field_count;//返回结果的数量
        vTemp.resize(fieldCount);//重设定大小
        for (int i = 0; i < fieldCount; i++) {
            MYSQL_FIELD* field = mysql_fetch_field(result);  // 返回下一个表字段的类型。  
            if (field == NULL) {
                return 0;
            }
            vTemp[i] = field->name;
        }
        //遍历行
        int rowCount = (int)result->row_count;
        vRe.resize(rowCount);
        for (int i = 0; i < rowCount; i++) {
            MYSQL_ROW row = mysql_fetch_row(result);//从结果集中获取下一条的记录，返回值类似char*[]
            if (row == NULL) {
                continue;
            }
            for (int j = 0; j < fieldCount; j++) {
                vRe[i][vTemp[j]] = row[j];
            }
        }
        mysql_free_result(result);    //释放结果
        return rowCount;//返回数量
    }
    //2.数据
    MYSQL m_sqlSelf = {};
    MYSQL* m_mConnection = NULL;
};