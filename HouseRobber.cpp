//my solution
//这道题的本质是在一列数组中取出一个或多个不相邻数，使其和最大，要用dynamic programming
//维持一个数组dp
//dp[i]表示到i位置时，不相邻数能形成的最大和
//递推公式dp[i] = max(num[i]+dp[i-2],dp[i-1])
//初始化dp[0] = num[0]和dp[1] = max(num[0],num[1]);
class Solution {
public:
    int rob(vector<int> &num) {
        if (num.size() <= 1) return num.empty() ? 0 : num[0];
        vector<int> dp = {num[0], max(num[0], num[1])};
        for (int i = 2; i < num.size(); ++i) {
            dp.push_back(max(num[i] + dp[i - 2], dp[i - 1]));
        }
        return dp.back();
    }
};

//分奇偶
class Solution {
public:
    int rob(vector<int> &num) {
        int a = 0, b = 0;
        for (int i = 0; i < num.size(); ++i) {
            if (i % 2 == 0) {
                a += num[i];
                a = max(a, b);
            } else {
                b += num[i];
                b = max(a, b);
            }
        }
        return max(a, b);
    }
};

//分奇偶简化版
class Solution {
public:
    int rob(vector<int> &nums) {
        int a = 0, b = 0;
        for (int i = 0; i < nums.size(); ++i) {
            int m = a, n = b;
            a = n + nums[i];
            b = max(m, n);
        }
        return max(a, b);
    }
};