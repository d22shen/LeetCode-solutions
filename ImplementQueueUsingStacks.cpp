//my solution
class Queue {
public:
    // Push element x to the back of queue.
    void push(int x) {
        mystack.push(x);
    }
    
    // Removes the element from in front of queue.
    void pop(void) {
        stack<int> tempstack;
        
        int item = 0;
        
        while(!mystack.empty()){
            item = mystack.top();
            mystack.pop();
            tempstack.push(item);
        }
        
        tempstack.pop();
        
        while(!tempstack.empty()){
            int tmp = tempstack.top();
            tempstack.pop();
            mystack.push(tmp);
        }
    }
    
    // Get the front element.
    int peek(void) {
        
        stack<int> tempstack;
        
        int item = 0;
        
        while(!mystack.empty()){
            int tmp = mystack.top();
            mystack.pop();
            tempstack.push(tmp);
        }
        
        item = tempstack.top();
        
        while(!tempstack.empty()){
            int tmp = tempstack.top();
            tempstack.pop();
            mystack.push(tmp);
        }
        
        return item;
    }
    
    // Return whether the queue is empty.
    bool empty(void) {
        return mystack.empty();
    }
    
private:
    stack<int> mystack;
};

//online solution
//the test cases are created using our push function rather than existing stacks
//so when stroring the data, we inverse the sequence in the push function
//hence, we can use the pop and peek directly rather than do the double inversion process


class Queue {
public:
    // Push element x to the back of queue.
    void push(int x) {
        stack<int> tmp;
        while (!s.empty()) {
            tmp.push(s.top());
            s.pop();
        }
        s.push(x);
        while (!tmp.empty()) {
            s.push(tmp.top());
            tmp.pop();
        }
    }
    
    // Removes the element from in front of queue.
    void pop(void) {
        s.pop();
    }
    
    // Get the front element.
    int peek(void) {
        return s.top();
    }
    
    // Return whether the queue is empty.
    bool empty(void) {
        return s.empty();
    }
    
private:
    stack<int> s;
};

//use two queues
class Queue {
public:
    // Push element x to the back of queue.
    void push(int x) {
        stack<int> tmp;
        while (!s.empty()) {
            tmp.push(s.top());
            s.pop();
        }
        s.push(x);
        while (!tmp.empty()) {
            s.push(tmp.top());
            tmp.pop();
        }
    }
    
    // Removes the element from in front of queue.
    void pop(void) {
        s.pop();
    }
    
    // Get the front element.
    int peek(void) {
        return s.top();
    }
    
    // Return whether the queue is empty.
    bool empty(void) {
        return s.empty();
    }
    
private:
    stack<int> s;
};

























