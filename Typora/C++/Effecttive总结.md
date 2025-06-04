### 条款5

编译期可以暗自为class创建default构造函数、copy构造函数、copy assignment操作符以及析构函数。

如果类中有引用对象比如string& name或者const对象，必须要有自定义的构造函数，因为引用对象和const对象必须在初始化列表中赋初值，并且需要自定义拷贝赋值操作符（即copy assignment），因为引用对象和const对象在默认生成的拷贝赋值操作符的操作是不明确的，引用对象本来就是别名，一旦初始化指向某个对象就会一直指向它直到销毁，其他赋值操作都只是修改了引用的值，但不会改变指向，const对象更是不能直接修改值。

~~~c++
// gpt总结：
自定义构造函数：对于包含引用或 const 成员的类，你必须提供自定义构造函数，以便在初始化列表中正确初始化这些成员。

自定义拷贝赋值运算符：由于引用成员不能重新绑定，const 成员不能被修改，默认的拷贝赋值运算符无法处理这些情况。因此，你需要自定义拷贝赋值运算符来确保正确的行为。
~~~

### 条款6

为驳回编译器自动提供的机能(比如生成默认构造、拷贝构造等函数)，有三种方式避免

~~~c++
1.将构造函数和拷贝构造函数自己声明，不进行定义，放到private中，就算友元类或者自己的其他方法调用到了也会提示错误。
2.继承一个第一点说到的这样一个类，因为子类初始化对象时会先初始化基类。
3.现代c++都支持使用= delete来显式删除某些方法。
~~~

### 条款7

~~~c++
1.带多态性质的类都应该声明一个virtual析构函数，如果这个类拥有任何virtual函数，它就应该拥有一个virtual函数，这样delete基类对象时候，如果指向的对象是派生类，就能把派生类的析构函数一并调用了
2.如果不是作为基类使用，就不要写virtual，因为会有额外的消耗(虚指针和虚表)
~~~

### 条款8

~~~c++
case:如果一个vector中包含10个自定义类对象，第一个对象就在析构函数过程抛异常了，后边九个的内存就没能释放，此时会有两种可能，程序结束或不明确行为继续执行(有别的异常捕获机制，后9个内存泄漏)。
1.析构函数绝对不要吐出异常，如果某个被析构函数调用的函数可能抛出异常，析构函数应该捕捉任何异常，可以选择吞下异常(不传播)或直接结束程序。
2.如果客户需要对某个操作函数运行期间抛出的异常作出反应，那么class应该提供一个普通函数，而非在析构函数中执行，比如释放数据库的连接。
~~~

### 条款9

~~~c++
case:比如在基类的构造函数中调用了virtual函数，当创建子类对象时，会先调用基类的构造函数，但基类构造函数中的这个virtual函数会调用基类自己的，因为此时子类的对象都还没构造出来呢，若是个纯虚函数大概率不会写定义，则会报错，如果需要在基类中得到派生类的某些信息，可以在子类构造函数中给基类传承，如下
class Transaction {
  public:
  Transaction(const std::string& info);
}
class BuyTransaction: public Transaction {
  public:
  BuyTransaction(params):Transaction(params);
}
析构函数也是一样的道理，子类先完成析构，基类析构时如果有virtual函数同样也是基类自己的，因为子类的变量都变成未定义的了，虚表那些也没了。
1.在构造和析构期间不要调用virtual函数，因为这类调用从不下降至derived class
~~~

### 条款10

~~~c++
1.令赋值(assignment)操作符返回一个reference to *this
class Widget {
public:
  Widget& operator=(const Widget& rhs)
  {
    return *this;
  }
}
~~~

### 条款11

~~~c++
1.确保当对象自我赋值operator=有良好的行为。其中技术包括
1.1.比较"来源对象"和"目标对象"的地址
Widget& Widget::operator=(const Widge& rhs)
{
  if (this == &rhs) return *this;
  delete pb;
  pb = new Bitmap(*rhs.pb); // 如果不比较这里就是取空指针对象了
  return *this
}
1.2.精心周到的语句顺序
Widget& Widget::operator=(const Widget& rhs)
{
  Bitmap* pOrig = rhs.pb;
  pb = new Bitmap(*rhs.pb);
  delete pOrig; 	// 对于1.1如果new Bitmap抛异常了，那pb指向的是空，而1.2保障了就算Bitmap异常，pb还是指向之前的地址，并且省去自证的过程也没问题。
  return *this;
}
1.3.copy-and-swap（就是1.2的变种）
class Widget{
void swap(Widget& rhs); // 交换*this和rhs的数据
}
Widget& widget::operator=(const Widge& rhs)
{
  Widget tmp(rhs);
  swap(tmp);
  return this;
}
//拓展：拷贝赋值传参可以用by value形式，但牺牲了清晰性，有可能生成更高效的代码
Widget& widget::operator=(Widge rhs) // 传参过程就完成了拷贝
{
  swap(rhs);
 	return rhs;
}

2.确定任何函数如果操作一个以上的对象，而其中多个对戏那个是同一个对象时，其行为仍然正确。
~~~

### 条款12

~~~c++
1.copying函数(拷贝构造、拷贝赋值)应该确保复制“对象内的所有成员变量”及“所有base class成分”。
2.不要尝试以某个copying函数实现另一个copying函数，应该将共同机能放进第三个函数中，并由两个copying函数共同调用。
~~~

### 条款13

~~~shell
RAII(Resource Acquisition Is Initialization，资源创建的时机便是初始化时机)，也就是智能指针。
1.为防止资源泄漏，请使用RAII对象，它们在构造函数中获得资源并在析构函数中释放资源。
2.两个常被使用的RAII classes分别是tr1::shared_ptr(存在循环引用问题)和auto_ptr。前者通常是较佳选择，因为其copy行为比较直观，若选择auto_ptr，复制动作会使被复制物指向null。
~~~

### 条款14

~~~shell
1.复制RAII对象必须一并复制它所管理的资源，也就是深拷贝(这里说的RAII对象一般是自定义的，核心目标就是不再需要某个复件时确保资源被正确释放)，资源的coping行为决定RAII对象的coping行为，普遍而常见的RAII class coping行为是：1.抑制coping(比如析构函数执行的操作并不是释放堆内容，而是解锁操作)、2.施行引用计数法(在自定义的RAII class中增加一个share_ptr，构造函数传参初始化列表时赋值为share_ptr)。

知识点:
share_ptr有第二个参数，提供一个删除器执行自定义操作，比如释放锁，而不是默认的执行delete操作，auto_ptr不支持第二参数，auto_ptr确保只有一个RAII对象指向资源，并且释放时
~~~

### 条款15

~~~c++
1.APIs往往要求访问原始资源(raw resources)，所以每一个RAII class应该提供一个"取得其所管理的资源"的办法，比如share_ptr和auto_ptr的.get()方法。
2.对原始资源的访问可能经由显示转换或隐式转换，一般而言显式转换比较安全，但隐式转换对客户比较方便。
显示转换获取资源:
FontHandle get() const {return resoure;}
隐式转换换取资源（本质是重载）:
class Font{
public:
operator FontHandle() const
{return resource;}
} 
Font a;
void changeFont(FontHandle f); //可以直接传递a这个对象，会隐式转换。
~~~

### 条款16

~~~c++
1.如果在new中使用[]，必须在相应的delete表达式中也使用[]，如果在new中不使用[]，一定不要在delete表达式中使用[]。
// 要当心使用别名的情况
typedef string AddressLines[4];// 定义了一个名为AddressLines的新类型，实际上是一个包含4个string对象的数组。
string* pal = new AddressLines;
delete [] pal; // 正确形式
delete pal;  // 行为未定义
~~~

### 条款17

~~~c++
1.以独立语句将newed对象存储于(置入)只能指针内，如果不这样做，一旦异常被抛出，有可能导致难以察觉的资源泄漏
// 举例：
int priority();
void processWidget(shared_ptr<Widget> pw, int priority)
processWidget(shared_ptr<Widget>(new Widget), priority());// 如果是这种写法，因为在一个语句中，操作序列可能(不确定，但又可能因此得到更高效的代码，没人知道)是1.执行new Widget，2.调用priority()，3.调用shared_ptr构造函数，如果在第二步抛异常，那么就资源泄漏了。
// 正确写法
shared_ptr<Widget> pw(new Widget);//因为位于不同语句内，编译器不得在它们之间任意选择执行次序
processWidget(pw, priority());  

// 注意：
不能是processWidget(new Widget, priority());就好像是shared_ptr<Widget> pw = new Widget;这种写法编译不会通过，因为shared_ptr没有一个接受原始指针的隐式构造函数，但可以是shared_ptr<Widget> ptr = shared_ptr<Widget>(new Widget(args));这样相当于是初始化而不是赋值，不过现代C++中都用hared_ptr<Widget> ptr = make_shared<Widget>(args);
~~~

