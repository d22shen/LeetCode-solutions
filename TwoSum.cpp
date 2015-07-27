//my solution
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> mymap;
        vector<int> myvector;
        for(int i=1;i<=nums.size();i++){
            mymap[nums[i-1]] = i;
        }
        for(int i = 1;i<=nums.size();i++){
            unordered_map<int,int>::const_iterator p = mymap.find(target-nums[i-1]);
            if(p!=mymap.end()&&i!=p->second){
                myvector.push_back(min(i,p->second));
                myvector.push_back(max(i,p->second));
                break;
            }
        }
        return myvector;
    }
};

//online solution
//the same to mine
class Solution {
public:
    vector<int> twoSum(vector<int> &numbers, int target) {
        vector<int> res;
        map<int, int> numMap;
        for (int i = 0; i < numbers.size(); ++i) {
            numMap[numbers[i]] = i;
        }
        for (int i = 0; i < numbers.size(); ++i) {
            int tmp = target - numbers[i];
            if (numMap.find(tmp) != numMap.end() && numMap[tmp] != i) {
                res.push_back(i + 1);
                res.push_back(numMap[tmp] + 1);
                break;
            }
        }
        return res;
    }
};