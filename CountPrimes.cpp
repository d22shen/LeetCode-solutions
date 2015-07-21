//不用判定n次，sqrt(n)就够了
//素数从2开始，2是最小的素数
//只能被1和自身整除
//全用循环太慢
//下面的方法叫Sieve of Eratosthenes
//用一个vector存是否是质数，所有的都初始化成true
//time complexity O(nlog(logn))
class Solution {
public:
    int countPrimes(int n) {
        vector<bool> isPrime(2,true);
        for(int i=2;i<n;i++){
            isPrime.push_back(true);
        }
        for(int i=2;i*i<n;i++){
            //如果不是质数，就进行下一次循环
            if(!isPrime[i]) continue;
            //如果是质数，就判定他的平方
            //vector isPrime的index就是要判定的数
            //mark off multiples of p starting at p2, then in increments of p: p2 + p, p2 + 2p, ... 
            for (int j = i * i; j < n; j += i) {
                isPrime[j] = false;
            }
        }
        int count = 0;
        //遍历，看一共多少
        for (int i = 2; i < n; i++) {
            if (isPrime[i])
                count++;
        }
        return count;
    }
};