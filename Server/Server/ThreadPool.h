#pragma once
#include <Windows.h>
#include <stdio.h>
#include <queue>
#include <mutex>
using namespace std;
//任务接口----使用见数据格式文件夹
class ITask
{
public:
    virtual void ExceuteTask() = 0;//接口，执行任务
};

class CThreadPool
{
public:
    //创建线程池
    void CreateThreadPool(int nThreadCount/*自己设定线程数*/) {
        m_nThreadCount = nThreadCount;
        m_isRunning = TRUE;
        // 创建信号量
        m_hSemphore = CreateSemaphoreA(NULL, 0, MAXLONG, NULL);
        //创建临界区
        InitializeCriticalSection(&m_Criti);
        //创建线程
        m_hThreadArr = new HANDLE[m_nThreadCount]();
        for (int i = 0; i < m_nThreadCount; i++) {
            m_hThreadArr[i] = CreateThread(NULL, 0, ThreadFunc, this, 0, NULL);
        }
    }
    //关闭线程池
    void CloseThreadPool()
    {
        //通知所有线程退出
        m_isRunning = FALSE;
        for (int i = 0; i < m_nThreadCount; i++) {
            ReleaseSemaphore(m_hSemphore, 1, NULL);
        }
        //等待全局程退出
        WaitForMultipleObjects(m_nThreadCount, m_hThreadArr, TRUE, -1);
        //清理资源
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
    // 添加任务
    void AddTask(ITask* pTask)
    {
        //线程锁
        EnterCriticalSection(&m_Criti);  //是多线程中用来确保同一时刻只有一个线程操作 被保护的数据的操作函数
        m_queTasks.push(pTask);
        LeaveCriticalSection(&m_Criti);
        ReleaseSemaphore(m_hSemphore, 1, NULL);
    }
private:
    //线程---判断并执行任务
    static DWORD WINAPI ThreadFunc(LPVOID param) {
        CThreadPool* pThis = (CThreadPool*)param;
        while (pThis->m_isRunning) {
            WaitForSingleObject(pThis->m_hSemphore, -1);
            // 取出任务并执行
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
    int m_nThreadCount = 0;                             //线程数量
    HANDLE* m_hThreadArr = NULL;                        //线程句柄数组
    BOOL m_isRunning = FALSE;                           //是否在运行
    HANDLE m_hSemphore = NULL;                          //信号量
    queue<ITask*>m_queTasks;                            //任务队列
    CRITICAL_SECTION m_Criti = {};                      //保护资源，在leave之前如果有其他访问资源会被挂起
};