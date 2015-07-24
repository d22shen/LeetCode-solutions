//my solution
//isEmpty to check whether it is consisted of only white spaces
//use a string to store the tokens
//get the last token to get the length
//bad solution I think
class Solution {
public:
    bool isEmpty(string s){
        for(int i=0;i<s.size();i++){
            if(s[i] != ' ')
                return false;
        }
        return true;
    }
    int lengthOfLastWord(string s) {
        if(s.empty())
            return 0;
        if(isEmpty(s))
            return 0;
        
        vector<string> myvector;
        stringstream ss(s);
        string buf;
        //buf is to get the token separated by the white spaces
        while(ss>>buf)
            myvector.push_back(buf);
        buf = myvector[myvector.size()-1];
        return buf.size();
    }
};

//other solutions, the same time complexity as mine-4ms
//but quite straight forward

class Solution {
public:
    int lengthOfLastWord(string s) {
        int len = s.size();
        int left = 0;
        int right = len - 1;
        int count = 0;
        while (s[left] == ' ') ++left;
        while (s[right] == ' ') --right;
        for (int i = left; i <= right; ++i) {
            if (s[i] == ' ') count = 0;
            else ++count;
        }
        return count;
    }
};

//simpler but same 4ms
class Solution {
public:
    int lengthOfLastWord(string s) {
        int len = s.size();
        int count = 0;
        for(int i = len-1;i>=0;i--){
            if(s[i] == ' '){
                if(count == 0) continue;
                return count;
            }
            count++;
        }
        return count;
    }
};











