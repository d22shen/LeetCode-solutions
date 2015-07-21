
 struct ListNode {
     int val;
     ListNode *next;
     ListNode(int x) : val(x), next(nullptr) {}
 };

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