#ifndef __OBJECTPOOL__H__
#define __OBJECTPOOL__H__
#include <iostream>
using namespace std;
#include <vector>
#include <memory>

//类模板
template <typename T>
class objectPool
{
public:
    objectPool() = default;
    ~objectPool() {m_pool.clear();}
    //得到对象
    //可变参数模板
    template <typename... Args>
    std::shared_ptr<T> getObj(arg &&... args)
    {
        std::lock_guard<mutex> lock(m_mutex);
        if(!m_pool.empty())
        {
            auto obj = m_pool.back();
            m_pool.pop_back();
            return obj;
        }
        else
        {
            //创建
            //完美转发
            return make_shared<T>(std::forward<Args>)(args)...);
        }
            
    }
    //资源回收
    void release(std::shared_ptr<T> obj)
    {
        std::lock_guard<mutex> lock(m_mutex);
        m_pool.push_back(obj);
    }
private:
    std::vector<std:shared_ptr<T>> m_pool;
    std::mutex m_mutex;

};



#endif // __OBJECTPOOL__H__