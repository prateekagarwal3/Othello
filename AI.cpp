#include <bits/stdc++.h>
#define MAX 1
#define MIN 2
#define DEBUG false
#define mp make_pair
 
using namespace std;
 
typedef vector<vector<int> > v2i;
 
const int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
const int dy[] = {1, 1, 0, -1, -1, -1, 0, 1};
 
void printState(const v2i &board, const int size) {
	for(int i = 0; i<size; i++) {
		for(int j = 0; j<size; j++) {
			printf("%d ", board[i][j]);
		}
		printf("\n");
	}
	printf("-\n");
}
 
bool fillCheck(const v2i &board, const int size) {
	for(int i = 0; i<size; i++) {
		for(int j = 0; j<size; j++)
			if(board[i][j] == 0)
				return false;
	}
	return true;
}

int disks(const v2i &board, const int size, const int player) {
	int count = 0;
	for(int i = 0; i<size; i++) {
		for(int j = 0; j<size; j++) {
			if(board[i][j] == player)
				count++;
		}
	}
	return count;
}
 
int corner(const v2i &board, const int size, const int player) {
	int count = 0;
	if(board[0][0] == player)
		count++;
	if(board[0][size-1] == player)
		count++;
	if(board[size-1][0] == player)
		count++;
	if(board[size-1][size-1] == player)
		count++;
	return count;
}
 
double eval(const v2i &board, const int size) {
	vector<vector<bool> > moveToMax(size, vector<bool>(size, false));
	vector<vector<bool> > moveToMin(size, vector<bool>(size, false));
	// set<pair<int,int> > indMax, indMin;
	int moveMax = 0, moveMin = 0, diskMax = 0, diskMin = 0;
	for(int i = 0; i<size; i++) {
		for(int j = 0; j<size; j++) {
			if(board[i][j] == 0)
				continue;
 
			if(board[i][j] == MAX) {
				diskMax++;
				for(int direct = 0; direct <8; direct++) {
					int next_i = i+dx[direct], next_j = j+dy[direct];
					if(next_i<0 || next_j<0 || next_i>=size || next_j>=size || board[next_i][next_j]==0 || board[next_i][next_j]==MAX)
						continue;
					while(next_i>=0 && next_j>=0 && next_i<size && next_j<size && board[next_i][next_j]!=MAX) {
						if(board[next_i][next_j]==0) {
							if(!moveToMax[next_i][next_j]) {
								moveMax++;
								moveToMax[next_i][next_j] = true;
							}
							// indMax.insert(mp(next_i, next_j));
							break;
						}
						next_i += dx[direct];
						next_j += dy[direct];
					}
				}
			} else {
				diskMin++;
				for(int direct = 0; direct<8; direct++) {
					int next_i = i+dx[direct], next_j = j+dy[direct];
					if(next_i<0 || next_j<0 || next_i>=size || next_j>=size || board[next_i][next_j]==0 || board[next_i][next_j]==MIN)
						continue;
					while(next_i>=0 && next_j>=0 && next_i<size && next_j<size && board[next_i][next_j]!=MIN) {
						if(board[next_i][next_j]==0) {
							if(!moveToMin[next_i][next_j]) {
								moveMin++;
								moveToMin[next_i][next_j] = true;
							}
							// indMin.insert(mp(next_i, next_j));
							break;
						}
						next_i += dx[direct];
						next_j += dy[direct];
					}
				}
			}
		}
	}
	// moveMax = indMax.size();
	// moveMin = indMin.size();
	int cornerMax = corner(board, size, MAX), cornerMin = corner(board, size, MIN);
	double Disks = ((double)(diskMax - diskMin))/((double)(diskMax+diskMin));
	double Moves = (moveMax + moveMin != 0)? ((double)(moveMax - moveMin))/((double)(moveMax+moveMin)) : 0;
	double Corners = (cornerMax + cornerMin != 0)? ((double)(cornerMax - cornerMin))/((double)(cornerMax+cornerMin)) : 0;
	// cout << "DISKS : " << diskMax << " " << diskMin << endl;
	// cout << "MOVES : " << moveMax << " " << moveMin << endl;
	return Disks + 100*Moves + 1000*Corners;
}

void loadNextMoves(const v2i &state, const int size, int player, v2i &nextMove, int &moveCount) {
	moveCount = 0;
	vector<vector<bool> > moveTo(size, vector<bool>(size, false));
	for(int i = 0; i<size; i++) {
		for(int j = 0; j<size; j++) {
			if(state[i][j] != player)
				continue;
			for(int direct = 0; direct<8; direct++) {
				int x = i + dx[direct], y = j + dy[direct];
				if(x<0 || y<0 || x>=size || y>=size || state[x][y]==0 || state[x][y]==player)
					continue;
				while(x>=0 && y>=0 && x<size && y<size && state[x][y]!=player) {
					if(state[x][y]==0) {
						if(!moveTo[x][y]) {
							moveCount++;
							moveTo[x][y] = true;
						}
						break;
					}
					x += dx[direct];
					y += dy[direct];
				}
			}
		}
	}
	for(int i = 0; i<size; i++) {
		for(int j = 0; j<size; j++) {
			if(moveTo[i][j])
				nextMove[i][j] = player;
			else
				nextMove[i][j] = 0;
		}
	}
}

double myValue(const v2i &state, const int size, const int depth, const int maxDepth, v2i &myState, const int player, double alpha, double beta) {
	// cout << "maxvalue depth: " << depth << endl;
	if(fillCheck(state, size))
		return disks(state, size, MAX) - disks(state, size, MIN);
	if(depth == maxDepth+2) {
		return eval(state, size);
	}
	double myUtility = (player == MAX)?INT_MIN:INT_MAX;
	set<pair<int,int> > moveLocation;
	
	for(int i = 0; i<size; i++) {
		for(int j = 0; j<size; j++) {
			if(state[i][j]!=player)
				continue;
			for(int direct=0; direct<8; direct++) {
				int next_i = i+dx[direct], next_j = j+dy[direct];
				if(next_i<0 || next_j<0 || next_i>=size || next_j>=size || state[next_i][next_j]==0 || state[next_i][next_j]==player)
					continue;
				// bool flag = false;
				while(next_i>=0 && next_j>=0 && next_i<size && next_j<size && state[next_i][next_j]!=player) {
					if(state[next_i][next_j]==0) {
						// flag = true;
						moveLocation.insert(mp(next_i, next_j));
						break;
					}
					next_i += dx[direct];
					next_j += dy[direct];
				}
			}
		}
	}
	// sort(moveLocation.begin(), moveLocation.end());
	for(set<pair<int,int> >::iterator it = moveLocation.begin(); it!= moveLocation.end(); it++) {
		v2i copy = state;
		int i = it->first, j = it->second;
		copy[i][j] = player;
		for(int direct = 0; direct < 8; direct++) {
			int x = i + dx[direct], y = j + dy[direct];
			if(x<0 || y<0 || x>=size || y>=size || copy[x][y] == 0 || copy[x][y] == player)
				continue;
			bool flag = false;
			while(x>=0 && y>=0 && x<size && y<size && copy[x][y] != 0) {
				if(copy[x][y] == player) {
					flag = true;
					break;
				}
				x += dx[direct];
				y += dy[direct];
			}
			if(!flag)
				continue;
			x = i+dx[direct];
			y = j+dy[direct];
			while(copy[x][y] != player) {
				copy[x][y] = player;
				x += dx[direct];
				y += dy[direct];
			}
		}
 
		v2i unused = copy;
		
		double temp = myValue(copy, size, depth+1, maxDepth, unused, (player==MAX)?MIN:MAX, alpha, beta);
		if(player == MAX && temp > beta)
			return temp;
		if(player == MIN && temp < alpha)
			return temp;
		if(player == MAX && temp > alpha)
			alpha = temp;
		if(player == MIN && temp < beta)
			beta = temp;
		if( (player==MAX && temp > myUtility) || (player==MIN && temp < myUtility) ) {
			myUtility = temp;
			myState = copy;
		}
	}
	if(moveLocation.empty()) {
		v2i unused = state;
		return myValue(state, size, depth+1, maxDepth, unused, (player==MAX)?MIN:MAX, alpha, beta);
	}
	return myUtility;
}

void playMove(v2i &state, const int size, const int player, const int i, const int j) {
	v2i copy = state;
	copy[i][j] = player;
	for(int direct = 0; direct < 8; direct++) {
		int x = i + dx[direct], y = j + dy[direct];
		if(x<0 || y<0 || x>=size || y>=size || copy[x][y] == 0 || copy[x][y] == player)
			continue;
		bool flag = false;
		while(x>=0 && y>=0 && x<size && y<size && copy[x][y] != 0) {
			if(copy[x][y] == player) {
				flag = true;
				break;
			}
			x += dx[direct];
			y += dy[direct];
		}
		if(!flag)
			continue;
		x = i+dx[direct];
		y = j+dy[direct];
		while(copy[x][y] != player) {
			copy[x][y] = player;
			x += dx[direct];
			y += dy[direct];
		}
	}
	state = copy;
}
 
void minimaxDecision(v2i &state, const int size, const int player, const int maxDepth) {
	v2i ret = state;
	myValue(state, size, 1, maxDepth, ret, player, INT_MIN, INT_MAX);
	state = ret;
}


 
// int main() {
// 	int t, size, m, d;
// 	scanf("%d", &t);
// 	while(t--) {
// 		scanf("%d", &size);
// 		v2i state(size, vector<int>(size));
// 		for(int i = 0; i<size; i++) {
// 			for(int j = 0; j<size; j++) {
// 				scanf("%d", &state[i][j]);
// 			}
// 		}
// 		scanf("%d%d", &m, &d);
// 		//cout << moves(state, size, MAX) << endl;
// 		// printState(minimaxDecision(state, size, MIN, d), size);
		
// 		int player = MAX;
// 		while(m--) {
// 			state = minimaxDecision(state, size, player, d);
// 			printState(state, size);
// 			player = (player == MAX)? MIN : MAX;
// 		}
 
// 		// cout << eval(state, size) << endl;
// 	}
// 	return 0;
// }