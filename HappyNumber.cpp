//my solution
class Solution {
public:
    bool isHappy(int n) {
        if(n<=0) return false;
        int p = n, t = 0;
        
        unordered_map<int, bool> myvector;
        myvector[1] = true;
        if(p!=1) myvector[p] = false;
        
        while(true){
            while(p!=0){
                t += pow(p%10,2);
                p = p/10;
            }
            if(myvector.find(t)!=myvector.end()) return myvector[t];
            myvector[t] = false;
            p = t;
            t = 0;
        }
    }
};

//slower than mine but simpler written
class Solution {
public:
    bool isHappy(int n) {
        unordered_map<int, int> m;
        while (n != 1) {
            int sum = 0;
            while (n > 0) {
                sum += ((n % 10) * (n % 10));
                n /= 10;
            }
            n = sum;
            if (m.find(n) != m.end()) break;
            else m[n] = 1;
        }
        return n == 1;
    }
};