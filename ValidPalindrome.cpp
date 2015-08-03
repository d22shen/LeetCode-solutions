//my solution
//bad style
class Solution {
public:
    bool isPalindrome(string s) {
        vector<char> temp1, temp2;
        for(int i = 0;i<s.size();i++){
            if(s[i]<=122&&s[i]>=97){
                temp1.push_back(s[i]);
            }else if(s[i]<=90&&s[i]>=65){
                temp1.push_back(s[i]+32);
            }else if(s[i]<=57&&s[i]>=48){
                temp1.push_back(s[i]);
            }
        }
        for(int i = s.size()-1;i>=0;i--){
            if(s[i]<=122&&s[i]>=97){
                temp2.push_back(s[i]);
            }else if(s[i]<=90&&s[i]>=65){
                temp2.push_back(s[i]+32);
            }else if(s[i]<=57&&s[i]>=48){
                temp2.push_back(s[i]);
            }
        }
        if(temp1.empty()||temp2.empty()) return true;
        //here, the size of the vector is no longer s.size() but the temp1.size()
        //because the unrelated characters are excluded
        for(int i = 0;i<temp1.size();i++){
            if(temp1[i]!=temp2[i]) {
                return false;
            }
        }
        
        return true;
    }
};

//better
class Solution {
public:
    bool isPalindrome(string s) {
        if (s.length() < 2) return true;
        
        int left = 0, right = s.length() - 1 ;
        while (left <= right) {
            while (left < s.length() - 1 && !isAlphaNumber(s[left])) ++left;
            while (right >= 0 && !isAlphaNumber(s[right])) --right;
            if (s[left] != s[right] && left <= right) return false;
            ++left;
            --right;
        }
        return true;
    }
    
    bool isAlphaNumber(char &ch) {
        if (ch >= 'a' && ch <= 'z') return true;
        if (ch >= 'A' && ch <= 'Z') {
            ch += 32;
            return true;
        }
        if (ch >= '0' && ch <= '9') return true;
        return false;
    }
    
};