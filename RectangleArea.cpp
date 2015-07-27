//online solution
class Solution {
public:
    int computeArea(int A, int B, int C, int D, int E, int F, int G, int H) {
        //the total covered area but may have overlap
        int sum = (C-A)*(D-B)+(H-F)*(G-E);
        
        //if the two rectangles do not overlap, return the total area
        if(G<=A||E>=C||F>=D||H<=B)
            return sum;
        
        //get the common area of the two rectangles
        return sum - (min(G,C) - max(A,E))*(min(H,D) - max(F,B));
        
    }
};

//one line solution
//max int of 32 bit is 2,147,483,647
//however, the test case -1,500,000,001, 0, -1500000000, 1, 1500000000, 0, 1500000001, 1 is too big
//when using the one line solution
class Solution {
public:
    int computeArea(int A, int B, int C, int D, int E, int F, int G, int H) {
        return (C - A) * (D - B) + (H - F) * (G - E) - (max((min(G, C) - max(A, E)), 0) * max((min(D, H) - max(B, F)), 0));
    }
};

//min(G,C) - max(A,E) = -3,000,000,000 overflow
//hence, this does not work