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
    //��ʼ��
    int Init(const char* szUser, const char* szPass, const char* szName) {
        memset(&this->m_sqlSelf, 0, sizeof(MYSQL));
        mysql_init(&this->m_sqlSelf);
        this->m_mConnection = mysql_real_connect(&m_sqlSelf, "localhost", szUser, szPass, szName, 3306, 0, 0);//�������ݿ� ���������˺�
        if (this->m_mConnection == NULL) {
            return -1;
        }
        return 1;
    }
    //�ر�
    void Close() {
        if (this->m_mConnection != NULL) {
            mysql_close(this->m_mConnection); //�Ͽ���musq1������
            this->m_mConnection = NULL;
        }
        memset(&this->m_sqlSelf, 0, sizeof(MYSQL));
    }
    //��ѯ
    int Inquire(const char* szSql, vector<map<CStringA, CStringA>>& vRe) {
        //���ݿ�δ����
        if (this->m_mConnection == NULL) {
            return -1;
        }
        //��ѯ
        if (mysql_query(this->m_mConnection, szSql) != 0) {
            return -1;
        }
        MYSQL_RES* result = mysql_store_result(this->m_mConnection); // ��ȡ������ѯ���õ��Ľ��
        if (result == NULL) {
            return 0;
        }
        //������
        vector<CStringA> vTemp;
        int fieldCount = (int)result->field_count;//���ؽ��������
        vTemp.resize(fieldCount);//���趨��С
        for (int i = 0; i < fieldCount; i++) {
            MYSQL_FIELD* field = mysql_fetch_field(result);  // ������һ�����ֶε����͡�  
            if (field == NULL) {
                return 0;
            }
            vTemp[i] = field->name;
        }
        //������
        int rowCount = (int)result->row_count;
        vRe.resize(rowCount);
        for (int i = 0; i < rowCount; i++) {
            MYSQL_ROW row = mysql_fetch_row(result);//�ӽ�����л�ȡ��һ���ļ�¼������ֵ����char*[]
            if (row == NULL) {
                continue;
            }
            for (int j = 0; j < fieldCount; j++) {
                vRe[i][vTemp[j]] = row[j];
            }
        }
        mysql_free_result(result);    //�ͷŽ��
        return rowCount;//��������
    }
    //2.����
    MYSQL m_sqlSelf = {};
    MYSQL* m_mConnection = NULL;
};