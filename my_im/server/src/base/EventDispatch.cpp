#include "EventDispatch.h"
#include "BaseSocket.h"

#define MIN_TIMER_DURATION  100 

CEventDispatch* CEventDispatch::m_pEventDispatch = NULL;

CEventDispatch::CEventDispatch() {
    running = false;
    m_epfd = epoll_create(1024);
    if(m_epfd == -1) {
        log_error("epoll_create failed");
    }
}

CEventDispatch::~CEventDispatch() {
    close(m_epfd);
}

void CEventDispatch::AddTimer(callback_t callback, void* user_data, uint64_t interval) {
    list<TimerItem*>::iterator it;
    for(it = m_timer_list.begin(); it != m_timer_list.end(); it ++) {
        TimerItem* pItem = *it;
        if(pItem->callback == callback && pItem->user_data == user_data) {
            pItem->interval = interval;
            pItem->next_tick = get_tick_count() + interval;
            return ;
        }
    }

    TimerItem* pItem = new TimerItem;
    pItem->callback = callback;
    pItem->user_data = user_data;
    pItem->interval = interval;
    pItem->next_tick = get_tick_count() + interval;
    m_timer_list.push_back(pItem);
}

//由此可见对于链表的定时器的删除操作是通过比对callback, user_data
void CEventDispatch::RemoveTimer(callback_t callback, void* user_data) {
    list<TimerItem*>::iterator it;
    for(it = m_timer_list.begin(); it != m_timer_list.end(); it ++) {
        TimerItem* pItem = *it;
        if(pItem->callback == callback && pItem->user_data == user_data) {
            m_timer_list.erase(it);
            delete pItem;
            return;
        }
    }
}


void CEventDispatch::_CheckTimer() {
    uint64_t curr_tick = get_tick_count();
    list<TimerItem*>::iterator it;
    for(it = m_timer_list.begin(); it != m_timer_list.end();) {
        TimerItem* pItem = *it;
        it ++;
        if(curr_tick >= pItem->next_tick) {
            //这里就是为什么需要interval字段的原因，因为是循环的  
            pItem->next_tick += pItem->interval;
            pItem->callback(pItem->user_data, NETLIB_MSG_TIMER, 0, NULL);
        }
    }
}

void CEventDispatch::AddLoop(callback_t callback, void* user_data) {
    TimerItem* pItem = new TimerItem;
    pItem->callback = callback;
    pItem->user_data = user_data;
    m_loop_list.push_back(pItem);
}

void CEventDispatch::_CheckLoop() {
    for(list<TimerItem*>::iterator it = m_loop_list.begin(); it != m_loop_list.end(); it ++) {
        TimerItem* pItem = *it;
        pItem->callback(pItem->user_data, NETLIB_MSG_LOOP, 0, NULL);
    }
}

CEventDispatch* CEventDispatch::Instance() {
    if(m_pEventDispatch == NULL) {
        m_pEventDispatch = new CEventDispatch();
    }
    return m_pEventDispatch;
}

void CEventDispatch::AddEvent(SOCKET fd, uint8_t socket_event) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLPRI | EPOLLERR | EPOLLHUP;
    ev.data.fd = fd;
    if(epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) != 0) {
        log_error("epoll_ctl() failed, errno = %d", errno);
    }
}

void CEventDispatch::RemoveEvent(SOCKET fd, uint8_t socket_event) {
    if(epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, NULL) != 0) {
        log_error("epoll_ctl failed, errno = %d", errno);
    }
}

void CEventDispatch::StartDispatch(uint32_t wait_timeout) {
    struct epoll_event events[1024];
    int nfds = 0;
    if(running) return;
    running = true;
    while(running) {
        nfds = epoll_wait(m_epfd, events, 1024, wait_timeout);
        for(int i = 0; i < nfds; i ++) {
            int ev_fd = events[i].data.fd;
            CBaseSocket* pSocket = FindBaseSocket(ev_fd);
            if(!pSocket) continue;
            #ifdef EPOLLRDHUP
            if(events[i].events & EPOLLRDHUP) {
                pSocket->OnClose();
            }
            #endif 
            if(events[i].events & EPOLLIN) {
                pSocket->OnRead();
            }
            if(events[i].events & EPOLLOUT) {
                pSocket->OnWrite();
            }
            if(events[i].events & (EPOLLPRI | EPOLLERR | EPOLLHUP)) {
                pSocket->OnClose();
            }
            pSocket->ReleaseRef();
        }
        _CheckTimer();
        _CheckLoop();
    }
}

void CEventDispatch::StopDispatch() {
    running = false;
}

