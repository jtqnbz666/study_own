#include <iostream>
#include <sys/epoll.h>

using namespace std;
using namespace std::chrono;

struct NodeBase {
	time_t expire ;
	int64_t id;
}

struct TimerNode {
	time_t expire; // 过期时间
	int64_t id;	   // 全局唯一的id
	using callback = std::function<void (const TimerNode &node)>;
	callback func; //函数拷贝代价高，应该避免
	TimerNode(time_t exp, int64_t id, callback func) : func(func) {
		this->expire = exp;
		this->id = id;
	}
};

//引用依然有
bool operator < (const NodeBase& lhd, const NodeBase &rhd) {
	if(lhd.expire < rhd.expire)
		return true;
	else if (lhd.expire > rhd.expire)
		return false;
	return lhd.id < rhd.id;
}

class CTimer{
public:
	static time_t GetTick() {
		auto sc = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
		auto tmp = chrono::duration_cast<chrono::milliseconds>(sc.time_since_epoch());
		return tmp.count();
	}
	NodeBase AddTimer(time_t msec, TimerNode::callback func) {
		time_t expire = GetTick() + msec;
		auto ele = timer.emplace(expire, GenID(), func);//避免拷贝,timer.insert()??
		return *ele.first;
}
	bool DelTimer(NodeBase& node) {
		auto iter = timer.find(node);//涉及拷贝问题，能不能通过nodebase找到timernode？c++14才可以
		if(iter != timer.end()) {
			timer.erase(iter);
			return true;
		}
		return false;
	}
	bool CheckTimer(){
		auto iter = timer.begin();
		if(iter != timer.end() && iter->expire <= GetTick()) {
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
	static int64_t GenID() {
		return gid++;
	}
	set<TimerNode, std::less<>> timer;
	static int64_t gid;
}；
int64_t CTimer::gid = 0;
int main() {
	int epfd = epoll_create(1);
	
	epoll_event ev[64] = {0};

	unique_ptr<CTimer> timer = make_unique<CTimer>();
	int i = 0;
	timer->AddTimer(1000, [&](const TimerNode& node) {
		cout << CTimer::GetTick() << "node id : " << node.id << "i =" << i ++ <<endl;
	})

	while(true) {
		int n = epoll_wait(epfd, ev. 64, timer->TimeToSleep());
		for(int i = 0; i < n; i ++ ) {
			 
		}

		while(timer->CheckTimer());
	}
}
