//单例非线程安全
#ifndef BASE_SINGLETON_H_
#define BASE_SINGLETON_H_

template<typename T>
class Singleton {
public:
    static T& Instance() {
        if(s_instance == nullptr) {
            s_instance = CreateInstance();
        } 
        return *s_instance;
    }

    static T* GetInstance() {
        if(s_instance == nullptr) {
            s_instance = CreateInstance();
        }
    }

    static void Destroy() {
        if(s_instance) {
            DestroyInstance(s_instance);
            s_instance = nullptr;
        }
    }
protected:
    Singleton() {
       
    }
    ~Singleton() {
        if(s_instance) delete s_instance;
        s_instance = nullptr;
    }

private:
    static T* CreateInstance() {
        return new T();     //所以新的类得把singleton类作为友元类，不然无法构造对象
    }
    
    static void DestroyInstance(T* p) {
        delete p;
    }

    static T* s_instance;

    explicit Singleton(const Singleton& ) {}
    Singleton& operator= (const Singleton& ) {
        return *this;
    }
};

//在头文件初始化，则这个头文件只能被包含一次，这里使用了预处理判断可以忽略这句话
//因为这里相当于定义.
template<typename T>
T* Singleton<T>::s_instance = nullptr;
#endif