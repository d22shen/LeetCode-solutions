//my solution
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        unordered_map<int, int> m;
        for (int i = 0; i < nums.size(); ++i) {
            auto key = m.find(nums[i]);
            if (key != m.end()&&abs(key->second-i)<=k) return true;
            m[nums[i]] = i;
        }
        return false;
    }
};

//slightly faster solution
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        unordered_map<int, int> m;
        int d = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (m.find(nums[i]) != m.end()) {
                d = i - m[nums[i]];
                if (d <= k) return true;
            }
            m[nums[i]] = i;
        }
        return false;
    }
};

//online 24ms solution using set
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        int size = nums.size();
        int maxElements = k + 1;
        // the difference between first and last element is at most k
        if (size <= maxElements)
        {
            set<int> s(nums.begin(), nums.end());
            return s.size() != size;
        }
        
        set<int> s(nums.begin(), nums.begin() + maxElements);
        for (int i = maxElements; i < size; i++)
        {
            if (s.size() != maxElements)
                return true;
            
            s.erase(nums[i - maxElements]);
            s.insert(nums[i]);
        }
        return s.size() != maxElements;
    }
};