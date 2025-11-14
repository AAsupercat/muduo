#include <cstdint>
#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include <unistd.h>

using TaskFunc = std::function<void()>;
using ReleaseFunc = std::function<void()>;

class TimerTask
{
private:
    uint64_t _id;         // 定义任务ID
    uint32_t _timeout;    // 定义超时时间
    bool _canceled;       // 定义是否取消任务
    TaskFunc _task_cb;    // 定义时间到后的执行函数
    ReleaseFunc _release; // 清除时间轮中的数据
public:
    // 初始化
    TimerTask(uint64_t id, uint32_t delay, const TaskFunc &cb) : _id(id), _timeout(delay), _canceled(false), _task_cb(cb) {}
    ~TimerTask()
    {
        if (_canceled == false)
            _task_cb();
    }
    void Cancel() { _canceled = true; }
    void SetRelease(const ReleaseFunc &cb) { _release = cb; }
    uint32_t DelayTime() { return _timeout; }
};
// 一个动态的二维数组，里面存储shared_ptr，采用引用计数的方式，延时；
// 一个哈希表，用来查找和创建延时指针对象，
class TimerWheel
{
private:
    using PtrTask = std::shared_ptr<TimerTask>;
    using WeakTask = std::weak_ptr<TimerTask>;
    int _tick;     // 滴答指针
    int _capacity; // 最长延续时间
    std::vector<std::vector<PtrTask>> _wheel;
    std::unordered_map<uint64_t, WeakTask> _timers;

private:
    void ReMove(uint64_t id)
    {
        auto it = _timers.find(id);
        if (it != _timers.end())
        {
            _timers.erase(id);
        }
    }

public:
    TimerWheel() : _tick(0), _capacity(60), _wheel(_capacity) {}
    void TimerAdd(uint64_t id, uint32_t delay, const TaskFunc &cb)
    {
        // 先根据所需要求构建shared_ptr指针
        PtrTask pt(new TimerTask(id, delay, cb));
        pt->SetRelease(std::bind(&TimerWheel::ReMove, this, id));
        int pos = (_tick + delay) % _capacity;
        _wheel[pos].push_back(pt);
        _timers[id] = WeakTask(pt);
    }

    // 任务的刷新
    void TimerRefresh(uint64_t id)
    {
        // 怎么叫刷新，重新添加一个share_ptr到时间轮里，增加引用计数；
        auto it = _timers.find(id);
        if (it == _timers.end())
            return;
        PtrTask pt = it->second.lock();
        int delay = pt->DelayTime();
        int pos = delay + _tick;
        _wheel[pos].push_back(pt);
        _timers[id] = WeakTask(pt);
    }

    void TimerCancel(uint64_t id)
    {
        auto it = _timers.find(id);
        if (it == _timers.end())
        {
            return; // 没找着定时任务，没法刷新，没法延迟
        }
        PtrTask pt = it->second.lock();
        if (pt)
            pt->Cancel();
    }

    // 这个函数应该每秒钟被执行一次，相当于秒针向后走了一步
    void RunTimerTask()
    {
        _tick = (_tick + 1) % _capacity;
        _wheel[_tick].clear(); // 清空指定位置的数组，就会把数组中保存的所有管理定时器对象的shared_ptr释放掉
    }
};


class Test {
    public:
        Test() {std::cout << "构造" << std::endl;}
        ~Test() {std::cout << "析构" << std::endl;}
};

void DelTest( Test *t) {
    delete t;
}

int main()
{
    TimerWheel tw;

    Test *t = new Test();

    tw.TimerAdd(888, 5, std::bind(DelTest, t));

    for(int i = 0; i < 5; i++) {
        sleep(1);
        tw.TimerRefresh(888);//刷新定时任务
        tw.RunTimerTask();//向后移动秒针
        std::cout << "刷新了一下定时任务，重新需要5s中后才会销毁\n";
    }
    //tw.TimerCancel(888);
    while(1) {
        sleep(1);
        std::cout << "-------------------\n";
        tw.RunTimerTask();//向后移动秒针
    }
    return 0;
}