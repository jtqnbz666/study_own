#include <iostream>
#include <functional>
#include <chrono>
#include <memory>
#include <set>
#include <sys/epoll.h>

using namespace std;
using namespace std::chrono;

struct NodeBase {
    time_t expire;  // 过期时间
    int64_t id;     // 全局唯一id
};

struct TimerNode : NodeBase {

    // C++ 11特性，使用函数对象。降低拷贝消耗，提高效率
    using Callback = std::function<void (const TimerNode &node)>;
    Callback func;
    int repeat;
    TimerNode(time_t exp, int64_t id, Callback func) : func(func) {
        this->expire = exp;
        this->id = id;
    }
};

// 引用依然有 
bool operator < (const NodeBase &lhd, const NodeBase &rhd) {
    if (lhd.expire < rhd.expire)
        return true;
    else if (lhd.expire > rhd.expire)
        return false;
    return lhd.id < rhd.id;
}

class CTimer {
public:
//system_clock::now()                -> 从系统获取的时钟；
//steady_clock::now()                 -> 不能被修改的时钟；就是系统开机的时间， 就是ubuntu开机运行的时间，不会变
//high_resolution_clock::now()   -> 高精度时钟，实际上是system_clock或者steady_clock的别名。
///*
//	duration_cast:  chrono的一个模板函数，不属于duration类
//	浮点时长和整数时长之间的转换，可以直接隐式转换，
//	其他情况都需要使用这个函数。
//
//	time_point_cast  :chrono的一个模板函数， 不属于time_point
//	不同的时间点的内部周期(ratio< , >)的转换，不同周期时间点，
//	如果存在精度丢失，不能用隐式转换，必须要采用time_point_cast做类型转换，
//	反之如果不存在精度丢失，可以直接隐式转换
//*/
    static time_t GetTick() {  //
        auto sc = time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
        auto tmp = chrono::duration_cast<chrono::milliseconds>(sc.time_since_epoch());//duration 持续时间  ，， epoch时代
        return tmp.count();

        //上边3句等价 = auto t1= duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();

        //计算当前时间距离1970年1月一日有多少小时， 注意这里用的system_clock::now();
        //cout<< duration_cast<chrono::hours>(system_clock::now().time_since_epoch()).count()<<endl;
    }
    NodeBase AddTimer(time_t msec, TimerNode::Callback func) {
        time_t expire = GetTick()+msec;
        auto ele = timer.emplace(expire, GenID(), func);
        return static_cast<NodeBase>(*ele.first);
    }
    bool DelTimer(NodeBase &node) {
        auto iter = timer.find(node); // 能不能通过 nodebase 找到 timernode？ C++14 才可以 可以通过等价key
        if (iter != timer.end()) {
            timer.erase(iter);
            return true;
        }
        return false;
    }
    bool CheckTimer() {
        auto iter = timer.begin();
        if (iter != timer.end() && iter->expire <= GetTick()) {
            iter->func(*iter);
            timer.erase(iter);
            return true;
        }
        return false;
    }
    time_t TimeToSleep() {
        auto iter = timer.begin();
        if (iter == timer.end()) return -1;
        time_t diss = iter->expire - GetTick();
        return diss > 0 ? diss : 0;
    }
private:
    static int64_t GenID() {
        return gid++;
    }
    set<TimerNode, std::less<>> timer;
    static int64_t gid;
};

int64_t CTimer::gid = 0;

int main () {

    int epfd = epoll_create(1);

    epoll_event ev[64] = {0};

    unique_ptr<CTimer> timer = make_unique<CTimer>();
    int i = 1000;
    timer->AddTimer(1000, [&](const TimerNode &node) {
        cout << CTimer::GetTick() << " node id:" << node.id << " i=" << i++ << endl; 
    });
    timer->AddTimer(1000, [&](const TimerNode &node) {
        cout << CTimer::GetTick() << " node id:" << node.id << " i=" << i++ << endl; 
    });
    timer->AddTimer(3000, [&](const TimerNode &node) {
        cout << CTimer::GetTick() << " node id:" << node.id << " i=" << i++ << endl; 
    });

    auto node = timer->AddTimer(2100, [&](const TimerNode &node) {
        cout << CTimer::GetTick() << " node id:" << node.id << " i=" << i++ << endl; 
    });

   // timer->DelTimer(node);

    while (true) {
        // timeout  要阻塞等待多长时间
        // -1 永久阻塞
        // 0 如果网络事件触发，立刻返回
        // > 0  比如 1000  最多阻塞等待 1s   如果没有网络事件
        // 通常传递 最近触发的定时任务  过期事件组织（有序的结构）
        // select poll
        int n = epoll_wait(epfd, ev, 64, timer->TimeToSleep());
        for (int i=0; i<n; i++) {
            // 处理网络事件
        }
        // cout << "handle timer event" << endl;
        // 处理定时事件
        while (timer->CheckTimer());
    }
    
    return 0;
}
