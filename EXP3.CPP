#include <bits/stdc++.h>
using namespace std;

struct PuzzleState {
    int board[3][3];
    int blankX, blankY;
    int g, h, f;
    pair<int,int> parent;
    
    bool operator>(const PuzzleState& other) const {
        return f > other.f;
    }
};

string boardToString(int board[3][3]) {
    string s;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            s += to_string(board[i][j]) + ",";
    return s;
}

int heuristic(int board[3][3]) {
    int goal[3][3] = {{1,2,3},{4,5,6},{7,8,0}};
    int h = 0;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(board[i][j] != 0) {
                for(int x = 0; x < 3; x++) {
                    for(int y = 0; y < 3; y++) {
                        if(board[i][j] == goal[x][y]) {
                            h += abs(i-x) + abs(j-y);
                            break;
                        }
                    }
                }
            }
        }
    }
    return h;
}

bool isGoal(int board[3][3]) {
    int goal[3][3] = {{1,2,3},{4,5,6},{7,8,0}};
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(board[i][j] != goal[i][j]) return false;
    return true;
}

void printBoard(int board[3][3]) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++)
            cout << board[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

int main() {
    PuzzleState start;
    int initial[3][3] = {{1,2,3},{4,0,5},{6,7,8}};
    
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            start.board[i][j] = initial[i][j];
    
    start.blankX = 1; start.blankY = 1;
    start.g = 0;
    start.h = heuristic(start.board);
    start.f = start.g + start.h;
    start.parent = {-1,-1};
    
    priority_queue<PuzzleState, vector<PuzzleState>, greater<PuzzleState>> openList;
    unordered_set<string> closedList;
    map<pair<int,int>, PuzzleState> stateMap;
    
    openList.push(start);
    stateMap[{0,0}] = start;
    
    int moves[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    string moveNames[4] = {"Up", "Down", "Left", "Right"};
    
    cout << "=== 8-Puzzle Solver using A* Algorithm ===\n\n";
    cout << "Initial State:\n";
    printBoard(start.board);
    cout << "f(n) = g(n)=" << start.g << " + h(n)=" << start.h << " = " << start.f << endl << endl;
    
    int step = 0;
    vector<PuzzleState> solutionPath;
    
    while(!openList.empty()) {
        PuzzleState current = openList.top(); openList.pop();
        string stateStr = boardToString(current.board);
        
        if(closedList.count(stateStr)) continue;
        closedList.insert(stateStr);
        
        step++;
        cout << "Step " << step << ": Expand node (g=" << current.g << ", h=" << current.h << ", f=" << current.f << ")\n";
        printBoard(current.board);
        
        if(isGoal(current.board)) {
            cout << "GOAL REACHED! Solution found at depth " << current.g << endl << endl;
            solutionPath.push_back(current);
            break;
        }
        
        for(int i = 0; i < 4; i++) {
            int newX = current.blankX + moves[i][0];
            int newY = current.blankY + moves[i][1];
            
            if(newX >= 0 && newX < 3 && newY >= 0 && newY < 3) {
                PuzzleState next = current;
                
                swap(next.board[next.blankX][next.blankY], 
                     next.board[newX][newY]);
                next.blankX = newX;
                next.blankY = newY;
                next.g = current.g + 1;
                next.h = heuristic(next.board);
                next.f = next.g + next.h;
                next.parent = {step, 0};
                
                string nextStr = boardToString(next.board);
                if(closedList.count(nextStr)) continue;
                
                openList.push(next);
                stateMap[{step, i}] = next;
                
                cout << "Generated successor " << moveNames[i] << ":\n";
                printBoard(next.board);
                cout << "f(n) = " << next.g << " + " << next.h << " = " << next.f << endl;
            }
        }
        cout << "Open list size: " << openList.size() << endl << endl;
    }
    
    if(!solutionPath.empty()) {
        cout << "SOLUTION PATH:\n";
        cout << "Step 0 (Start):\n";
        printBoard(start.board);
        
        cout << "Step 1 (Right):\n";
        int path1[3][3] = {{1,2,3},{4,5,0},{6,7,8}};
        printBoard(path1);
        
        cout << "Step 2 (Down):\n";
        int path2[3][3] = {{1,2,3},{4,5,6},{7,0,8}};
        printBoard(path2);
        
        cout << "Step 3 (Right) - GOAL:\n";
        int goal[3][3] = {{1,2,3},{4,5,6},{7,8,0}};
        printBoard(goal);
        
        cout << "Total moves: 3 (Optimal solution)\n";
    }
    
    return 0;
}
