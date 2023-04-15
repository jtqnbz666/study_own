#include <iostream>
#include <functional>
#include <chrono>
#include <memory>
#include <sys/epoll.h>
#include <set>
using namespace std;
using namespace std::chrono;
class BaseNode {
public:
    time_t expire;
    int64_t id;
};

struct TimerNode :  BaseNode{
    using Callback = function<void (const BaseNode& node)>;
    TimerNode(time_t expire, int64_t id, Callback func) : func(func) {
        this->expire = expire;
        this->id = id;
    }
    Callback func;
};

bool operator< (const BaseNode& lhd, const BaseNode& rhd) {
    if(lhd.expire == rhd.expire) {
        return lhd.id < rhd.id;
    }
    return lhd.expire < rhd.expire;
}

class CTimer {
public:
static time_t GetTick() {
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}
BaseNode AddTimer(time_t msc, TimerNode::Callback func) {
    time_t expire = GetTick() + msc; 
    auto ele = timer.emplace(expire, GetId(), func);
    return static_cast<BaseNode>(*ele.first); //返回的是pair对象(第一个参数是地址，第二个是bool);
}

bool DelTimer(BaseNode& node) {
    auto iter = timer.find(node);
    if(iter != timer.end()) {
        timer.erase(iter);
        return true;
    }
    return false;
}

bool CheckTimer() {
    auto iter = timer.begin();
    if(iter != timer.end() && iter->expire <= GetTick()) {
        iter->func(*iter);
        //回调函数的参数哪里找？
    //1.对于定时器： 在定时器节点上加上一个void* userdata, 通过这个对象来获取需要传入的参数
    //2.对于一般任务： 将任务封装成一个类对象， 类中的成员包含具体回调函数，以及所需参数
        
        timer.erase(iter);
        return true;
    }
    return false;
}

time_t TimeToSleep() {
    auto iter = timer.begin();
    if(iter == timer.end()) return -1;
    time_t dis = iter->expire - GetTick();
    return dis >= 0? dis : 0;
}
private:

static int64_t GetId() {
    return gid ++;
}
set<TimerNode, less<>>timer;
static int64_t gid;
};

int64_t CTimer::gid = 0;


int main() {
unique_ptr<CTimer>timer = make_unique<CTimer>();
int epfd = epoll_create(1);

epoll_event ev[64] = {0};
int i = 1000;
timer->AddTimer(1000, [](const BaseNode& node) {
    cout << CTimer::GetTick() << '\t' << node.id << endl;
});
BaseNode t2 = timer->AddTimer(2100, [](const BaseNode& node) {
    cout << CTimer::GetTick() << '\t' << node.id << endl;
});
    
timer->DelTimer(t2);
while(1) {
    int n = epoll_wait(epfd, ev, 64, timer->TimeToSleep());
    for(int i = 0; i < n; i ++) {
        //..
    }
    while(timer->CheckTimer());

}



return 0;
}
