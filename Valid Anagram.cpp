//my solution
//70+ ms
//I cannot understand the requirements as a foreigner even with Google!!
class Solution {
public:
    bool isAnagram(string s, string t) {
        if(s.size()!=t.size()) return false;
        unordered_map<char,int> mymap;
        for(int i=0;i<s.size();i++){
            mymap[s[i]] = 0;
        }
        for(int i=0;i<s.size();i++){
            mymap[s[i]]++;
        }
        //the showup times of the char is limited
        for(int i=0;i<t.size();i++){
            if(mymap.find(t[i]) == mymap.end()) return false;
            --mymap[t[i]];
            if(mymap[t[i]]<0) return false;
        }
        return true;
    }
};

//much better solution
class Solution {
public:
    bool isAnagram(string s, string t) {
        if (s.size() != t.size()) return false;
        int m[26] = {0};
        for (int i = 0; i < s.size(); ++i) ++m[s[i] - 'a'];
        for (int i = 0; i < t.size(); ++i) {
            if (--m[t[i] - 'a'] < 0) return false;
        }
        return true;
    }
};
