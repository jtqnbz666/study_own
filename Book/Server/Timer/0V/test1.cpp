#include <iostream>
#include <sys/epoll.h>
#include <set>
#include <memory>
#include <chrono>// c++时间库
#include <functional>

using namespace std;
using namespace std::chrono;

struct NodeBase{
	time_t expire;	//mutable 修饰词真好用
	int64_t id;
};

struct TimerNode : NodeBase{
	using Callback = std::function<void (const TimerNode& node)>;
	Callback func;
	int repeat;
	TimerNode(time_t expire, int64_t id, Callback func) : func(func) {
		this->expire = expire; 
		this->id = id;
	}
};
bool operator < (const NodeBase& lhd, const NodeBase& rhd) {
	if(lhd.expire < rhd.expire) {
		return true;
	} else if(lhd.expire > rhd.expire) {
		return false;
	}
	return lhd.id < rhd.id;
}
class CTimer {
public:
	static time_t GetTick() {
		return duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	}
	NodeBase AddTimer(time_t msec, TimerNode::Callback func) {
		time_t expire = GetTick() + msec;
		auto ele = timer.emplace(expire, GetID(), func);
		return static_cast<NodeBase>(*ele.first);
	}
	bool DelTimer(NodeBase &node) {
		auto iter = timer.find(node);
		if(iter != timer.end()) {
			timer.erase(iter);
			return true;
		}
		return false;
	}
	bool CheckTimer() {
		auto iter = timer.begin();
		if(iter != timer.end() && iter->expire < GetTick()) {
			iter->func(*iter);
			timer.erase(iter);
			return true;
		}
		return false;
	}
	time_t TimeToSleep() {
		auto iter = timer.begin();
		if(iter == timer.end()) return -1;
		time_t diss = iter->expire - GetTick();
		return diss > 0 ? diss : 0;
	}
private:
	static int64_t GetID() {
		return gid ++;
	}
	set<TimerNode, less<>> timer;
	static int64_t gid;
};
int64_t CTimer::gid = 0;

int main() {
	int epfd = epoll_create(1);
	
	epoll_event ev[64] = {0};
	unique_ptr<CTimer>timer = make_unique<CTimer>();
	int i = 1000;
	timer->AddTimer(1000, [&](const TimerNode& node){
		cout << CTimer::GetTick() << "node id:" << node.id << "i =" << i++ << endl;
	});
	
	timer->AddTimer(1000, [&](const TimerNode& node) {
		cout << CTimer::GetTick() << "node id:" << node.id << "i =" << i++ << endl;
	});
	timer->AddTimer(2100, [&](const TimerNode& node) {
		cout << CTimer::GetTick() << "node id:" << node.id << "i =" << i++ << endl;
	});
	while(1) {
		int ret = epoll_wait(epfd, ev, 64, timer->TimeToSleep());
		for(int i = 0; i < ret; i ++) {

		
		}
		while(timer->CheckTimer());
	}
	return 0;
}
