//my solution
class Solution {
public:
    int titleToNumber(string s) {
        int size = s.size();
        if(!size) return 0;
        int column = 0;
        for(int i = 0;i < size;i++){
            column += pow(26,i)*(s[size - i -1] - 'A' + 1) ;
        }
        return column;
    }
};

