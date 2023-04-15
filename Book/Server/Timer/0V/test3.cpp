#include <iostream>
#include <sys/epoll.h>
#include <functional>
#include <chrono>
#include <set>
#include <memory>

using namespace std;
using namespace std::chrono;

struct BaseNode {
	time_t expire;
	int64_t id;
};

struct TimerNode : BaseNode {
	using Callback = function<void(const TimerNode& node)>;
	Callback func;
	TimerNode(int64_t expire, int64_t id, Callback func) : func(func) {
		this->expire = expire;
		this->id = id;
	}
};

bool operator < (const BaseNode& node1, const BaseNode& node2) {
	if(node1.expire == node2.expire) {
		return node1.id < node2.id;
	}
	return node1.expire < node2.expire;
}

class CTimer {
public:
	static int64_t GetTick() {
		return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
	}
	BaseNode AddTimer(int64_t sec, TimerNode::Callback func) {
		int64_t expire = sec + GetTick();
		auto ele = timer.emplace(expire, GetID(), func);
		return static_cast<BaseNode>(*ele.first);
	}
	bool DelTimer(const BaseNode& node) {
		auto iter = timer.find(node) ;
		if(iter != timer.end()) {
			timer.erase(iter);
			return true;
		}
		return false;
	}
	int64_t TimeToSleep() {
		auto iter = timer.begin();
		if(iter == timer.end()) return -1;
		int64_t dis = iter->expire - GetTick();
		return dis > 0 ? dis : 0;
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
	
private:
	static int64_t GetID() {
		return gid++;
	}
	static int64_t gid;
	set<TimerNode, less<>> timer;
};

int64_t CTimer::gid = 0;	

int main() {
	unique_ptr<CTimer>timer = make_unique<CTimer>();

	timer->AddTimer(3000, [](const TimerNode& node) {
		cout << CTimer::GetTick() << "id = " << node.id << endl;
	});
	timer->AddTimer(4000, [](const BaseNode& node) {
		cout << CTimer::GetTick() << "id = " << node.id << endl;
	});
	int epfd = epoll_create(1);
	epoll_event ev[64] = {0};
	while(1) {
		int ret = epoll_wait(epfd, ev, 64, timer->TimeToSleep());
		for(int i = 0; i < ret; i ++) {
		//...
		}
		while(timer->CheckTimer());
	}


	return 0;
}
