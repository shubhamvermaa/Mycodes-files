#define vint vector<int>
class Solution {
public:
  void build(vint& segTree, vint& v, int low, int high, int ind = 0) {
    if (low == high) { segTree[ind] = v[low]; return; }
    int mid = (low + high) >> 1;
    build(segTree, v, low, mid, 2 * ind + 1);
    build(segTree, v, mid + 1, high, 2 * ind + 2);
    segTree[ind] = segTree[2 * ind + 1] + segTree[2 * ind + 2];
  }
  // search space : l, h zero based
  int query(vint& segTree, int low, int high, int left, int right, int ind = 0) {
    if (high < left or low > right) return 0;// low high  l  h or l  h low high
    if (low >= left and high <= right) return segTree[ind];// l  low  high h
    int mid = (low + high) >> 1;
    //(low   l   h  high) or (low  l  high h) or (l low  h high) : further investigation is required
    int fromLeft = query(segTree, low, mid, left, right, 2 * ind + 1);
    int fromRight = query(segTree, mid + 1, high, left, right, 2 * ind + 2);
    return fromLeft + fromRight;
  }
  // Changing the value at updateIdx to val in v, and updating the segment tree correspondingly
  void pointUpdate(vint& segTree, vint& v, int low, int high, int updateIdx, int val, int ind = 0) {
    if (low == high) {
      v[updateIdx] = val;
      segTree[ind] = val;
      return;
    }
    int mid = (low + high) >> 1;
    if (low <= updateIdx and updateIdx <= mid) pointUpdate(segTree, v, low, mid, updateIdx, val, 2 * ind + 1);
    else pointUpdate(segTree, v, mid + 1, high, updateIdx, val, 2 * ind + 2);
    segTree[ind] = segTree[2 * ind + 1] + segTree[2 * ind + 2];
  }
  void printqueue(queue<int> q) {
    while(q.empty() == false) {
      cout<<q.front()<<' ';
      q.pop();
    }
    cout<<endl;
  }
  string minInteger(string num, int k) {
    int n = num.size();
    vector<queue<int>> vq(10);
    for(int i = 0; i < n; i++) vq[num[i] - '0'].push(i);

    // creating segmenttree to get the number of operations performed before the current working index
    vector<int> seg(4 * n), operations(n);
    build(seg, operations, 0, n - 1);

    string ans = "";
    for(int i = 0; i < n; i++) {
      for(int j = 0; j <= 10; j++) {
        if(vq[j].size() == 0) continue;

        int pos = vq[j].front();// position of the optimal number
        // if 0th index number is selected as the optimal number then its left does not exist, so the number of operations happened on the left of 0th index number is 0.
        int leftOperations = (pos - 1 >= 0) ? query(seg, 0, n - 1, 0, pos - 1) : 0;
        int steps = pos - leftOperations;

        if(steps <= k) {
          k -= steps;
          ans.push_back(char(j + '0'));// putting the optimal digit in the answer
          pointUpdate(seg, operations, 0, n - 1, pos, 1);// marking that, an operation has occured at the index pos
          vq[j].pop();// the optimal number is utilized so its occurance is removed
          break;// current index is filled with the optimal digit, so no need to continue
        }
      }
    }
    return ans;
  }
};
