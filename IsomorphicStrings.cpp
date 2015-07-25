//my solution
class Solution {
public:
    bool isIsomorphic(string s, string t) {
        if(s.size()!=t.size())
            return false;
        unordered_map<char, char> myhashST1, myhashST2;
        for(int i = 0;i < s.size(); i++){
            myhashST1[s[i]] = t[i];
            myhashST2[t[i]] = s[i];
        }
        for(int i=0;i<s.size();i++){
            if(t[i]!=myhashST1[s[i]]||s[i]!=myhashST2[t[i]])
                return false;
        }
        return true;
    }
};

//better solution
class Solution {
public:
    bool isIsomorphic(string s, string t) {
        int m1[256] = {0}, m2[256] = {0}, n = s.size();
        for (int i = 0; i < n; ++i) {
            if (m1[s[i]] != m2[t[i]]) return false;
            m1[s[i]] = i + 1;
            m2[t[i]] = i + 1;
        }
        return true;
    }
};