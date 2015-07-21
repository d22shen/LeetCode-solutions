//The structure of the list node
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

//my solution
//two helper functions: get the lengeth of a list and reverse a list
//separate the list into two halves
//reverse the second half
//compare the two half list
//if all the same return true
//else return false
//time complexity O(n) and space complexity O(1)
class Solution {
public:
    
    int getLength(ListNode* head){
        int c = 0;
        while(head!=nullptr){
            c++;
            head = head->next;
        }
        return c;
    }
    
    ListNode* reverseList(ListNode* head){
        ListNode *prev = nullptr;
        while(head!=nullptr){
            ListNode* temp = head->next;
            head->next = prev;
            prev = head;
            head = temp;
        }
        return prev;
    }
    
    bool isPalindrome(ListNode* head) {
        int len = getLength(head);
        int t = len/2;
        int c = 0;
        
        ListNode* firsthalf = head;
        ListNode* lasthalf = head;
        ListNode* temp = nullptr;
        
        if(len == 0||len == 1)
            return true;
        if(t*2 == len){
            while(c<len/2-1){
                firsthalf = firsthalf->next;
                c++;
            }
            lasthalf = firsthalf->next;
            firsthalf->next = nullptr;
            firsthalf = head;
        }else{
            while(c<len/2-1){
                firsthalf = firsthalf->next;
                c++;
            }
            temp = firsthalf->next;
            lasthalf = temp->next;
            firsthalf->next = nullptr;
            firsthalf = head;
        }
        ListNode* firsttemp = firsthalf;
        
        lasthalf = reverseList(lasthalf);
        
        while(firsttemp!=nullptr&&lasthalf!=nullptr){
            if(firsttemp->val != lasthalf->val)
                return false;
            firsttemp = firsttemp->next;
            lasthalf = lasthalf->next;
        }
        
        //reverse the previously reversed second half
        //add the two halves together so that the list is not modified
        lasthalf = reverseList(lasthalf);
        
        if(temp == nullptr){
            firsthalf->next = lasthalf;
        }else{
            firsthalf->next = temp;
            temp->next = lasthalf;
        }
        
        return true;
    }
};

//Reverse the first half and compare it with the second half
//solution online
//http://blog.csdn.net/abc123man/article/details/46835287
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if(!head || !head->next)//null or only 1 node
        {
            return true;
        }
        
        ListNode *pre = NULL, *ne = NULL;//pre指向已经反转好的头节点
        ListNode *phalf = head, *p = head;
        bool ovenNode = false;//判断节点数是偶数还是奇数
        while(p->next) //找出中间节点，并将前一半节点翻转
        {
            ne = phalf;
            phalf = phalf->next;
            p = p->next;
            if(p->next)//p移动两次，phalf移动一次
            {
                p = p->next;
            }
            else
            {
                ovenNode = true;
            }
            
            if(ne == head)//第一次不翻转
            {
                head->next = NULL;
                pre = head;
            }
            else
            {
                ne->next = pre; //翻转
                pre = ne;
            }
            
        }
        
        if(!ovenNode) //奇数个节点，跳出中间节点不比较
        {
            phalf = phalf->next;
        }
        
        while(phalf) //回文检测
        {
            if(pre->val != phalf->val)
            {
                return false;
            }
            phalf = phalf->next;
            pre = pre->next;
        }
        
        return true;
    }
};

//use a vector to store the value
//time complexity O(n) and space complexity O(n)
//solution online
//http://www.2cto.com/kf/201507/418110.html

class Solution {
public:
    bool isPalindrome(ListNode* head) {
        vector<int> temp;
        ListNode* ptr = head;
        while(ptr!=NULL)
        {
            temp.push_back(ptr->val);
            ptr = ptr->next;
        }
        int n = temp.size();
        for(int i = 0; i < n/2; i++)
        {
            if(temp[i] != temp[n-1-i])
                return false;
        }
        return true;
    }
};




















