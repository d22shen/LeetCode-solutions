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