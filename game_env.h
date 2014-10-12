#ifndef GAME_ENV_H
#define GAME_ENV_H

#include "crocgame.h"
using namespace std;

struct GameState{
	int score;
	int playerLocation;
    int backpacker1Activity;
	int backpacker2Activity;
	std::vector<std::pair<double,double>> croc;
	//double calciumReading;
	//double salineReading;
    //double alkalinityReading;
};

struct GameDistributions {
	std::vector<std::pair<double,double>> calcium;
	std::vector<std::pair<double,double>> salinity;
	std::vector<std::pair<double,double>> alkalinity;
	std::vector<std::vector<int>> path;
	std::vector<std::vector<int>> x;
	std::vector<int> i; 
	std::vector<std::vector<long double>> y;
	std::vector<std::vector<int>> q;
	std::vector<int> s;
	std::pair<int, int> step;
};

class GameEnv {
	

	bool print;
	int argSmax;
	CrocSession *_client;
	GameState _state;
	GameDistributions _distr;
	
public:
	
	GameEnv(CrocSession *client){ 
		print = false;
		argSmax = 0;
		//nextStep = -1;
		_client = client;

		
	}
	~GameEnv(void);
	
	void updateGameInfo(void);
	void startGame(void);
	void findCroc(void);
	int minPath(int i, int j);
	bool nextGoal(int i, int j);
	void findPath(void);
	void viterbi(void);
	double countXi(int i, int j);
	double countIx(int j);
	double countIi(int j, int k);
	double pI(int x);
	double iGivenI(int j, int x);
	double xGivenI(int i, int j);
	double iAndX(int i, int j);

	int uMaxS(int j);
	double uMaxQ(int j, int i);
	double uMax(int j, int i);
	int match(int x, int out);
	
private:
	void clearGameInfo(void); 
	
};
//Node arr[] = {std::make_pair(20,10),std::make_pair(30,20),std::make_pair(20,30),std::make_pair(10,20)};
//std::vector<int> TestVector(arr, arr+5);
//const std::vector<Node> GameEnv::_anchors(arr,arr+5);

#endif