#ifndef AI_H
#define AI_H

void loadNextMoves(const std::vector<std::vector<int> > &, const int, int, std::vector<std::vector<int> > &, int &);
void playMove(std::vector<std::vector<int> > &, const int, const int, const int, const int);
void minimaxDecision(std::vector<std::vector<int> > &, const int, const int, const int);
void printState(const std::vector<std::vector<int> > &, const int);
int disks(const std::vector<std::vector<int> > &, const int, const int);

#endif