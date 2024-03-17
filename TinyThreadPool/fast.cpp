#include<bits/stdc++.h>

class TinyThreadPool{
public:
    TinyThreadPool(int num_threads):stop(false){
        for(int i=1;i<=num_threads;i++){
            //开一个工作线程
            std::function<void()> worker = [=]()->void{
                //工作线程需要不断检查是不是有新任务，并且执行
                while(1){
                    std::function<void()> task;
                    std::unique_lock<std::mutex> lock(workers_mtx);
                    if(stop&&tasks_queue.empty()){
                        std::cout << "PID " << i << " finished.\n";
                        return;
                    } //如果线程池已经停止工作了，并且工作队列也是空的，那就直接结束
                    cv.wait(lock,[this]{
                        return !tasks_queue.empty();
                    }); //假如当前任务队列空，那就让工作线程先沉睡
                    
                    task = std::move(tasks_queue.front()); //从任务队列中取任务，用move右值转左值
                    tasks_queue.pop(); //任务队列弹出任务
                    std::cout << "PID " << i << " is working on :";
                    task(); //执行任务
                    std::cout << std::endl;
                    }
            };
            workers_queue.emplace_back(worker); //emplace避免拷贝构造
        }
    };
    template<class F>
    void push_back(F task){
        {
            std::unique_lock<std::mutex> lock(workers_mtx);  
            if(stop){
                throw std::runtime_error("Don't push new task into stopped thread pool.");
            } //如果线程池已经停止，拒绝一切新任务并抛出异常
            tasks_queue.emplace(task); //把任务塞进来
        }
        cv.notify_one(); //塞进一个任务，唤醒一个线程，很合理
    };
    ~TinyThreadPool(){
        {
            std::unique_lock<std::mutex> lock(workers_mtx); //确保所有工作线程都能看到停止标记
            stop = true; //设置停止标记
        }
        cv.notify_all(); //先唤醒所有线程
        for(auto &workers:workers_queue){
            workers.join(); //再释放线程资源
        }
    };    
private:
    bool stop; //停止标记，允许显式停止线程池的工作
    std::condition_variable cv; //用于线程间的通信
    std::mutex workers_mtx; //每一个任务只允许一个工作线程进入
    std::vector<std::thread> workers_queue; //工作线程队列
    std::queue<std::function<void()>> tasks_queue; //任务队列
};

int main()
{
    TinyThreadPool pool_1(10);
    for(int i=1;i<=1000;i++) {
        pool_1.push_back([=](){
            std::cout << "task " << i;
        });
    }
    return 0;
}