#include <iostream>
#include <memory>
#include <sys/epoll.h>
#include <functional>
#include <set>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct BaseNode {
    time_t expire;
    int64_t id;
};
struct TimerNode : BaseNode {
    using Callback = function<void(const TimerNode& node)>;
    Callback func;
    TimerNode(time_t expire, int64_t id, Callback func) : func(func) {
        this->expire = expire;
        this->id = id;
    }
};

bool operator < (const BaseNode&node1, const BaseNode& node2) {
    if(node1.expire == node2.expire) {
        return node1.id < node2.id;
    }
    return node1.expire < node2.expire;
}

class Timer {
public:
    static time_t GetTick() {
        return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
    }
    BaseNode AddTimer(time_t msc, TimerNode::Callback func) {
        time_t expire = msc + GetTick();
        auto ele = timer.emplace(expire, GetID(), func);
        return static_cast<BaseNode>(*ele.first);
    }
    bool DelTimer(BaseNode &node) {
        auto iter = timer.find(node);
        if(iter != timer.end()) {
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
       if(iter == timer.end()) return -1;
       time_t dis = iter->expire - GetTick();
       return dis >= 0 ? dis : 0;
    }
private: 
    static int64_t GetID() {
        return gid++;
    }
    static int64_t gid;
    set<TimerNode, less<>> timer;
};
    int64_t Timer::gid = 0;

int main() {
    int epfd = epoll_create(1);
    epoll_event ev[64] = {0};

    unique_ptr<Timer>timer = make_unique<Timer>();
    int i = 0;
    timer->AddTimer(1000, [](const TimerNode& node) {
        cout << Timer::GetTick()  << "node id = :" << node.id  << endl;
    });
    while(true) {
        int ret = epoll_wait(epfd, ev, 64, timer->TimeToSleep());  
        for(int i = 0; i < ret; i ++) {

        }

        while(timer->CheckTimer());
    }
    return 0;
}