int _max(int a, int b) { return a > b ? a : b; }
int _min(int a, int b) { return a < b ? a : b; }

int countIntersectingIntervals(int** intervals, int intervalsSize, int* intervalsColSize) {
    int ans = 0;
    for (int i = 0; i < intervalsSize; i++) {
        for (int j = i+1; j < intervalsSize; j++) {
            int l = _max(intervals[i][0], intervals[j][0]);
            int r = _min(intervals[i][1], intervals[j][1]);
            if (l <= r) ans++;
        }
    }
    return ans;
}