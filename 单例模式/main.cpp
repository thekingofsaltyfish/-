#include <iostream>
#include <mutex>

/*
以下用依次5种方式实现单例模式：
1.懒汉 线程不安全
2.懒汉 线程安全
3.饿汉 
4.双重检查锁定
5.C++ 11 饿汉

*/

//懒汉式（线程不安全）
class Singleton1{
public:
    static Singleton1* get(){
        if(instance==nullptr){
            instance = new Singleton1();
        }
        return instance; 
    }
private:
    static Singleton1* instance;
    Singleton1(){
        
    } 
};

//进行初始化
Singleton1* Singleton1::instance = nullptr;

//检查是否是单例的
void test1(){
    std::cout << "The first way:\n";
    Singleton1* instance1 = Singleton1::get();
    Singleton1* instance2 = Singleton1::get();
    std::cout << "The address of instance1 is :" << instance1 << "\n";
    std::cout << "The address of instance2 is :" << instance2 << "\n";
    if(instance1==instance2) {
        std::cout << "Same\n";
    }    
    else {
        std::cout << "Not same\n";
    }
    std::cout << "\n";
}

//懒汉式（线程安全）
class Singleton2{
public:
    static Singleton2* get(){
        std::unique_lock<std::mutex> lock(mtx);
        if(instance==nullptr){
            instance = new Singleton2();//确保只被实例化一次
        }
        return instance;
    }
private:
    static Singleton2* instance;//表示该类拥有的唯一的实例，此处只是声明
    static std::mutex mtx;//声明一个锁,如果每个线程使用一个锁，也不会起到线程安全的作用，因此要使用static
    Singleton2(){}
};

Singleton2* Singleton2::instance = nullptr;//类外初始化为nullptr
std::mutex Singleton2::mtx;//类外初始化，std::mutex的默认构造函数会提供mtx的初始化

void test2(){
    std::cout << "The second way:\n";
    Singleton2* instance1 = Singleton2::get();
    Singleton2* instance2 = Singleton2::get();
    std::cout << "The address of instance1 is :" << instance1 << "\n";
    std::cout << "The address of instance2 is :" << instance2 << "\n";
    if(instance1==instance2) {
        std::cout << "Same\n";
    }    
    else {
        std::cout << "Not same\n";
    }
    std::cout << "\n";
}

class Singleton3{
public:
    static Singleton3& get(){//直接返回引用
        return instance;
    }
private:
    static Singleton3 instance;
    Singleton3(){}
};
Singleton3 Singleton3::instance;//类外直接初始化，缺点是会增加开销，因为不管有没有用到这个方法，都需要初始化消耗资源

void test3(){
    std::cout << "The third way:\n";
    //这里是取了Singleton3的引用，因为直接创建会导致拷贝复制
    Singleton3 &instance1 = Singleton3::get();
    Singleton3 &instance2 = Singleton3::get();
    //static_cast在这里用于对instance1和instance2进行到void*的转换，void*可以指向任何类型的指针，因此可以打印出地址
    std::cout << "The address of instance1 is :" << static_cast<void*>(&instance1) << "\n";
    std::cout << "The address of instance2 is :" << static_cast<void*>(&instance2) << "\n";
    //比较地址
    if(&instance1==&instance2) {
        std::cout << "Same\n";
    }    
    else {
        std::cout << "Not same\n";
    }
    std::cout << "\n";
}

//双重检查锁定的懒汉式
class Singleton4{
public:
    static Singleton4* get(){
        if(instance==nullptr){
            std::unique_lock<std::mutex> lock(mtx);
            if(instance==nullptr){
                instance = new Singleton4();
            }
        }
        return instance;
    }
private:
    static Singleton4* instance;
    static std::mutex mtx;
    Singleton4(){}
};
Singleton4* Singleton4::instance = nullptr;
std::mutex Singleton4::mtx;

void test4(){
    std::cout << "The fourth way:\n";
    Singleton4* instance1 = Singleton4::get();
    Singleton4* instance2 = Singleton4::get();
    std::cout << "The address of instance1 is :" << instance1 << "\n";
    std::cout << "The address of instance2 is :" << instance2 << "\n";
    if(instance1==instance2) {
        std::cout << "Same\n";
    }    
    else {
        std::cout << "Not same\n";
    }
    std::cout << "\n";
}
//局部静态变量
class Singleton5{
public:
    static Singleton5& get(){
        static Singleton5 instance;
        return instance;
    }
private:
    Singleton5(){}
    Singleton5(const Singleton5&) = delete;
    Singleton5& operator= (const Singleton5&) = delete;
};

void test5(){
    std::cout << "The fifth way:\n";
    Singleton5& instance1 = Singleton5::get();
    Singleton5& instance2 = Singleton5::get();
    std::cout << "The address of instance1 is :" << static_cast<void*>(&instance1) << "\n";
    std::cout << "The address of instance2 is :" << static_cast<void*>(&instance2) << "\n";
    if(&instance1==&instance2) {
        std::cout << "Same\n";
    }    
    else {
        std::cout << "Not same\n";
    }
    std::cout << "\n";
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    return 0;
}