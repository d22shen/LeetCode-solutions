//my solution
class Stack {
public:
    // Push element x onto stack.
    void push(int x) {
        queue<int> tmpqueue;
        while(!myqueue.empty()){
            int tmp = myqueue.front();
            myqueue.pop();
            tmpqueue.push(tmp);
        }
        //这个时候tmpqueue里面元素的顺序和原来myqueue里是一样的
        //但是myqueue里是空的，所以往myqueue里push新的x，再把其他的push进去
        myqueue.push(x);
        while(!tmpqueue.empty()){
            int tmp = tmpqueue.front();
            myqueue.push(tmp);
            tmpqueue.pop();
        }
    }
    
    // Removes the element on top of the stack.
    void pop() {
        myqueue.pop();
    }
    
    // Get the top element.
    int top() {
        return myqueue.front();
    }
    
    // Return whether the stack is empty.
    bool empty() {
        return myqueue.empty();
    }
private:
    queue<int> myqueue;
};

//use two queues
class Stack {
public:
    // Push element x onto stack.
    void push(int x) {
        q2.push(x);
        while (q2.size() > 1) {
            q1.push(q2.front());
            q2.pop();
        }
    }
    
    // Removes the element on top of the stack.
    void pop(void) {
        top();
        q2.pop();
    }
    
    // Get the top element.
    int top(void) {
        if (q2.empty()) {
            for (int i = 0; i < (int)q1.size() - 1; ++i) {
                q1.push(q1.front());
                q1.pop();
            }
            q2.push(q1.front());
            q1.pop();
        }
        return q2.front();
    }
    
    // Return whether the stack is empty.
    bool empty(void) {
        return q1.empty() && q2.empty();
    }
    
private:
    queue<int> q1, q2;
};