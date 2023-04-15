#include <iostream>
#include <functional>
#include <memory>
#include <set>
#include <chrono>
#include <sys/epoll.h>

struct BaseNode {
	time_t expire;
	int64_t id;
};

struct TimerNode : public BaseNode {
	using Callback = function<void (const TimerNode& node)>;
	Callback func;
	TimerNode(time_t expire, int64_t id, Callback func) : func(func) {
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

BaseNode AddTimer(time_t sec, int64_t id, TimerNode::Callback func) {
	time_t expire = sec + GetTick();
	auto ele = timer.emplace(expire, GetID, func);
	return static_cast<BaseNode>(*ele.first);
}

bool DelTimer(const BaseNode& node) {
	auto iter = timer.find(node);
	}if(iter != timer.end) {
		timer.erase(iter);
		return true;
	}
	return false;
};

bool CheckTimer() {
	auto iter = timer.begin();

class CTimer {
static time_t GetTick() {
	return duration_cast<milliseconds>(steady_clock::now().epoch()).count();
}



private:
	static int64_t GetID() {
		return ++gid;
	}
	staticq int64_t gid;
	set<TimerNode, less<>> timer;
};

int64_t TimerNode::gid = 0;




int main() {
	unique_ptr<CTimer>timer = make_unique<CTimer>();
	
	int epfd = epoll_create(1);
	epoll_event[64] = {0};
	
	while(1) {
		int n = epoll_wait(epfd, ev, 64, timer->TimeToSleep());
		for(int i = 0; i < n; i ++) {
			//
		}
		while(timer->CheckTimer());
	}
	return 0;
}
