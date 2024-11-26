#pragma once
#include <Windows.h>
#include <stdio.h>
#include <queue>
#include <mutex>
using namespace std;
//����ӿ�----ʹ�ü����ݸ�ʽ�ļ���
class ITask
{
public:
    virtual void ExceuteTask() = 0;//�ӿڣ�ִ������
};

class CThreadPool
{
public:
    //�����̳߳�
    void CreateThreadPool(int nThreadCount/*�Լ��趨�߳���*/) {
        m_nThreadCount = nThreadCount;
        m_isRunning = TRUE;
        // �����ź���
        m_hSemphore = CreateSemaphoreA(NULL, 0, MAXLONG, NULL);
        //�����ٽ���
        InitializeCriticalSection(&m_Criti);
        //�����߳�
        m_hThreadArr = new HANDLE[m_nThreadCount]();
        for (int i = 0; i < m_nThreadCount; i++) {
            m_hThreadArr[i] = CreateThread(NULL, 0, ThreadFunc, this, 0, NULL);
        }
    }
    //�ر��̳߳�
    void CloseThreadPool()
    {
        //֪ͨ�����߳��˳�
        m_isRunning = FALSE;
        for (int i = 0; i < m_nThreadCount; i++) {
            ReleaseSemaphore(m_hSemphore, 1, NULL);
        }
        //�ȴ�ȫ�ֳ��˳�
        WaitForMultipleObjects(m_nThreadCount, m_hThreadArr, TRUE, -1);
        //������Դ
        for (int i = 0; i < m_nThreadCount; i++) {
            CloseHandle(m_hThreadArr[i]);
        }
        CloseHandle(m_hSemphore);
        DeleteCriticalSection(&m_Criti);
        delete[] m_hThreadArr;
        m_hThreadArr = NULL;
        m_nThreadCount = 0;
        while (m_queTasks.size() > 0) {
            ITask* pTask = m_queTasks.front();
            m_queTasks.pop();
            delete pTask;
        }
    }
    // �������
    void AddTask(ITask* pTask)
    {
        //�߳���
        EnterCriticalSection(&m_Criti);  //�Ƕ��߳�������ȷ��ͬһʱ��ֻ��һ���̲߳��� �����������ݵĲ�������
        m_queTasks.push(pTask);
        LeaveCriticalSection(&m_Criti);
        ReleaseSemaphore(m_hSemphore, 1, NULL);
    }
private:
    //�߳�---�жϲ�ִ������
    static DWORD WINAPI ThreadFunc(LPVOID param) {
        CThreadPool* pThis = (CThreadPool*)param;
        while (pThis->m_isRunning) {
            WaitForSingleObject(pThis->m_hSemphore, -1);
            // ȡ������ִ��
            EnterCriticalSection(&pThis->m_Criti);
            if (pThis->m_queTasks.size() > 0) {
                ITask* pTask = pThis->m_queTasks.front();
                pThis->m_queTasks.pop();
                pTask->ExceuteTask();
                delete pTask;
            }
            LeaveCriticalSection(&pThis->m_Criti);
        }
        return 0;
    }
private:
    int m_nThreadCount = 0;                             //�߳�����
    HANDLE* m_hThreadArr = NULL;                        //�߳̾������
    BOOL m_isRunning = FALSE;                           //�Ƿ�������
    HANDLE m_hSemphore = NULL;                          //�ź���
    queue<ITask*>m_queTasks;                            //�������
    CRITICAL_SECTION m_Criti = {};                      //������Դ����leave֮ǰ���������������Դ�ᱻ����
};