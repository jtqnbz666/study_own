#include <iostream>
#include <sys/epoll.h>
#include <memory>
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
	using Callback = std::function<void (const TimerNode& node)>;
	Callback func;
	TimerNode(time_t expire, int64_t id, Callback func) : func(func) {
		this->expire = expire;
		this->id = id;
	}
};

/*bool operator < (const BaseNode& lhd, const BaseNode& rhd) {
	if(lhd.expire < rhd.expire) {
		return true;
	} else if (lhd.expire > rhd.expire) {
		return false;
	}
	return lhd.id < rhd.id;
}*/

class Timer {
public:
	static time_t GetTick() {
		return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
	}
	bool CheckTimer() {
		auto iter = timer.begin();
		if(iter != timer.end()) {
			if(iter->expire < GetTick()) {
				iter->func(*iter);
				timer.erase(iter);
				return true;
			}
		}
		return false;
	}
	BaseNode AddTimer(time_t sec, TimerNode::Callback func) {
		time_t expire = sec + GetTick();
		auto ele = timer.emplace(expire, GetID(), func);
		return static_cast<BaseNode>(*ele.first);	//因为emplace返回值虽然是pair,但first是元素的地址
	}
	bool DelTimer(BaseNode& node) {
		auto iter = timer.find(node); // 仅14标准及其之后
		if(iter != timer.end()) {
			timer.erase(iter);
			return true;
		}
		return false;
	}
	time_t TimeToSleep() {
		auto iter = timer.begin();
		if(iter == timer.end()) return -1;
		time_t dis = iter->expire - GetTick();
		return dis > 0 ? dis : 0;
	}
	
private:
	static int64_t GetID(){
		return id++;
	}
	set<TimerNode, less<>> timer;
	static int64_t id;
};
int64_t Timer::id = 0;

int main() {
	int epfd = epoll_create(1);
	epoll_event ev[64] = {0};
	unique_ptr<Timer>timer = make_unique<Timer>();

	timer->AddTimer(1000, [&](const TimerNode& node) {
		cout << Timer::GetTick() << "node_id = " << node.id << endl;
	});
	timer->AddTimer(1200, [&](const TimerNode& node) {
		cout << Timer::GetTick() << "node_id = " << node.id << endl;
	});
	while(true) {
		int ret =epoll_wait(epfd, ev, 64, timer->TimeToSleep());
		for(int i = 0; i < ret; i ++) {
		//..
		}
		while(timer->CheckTimer());
	}
	
	return 0;
}
