#include "game_env.h"
#include <vector>
#include <ppl.h>
#include <algorithm>     //std::find
#include <Windows.h>
#include <sstream>
#include <iostream>
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) > (b)) ? (b) : (a))
//#define abs(a) (((a) < 0.0) ? -1.0*(a) : (a))
using namespace std;


/*----------------------------Interfaces-------------------------------------*/
/* 
*	Should be run first.
*/
void GameEnv::startGame(void){
	//wstring output;
	_client->StartGame();
	_state = GameState();
	_distr = GameDistributions();
	_distr.path = _client->getPaths();
}

void GameEnv::updateGameInfo(void){
	if(print)
		std::cout<<"\n\n # START Update Game Info # \n\n";
	//double c, s, a;
	_state.croc.clear();
	//_state.croc.reserve(3);
	_state.croc.push_back(std::pair<double, double> ());
	_state.croc.push_back(std::pair<double, double> ());
	_state.croc.push_back(std::pair<double, double> ());
	_client->GetGameState(_state.score, _state.playerLocation, _state.backpacker1Activity, _state.backpacker2Activity, _state.croc[0].first, _state.croc[1].first, _state.croc[2].first);
	
	//_state.croc.push_back(std::pair<double, double> );
	_state.croc[0].second = _state.croc[0].first*100.0-(int)(_state.croc[0].first)*100; _state.croc[0].first = (int)_state.croc[0].first;
	_state.croc[1].second = _state.croc[1].first*100.0-(int)(_state.croc[1].first)*100; _state.croc[1].first = (int)_state.croc[1].first; 
	_state.croc[2].second = _state.croc[2].first*100.0-(int)(_state.croc[2].first)*100; _state.croc[2].first = (int)_state.croc[2].first; 
	_client->GetGameDistributions(_distr.calcium, _distr.salinity, _distr.alkalinity);
	if(print){
		std::cout<<"\n\n # START Update Game Info # \n\n";
		std::cout<< "SCORE = " << _state.score << " \n";
		std::cout<< "PLAYER LOCATION = " << _state.playerLocation << " \n";
		std::cout<< "BACKPACKER 1 ACTIVITY = " << _state.backpacker1Activity << " \n";
		std::cout<< "BACKPACKER 2 ACTIVITY = " << _state.backpacker2Activity << " \n";
		std::cout<< "CROC (MEAN, STANDARD DEVIATION) = (" << _state.croc[0].first << ", " << _state.croc[0].second << ") CALCIUM \n";
		std::cout<< "CROC (MEAN, STANDARD DEVIATION) = (" << _state.croc[1].first << ", " << _state.croc[1].second << ") SALINITY \n";
		std::cout<< "CROC (MEAN, STANDARD DEVIATION) = (" << _state.croc[2].first << ", " << _state.croc[2].second << ") ALKALINITY \n\n";
		std::cout<<"\n\nPress [Enter] to continue . . .";
		std::cin.get();
		for(int i = 0; i < _distr.path.size(); i++){
			std::cout<< "WATERHOLE " << i << " (MEAN, STANDARD DEVIATION) = (" << _distr.calcium[i].first << ", " << _distr.calcium[i].second << ") CALCIUM \n";
			std::cout<< "WATERHOLE " << i << " (MEAN, STANDARD DEVIATION) = (" << _distr.salinity[i].first << ", " << _distr.salinity[i].second << ") SALINITY \n";
			std::cout<< "WATERHOLE " << i << " (MEAN, STANDARD DEVIATION) = (" << _distr.alkalinity[i].first << ", " << _distr.alkalinity[i].second << ") ALKALINITY \n\n";
		}
		std::cout<<"\n\n # END Update Game Info # \n\n";
		std::cout<<"\n\nPress [Enter] to continue . . .";
		std::cin.get();
	}
}

void GameEnv::findCroc(void){
	if(print)
		std::cout<<"\n\n # START Find Croc # \n\n";
	//updateGameInfo();
	int n = 0, j = 0;
	_distr.x.clear(); _distr.i.clear();
	std::cout<<"\n\n DISTRIBUTIONS.PATH.SIZE : " << _distr.path.size() << " \n\n";
	for(int i = 0; i < _distr.path.size(); i++){
		
		if(_distr.calcium[i].first-_distr.calcium[i].second <= _state.croc[0].first+_state.croc[0].second && _state.croc[0].first-_state.croc[0].second <= _distr.calcium[i].first+_distr.calcium[i].second){
		//&& _state.croc[0].first-_state.croc[0].second <= _distr.calcium[i].first && _distr.calcium[i].first <= _state.croc[0].first+_state.croc[0].second){

			if(_distr.salinity[i].first-_distr.salinity[i].second <= _state.croc[1].first+_state.croc[1].second && _state.croc[1].first-_state.croc[1].second <= _distr.salinity[i].first+_distr.salinity[i].second){
			//&& _state.croc[1].first-_state.croc[1].second <= _distr.salinity[i].first+_state.croc[0].second && _distr.salinity[i].first-_state.croc[1].second <= _state.croc[1].first+_state.croc[1].second){

				if(_distr.alkalinity[i].first-_distr.alkalinity[i].second <= _state.croc[2].first+_state.croc[2].second && _state.croc[2].first-_state.croc[2].second <= _distr.alkalinity[i].first+_distr.alkalinity[i].second){
				//&& _state.croc[2].first-_state.croc[2].second <= _distr.alkalinity[i].first && _distr.alkalinity[i].first <= _state.croc[2].first+_state.croc[2].second){
					//_distr.x.reserve(_distr.path[i].size());
					if(print)
						std::cout<<"\n\n # START Initialize X & I # \n\n";
						_distr.x.push_back(std::vector<int> ());
					for(int k = 0; k < _distr.path[i].size(); k++){
						_distr.x[_distr.x.size()-1].push_back(_distr.path[i][k]-1);// n++;
						if(print)
							std::cout<<" X[" << _distr.x.size()-1 << "][" << k << "].push(" << _distr.path[i][k]-1 << ")\n";
					}
					//_distr.i.reserve(1);
					_distr.i.push_back(i);// j++;
					if(print){
						std::cout<<"\n I[" << _distr.i.size()-1 << "].push(" << i << ")\n";
						std::cout<<"\n\n # END Initialize X & I # \n\n";
						std::cout<<"\n\nPress [Enter] to continue . . .";
						std::cin.get();
					}
				}

			}

		}

	}

	viterbi();
	if(print){
		std::cout << "\n\n # START Print Y / SIZE " << _distr.y.size() << " # \n\n";

		for(int i = 0; i < _distr.y.size(); i++){
			std::cout << "\n - Y ROW " << i << " / SIZE " << _distr.y[i].size() << " -\n";
			for(int k = 0; k < _distr.y[i].size(); k++)
				std::cout << " Y[" << i << "][" << k << "] = " << _distr.y[i][k] << "\n";
		}
		std::cout << "\n\n # END Print Y  # \n\n";


		std::cout << "\n\n # START Print Q / SIZE " << _distr.q.size() << " # \n\n";

		for(int i = 0; i < _distr.q.size(); i++){
			std::cout << "\n - Q ROW " << i << " / SIZE " << _distr.q[i].size() << " -\n";
			for(int k = 0; k < _distr.q[i].size(); k++)
				std::cout << " Q[" << i << "][" << k << "] = " << _distr.q[i][k] << "\n";
		}

		std::cout << "\n\n # END Print Q  # \n\n";


		std::cout << "\n\n # START Print S / SIZE " << _distr.s.size() << " # \n\n";

		for(int i = 0; i < _distr.s.size(); i++)
			std::cout << " S[" << i << "] = " << _distr.s[i] << "\n";

		std::cout << "\n\n # END Print S  # \n\n";
		//printf (" Suppose that croc vill be at the waterhole number %s \n", _distr.s[0]);
		std::cout<<"\n\n # END Find Croc # \n\n";
		std::cout<<"\n\nPress [Enter] to continue . . .";
		std::cin.get();
	}
}

/* Viterbi (M, X)
1: for i = 1 to m do {Initialization}
2: γ0(i) = πj
3: δ0(i) = 0
4: end for
5: for j = 1 to n do {First phase}
6: for i = 1 to m do
7: γj (i) = max<m, k=1> γj−1(k) * tk(i) * ei(Xj) {Recurrence}
8: δj (i) = arg max<m, k=1> γj−1(k) * tk(i) * ei(Xj)
9: end for
10: end for
11: sn = arg max<m, k=1> γn(k)
12: for i = n − 1 to 1 do {Second phase (back-tracing)}
13: si = δi+1(si+1)
14: end for
15: return s
*/

bool GameEnv::nextGoal(int i, int j){
	for(int x = 0; x < _distr.path[j].size(); x++){
		if(_distr.path[j][x] == i)
			return true;
	}
	return false;
}

int GameEnv::minPath(int i, int j) {
   if(i == j) return -1;
   //std::vector<int> cost;
   int minArg = -1, low = -1;
   for(int x = 0; x < _distr.path[j].size(); x++){
	   for(int k = 0; k < _distr.path[_distr.path[j][x]].size(); k++){
		   for(int r = 0; r < _distr.path[i].size(); r++){
			   if(nextGoal(_distr.path[_distr.path[j][x]][k], _distr.path[i][r]) || nextGoal(_distr.path[j][x], _distr.path[i][r]) || nextGoal(_distr.path[j][x], i) || nextGoal(j, i)){
				   //low = 1;
				   minArg = _distr.path[j][x];
			   }
		   }
		   //if(nextGoal(_distr.path[j][k], i)){
			   //low = 1;

		   //}
	   }
      //int val = 1+i, _distr.path[j][x];
      //cost.push_back(val);
      //if(low < 0 || val < low){ low = val; minArg = _distr.path[j][x]; }
      
   }
   _distr.step.second = minArg;
   return minArg;
}

void GameEnv::findPath(void){
	minPath(_state.playerLocation, _distr.x[0][_distr.s[0]]);
	if(_distr.step.second =! -1 && _distr.step.first == _distr.step.second) { 
		_distr.step.first = _distr.step.second; minPath(_distr.step.first, _distr.s[0]);
	}
	std::cout<<"\n\n FIRST STEP " << _distr.step.first << " SECOND STEP " << _distr.step.second << " GOAL "<< _distr.s[0];
	std::cout<<"\n\nPress [Enter] to continue . . .";
	std::cin.get();
}

void GameEnv::viterbi(void){
	_distr.y.clear(); _distr.q.clear(); _distr.s.clear();
	//_distr.y.reserve(_distr.i.size());
	//_distr.q.reserve(_distr.i.size());
	//(_distr.y[0]).reserve(_distr.x.size());
	//(_distr.q[0]).reserve(_distr.x.size());
	//if(print)
		std::cout << "\n\n # START {Initialization} Y & Q  # \n\n";
	//_distr.y.push_back(std::vector<long double> ());
	//_distr.q.push_back(std::vector<int> ());
	for(int j = 0; j < _distr.i.size(); j++){
		_distr.y.push_back(std::vector<long double> ());
		_distr.q.push_back(std::vector<int> ());
		//_distr.s.push_back(int ());
		int maxArg = 0;
		for(int i = 0; i < _distr.x[j].size(); i++){
			_distr.y[j].push_back(iAndX(j, i));
			//std::cout << _distr.y[0].capacity() << "\n";
			if((_distr.y[j][maxArg]) < (_distr.y[j][i])) maxArg = i;
			//std::cout << _distr.q[0].capacity() << "\n";
			//if(print){
				std::cout << " Y[" << j << "][" << _distr.y[j].size()-1 << "].push(" << _distr.y[j][_distr.y[j].size()-1] << "); \n";
				_distr.q[j].push_back(maxArg);
				std::cout << " Q[" << j << "][" << _distr.q[j].size()-1 << "].push(" << maxArg << "); < Y[" << j << "][" << _distr.q[j].size()-1 << "].push(" << maxArg << ") \n\n";
			//}
		}
		//std::cout << " Q[" << j << "][" << _distr.q[j].size() << "].push(" << maxArg << "); \n\n";
		//_distr.s.push_back(maxArg);
		//_distr.q[j].push_back(maxArg);
		
	}
	//if(print){
		std::cout << "\n\n # END {Initialization} Y & Q # \n\n";
		std::cout<<"\n\nPress [Enter] to continue . . .";
		std::cin.get();
		std::cout << "\n\n # START {First Phase} (Recurrence) # \n\n";
		std::cout << "\n\n # START Initializing Y # \n\n";
	//}
	for(int j = 1; j < _distr.i.size(); j++){
		//_distr.y.push_back(std::vector<double> ());
		//_distr.q.push_back(std::vector<int> ());
		//if(print)
//				std::cout << "\n - Y ROW " << j << " \ SIZE " << _distr.y[j].size() << " - \n";
		for(int i = 0; i < _distr.x[j].size(); i++){
			_distr.y[j][i] = uMaxQ(j, i);
			//if(print)
				std::cout << " Y[" << j << "][" << i << "].push(" << _distr.y[j][i] << "); \n";
		}
	}
	//if(print){
		std::cout << "\n\n # END Initializing Y # \n\n";
		std::cout << "\n\nPress [Enter] to continue . . .";
		std::cin.get();
	//}
	//_distr.s.reserve(_distr.y.size());
	//std::cout << "_distr.y.size() = " << _distr.y.size() << "\n";
	//if(print)
		std::cout<<"\n\n # START Initializing S # \n\n";
	for(int k = 0; k < _distr.i.size(); k++){
		_distr.s.push_back(_distr.q[k][_distr.q[k].size()-1]);//uMaxS(n));
		//if(print)
			std::cout<<" S[" << _distr.s.size()-1 << "].push(" << _distr.s[_distr.s.size()-1]<< ")\n";
	}
	//if(print){
		std::cout << "\n\n # END Initializing S # \n\n";
		std::cout << "\n\n # END {First Phase} # \n\n";
		std::cout << "\n\nPress [Enter] to continue . . .";
		std::cin.get();
		std::cout << "\n\n # START {Second Phase} (back-tracking) # \n\n";
	//}
	//if(print)
		//std::cout<<" S[" << _distr.s.size()-1 << "] = " << _distr.s[_distr.s.size()-1]<< "\n";
		argSmax = 0;
	for(int i = 0; i > _distr.s.size(); i++){
		if(_distr.y[argSmax][_distr.s[argSmax]] <= _distr.y[i][_distr.s[i]]) argSmax = i;
		//out.push_back();
		//if(_distr.y[i+1][_distr.s[i+1]] == 0)
		//_distr.s[i] = _distr.q[i+1][_distr.s[i+1]];
		//if(print)
			std::cout<<" S[" << i << "] = " << _distr.s[i]<< "\n";
	}
	//if(print)
	std::cout<<"\n/# ~ ARG MAX X[" << argSmax << "][ S[" << argSmax << "] = " << _distr.s[argSmax]<< " ] = " << _distr.x[argSmax][_distr.s[argSmax]] << " ~ #/\n";
	//if(print){
		std::cout << "\n\n # END {Second Phase} # \n\n";
		std::cout << "\n\nPress [Enter] to continue . . .";
		std::cin.get();
	//}
	//for(int i = 0; i < _distr.s.size(); i++)
	//std::cout << "_distr.s.size() = " << _distr.s.size() << "\n";
}

/*int GameEnv::qMax(int i){
	int maxArg = 0;
	std::vector<double> out;
	for(int k = 0; k < _distr.q[i].size(); k++){
	 
		if(std::max(_distr.q[i][maxArg], _distr.q[i][k]) == _distr.q[i][k])
			maxArg = k;
	}
	return maxArg;
}*/

/*double GameEnv::pX(int i, j){
	return (matchX(i, 1)/(_distr.path[_distr.i[j]].size()));
}*/

double GameEnv::countXi(int i, int j){
	double out = 0;
	for(int k = 0; k < _distr.i.size(); k++){
		for(int x = 0; x < _distr.x[k].size(); x++){
			if(_distr.x[k][x] == _distr.x[j][i])
				out++;

		}
	}
	//double out = ;
	if(print && out != 0)
		std::cout << "\n # countXi # < ( X[" << j << "][" << i << "] = " << _distr.x[j][i] << " , I[" << j << "] = " << _distr.i[j] << " ) = " << out << " > \n";
	return out;
}

double GameEnv::countIx(int j){
	double out = 0;
	for(int i = 0; i < _distr.i.size(); i++){
		for (int k = 0; k < _distr.x[i].size(); k++){
			for (int x = 0; x < _distr.path[_distr.x[i][k]].size(); x++){
				if(_distr.i[j] == _distr.path[_distr.x[i][k]][x]-1){
					out++;
				}
			}
		}
	}
	//double out = ;
	if(print && out != 0)
		std::cout << "\n # countIx # < ( I[" << j << "] = " << _distr.i[j] << " ) = " << out << " > \n";
	return out;
}

double GameEnv::countIi(int j, int k){
	double out = 0;
	for (int i = 0; i < _distr.i.size(); i++){
		for (int x = 0; x < _distr.x[i].size(); x++){
			if(_distr.i[j] == _distr.x[i][x])// && std::find(_distr.path[_distr.i[j]].begin(), _distr.path[_distr.i[j]].end(), _distr.i[j])!=_distr.path[_distr.i[j]].end())
				out++;
		}
	}
	//double out = ;
	if(print && out != 0)
		std::cout << "\n # countIi # < ( I[" << j << "] = " << _distr.i[j] << " , I[" << k << "] = " << _distr.i[k] << " ) = " << out << " > \n";
	return out;
}

double GameEnv::pI(int x){
	int size = 0;
	for(int i = 0; i < _distr.x.size(); i++){
		size += _distr.x[i].size();
	}
	double out = (countIx(x)/(size+1));
	if(print && out != 0)
		std::cout << "\n # pI # < P( I[" << x << "] = " << _distr.i[x] << " ) = " << out << " > \n";
	return out;
}

double GameEnv::iGivenI(int j, int x){
	double out = ((pI(x)*countIi(j, x))/_distr.x[x].size());
	if(print && out != 0)
		std::cout << "\n # iGivenI # < P( I[" << j << "] = " << _distr.i[j] << " | I[" << x << "] = " << _distr.i[x] << " ) = " << out << " > \n";
	return out;

}

double GameEnv::xGivenI(int i, int j){
	double out = ((pI(j)*countXi(i, j))/_distr.x[j].size());
	if(print && out != 0)
		std::cout << "\n # xGivenI # < P( X[" << j << "][" << i << "] = " << _distr.x[j][i] << " | I[" << j << "] = " << _distr.i[j] << " ) = " << out << " > \n";
	return out;
}

double GameEnv::iAndX(int k, int j){
	double out = (pI(k)*(10000.0)*xGivenI(j, k));
	if(print && out != 0)
		std::cout << "\n # iAndX # < P( I[" << k << "] = " << _distr.i[k] << " , X[" << k << "][" << j << "] = " << _distr.x[k][j] << " ) = " << out << " > \n";
	return out;
}

int GameEnv::uMaxS(int j){
	///*******/////
	int maxArg = 0;
	//#define max(a,b) (((a) > (b)) ? (a) : (b))
	for (int k = 0; k < _distr.y[j].size(); k++){
		//if(_distr.y[j][maxArg] < _distr.y[j][k])
			maxArg = k;
	}
	//_distr.s[j] = maxArg;
	return maxArg;
}

double GameEnv::uMaxQ(int j, int i){
	if(print){
		std::cout << "\n\n # START uMaxQ # \n\n";
		std::cout << "\n\n # START Initializing Y # \n\n";
	}
	std::vector<double> out;
	//std::vector<int> outArg;
	int maxArg = 0;
	//out.reserve(_distr.y[j].size());
	//int sizeYj = _distr.y[j-1].size();
	//_distr.y.push_back(std::vector<double> ());
	//double Yjk = 0;
	for (int k = 0; k < _distr.y[j-1].size(); k++){
		//_distr.y[j].push_back(double ());
		//std::cout << "\n ~ # Y[" << j-1 << "][S[" << j-1 << "] = " << _distr.s[j-1] << "] = " << _distr.y[j-1][_distr.s[j-1]] << " # ~\n";
		out.push_back(_distr.y[j-1][_distr.y[j-1].size()-1]*xGivenI(k, j-1)*iGivenI(j, j-1));
		//if(xGivenI(k, j) != 0) 
			//std::cout << "\n < xGivenI >  ( " << k << ", " << j << " ) " << xGivenI(k, j) << " ; xGivenI(" << i << ", " << j << ") " << xGivenI(i, j) << "\n";
		//_distr.y[j][_distr.y[j].size()-1] = Yjk; 
		//if(print)
			//std::cout << " OUT[" << k << "].push(" << _distr.y[j][_distr.y[j].size()-1] << "); \n";
		//out[k] *=uMax(j--, k)*xGivenI(k, j)*iGivenI(j, j--);
		//max = max(max, out[k]);
		if(_distr.y[j-1][k]*xGivenI(k, j-1)*iGivenI(j, j-1) > _distr.y[j-1][maxArg]*xGivenI(maxArg, j-1)*iGivenI(j, j-1)){
			maxArg = k;
			//Yjk = out[maxArg];
			 //std::cout << " ~ MAXARG Y[" << j << "][" << maxArg << "] = " << out[maxArg] << " ~ \n";
		}
		else{
			//std::cout << " ~ CURRENT ARG Y[" << j << "][" << k << "] = " << out[k] << " < MAXARG Y[" << j << "][" << maxArg << "] = " << out[maxArg] << " ~ \n";
		}
	}
	if(print)
		std::cout << "\n\n # END Initializing Y # \n\n";
	//int sizeQ = _distr.q.size();
	//if(_distr.q.size() <= j) _distr.q.push_back(std::vector<int> ());
	//out.clear();
	//_distr.s[j] = maxArg;
	if(print){
		std::cout << "\n Q[" << j << "][" << _distr.q[j].size()-1 << "].push(" << _distr.q[j][_distr.q[j].size()-1] << "); \n";
		std::cout << "\n\n # END uMaxQ ( MAX Q : " << _distr.y[j][maxArg] << " ) # \n\n";
		//std::cout << "\n\nPress [Enter] to continue . . .";
		//std::cin.get();
	}
	return _distr.y[j][maxArg];
}

double GameEnv::uMax(int j, int i){
	///*******/////
	if(j <= 0) return _distr.y[0][i];
	std::vector<double> out;
	double max = 0;
	//out.reserve(_distr.x.size());
	int size = _distr.x.size();
	for (int k = 0; k < size; k++){
		out.push_back(uMax(j-1, k)*xGivenI(k, j)*iGivenI(j, j-1));
		max = max(max, out[k]);
	}
	//out.clear();
	return max;
}

/*-------------------------Private Interfaces--------------------------------*/
void GameEnv::clearGameInfo(void){
	
}

/*----------------------------Clean-up---------------------------------------*/

GameEnv::~GameEnv(){
	//std::exit(0);
	//delete _client;
	_client->~CrocSession ();
	this->clearGameInfo();
	std::exit(0);
}
