#include <iostream>
#include <vector>
#include "sat.h"

using namespace std;

class Gate
{
public:
   Gate(unsigned i = 0): _gid(i) {}
   ~Gate() {}

   Var getVar() const { return _var; }
   void setVar(const Var& v) { _var = v; }

private:
   unsigned   _gid;  // for debugging purpose...
   Var        _var;
};

vector<Gate *> B;
vector<Gate *> W;
vector<Gate *> X;
vector<Gate *> Bk;
vector<Gate *> Wk;
vector<Gate *> Y;
Gate* Bwin;


int literal_num = 0;
void
initCircuit(int steps)
{
    // Init gates
    // B[i][j]
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 15; j++){
			B.push_back(new Gate(literal_num));
			literal_num++;
		}
	}

	// W[i][j]
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 15; j++){
			W.push_back(new Gate(literal_num));
			literal_num++;
		}
	}
	
	// X[i][j]

	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 15; j++){
			X.push_back(new Gate(literal_num));
			literal_num++;
		}
	}
	
	// B[i][j][k]	
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 15; j++){
			for(int k = 0; k < steps; l++){
				Bk.push_back(new Gate(literal_num));
				literal_num++;
			}
		}
	}
	
	// W[i][j][k]	
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 15; j++){
			for(int k = 0; k < steps; l++){
				Wk.push_back(new Gate(literal_num));
				literal_num++;
			}
		}
	}
	
	// Y[i][j][s][t][k]
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 15; j++){
			for(int s = 0; s < 15; s++){
				for(int t = 0; t < 15; t++){
					for(int l = k; l < steps; l++){
						Y.push_back(new Gate(literal_num));
						literal_num++;
					}
				}
			}
		}
	}
	
	Bwin = new Gate(literal_num);
	literal_num++;


}

void
genProofModel(SatSolver& s, int steps)
{
	// Allocate and record variables; No Var ID for POs
	for (size_t i = 0, n = B.size(); i < n; ++i) {
    	Var v = s.newVar();
    	B[i]->setVar(v);
	}
	for (size_t i = 0, n = W.size(); i < n; ++i) {
      Var v = s.newVar();
      W[i]->setVar(v);
	}
	for (size_t i = 0, n = X.size(); i < n; ++i) {
      Var v = s.newVar();
      X[i]->setVar(v);
	}
	for (size_t i = 0, n = Bk.size(); i < n; ++i) {
      Var v = s.newVar();
      Bk[i]->setVar(v);
	}
	for (size_t i = 0, n = Wk.size(); i < n; ++i) {
      Var v = s.newVar();
      Wk[i]->setVar(v);
	}
	for (size_t i = 0, n = Y.size(); i < n; ++i) {
      Var v = s.newVar();
      Y[i]->setVar(v);
	}
    Var v = s.newVar();
    Bwin->setVar(v);
	
	// Hard code the model construction here...
   	// only B[i][j] or W[i][j] is true
	for(size_t i = 0, n = B.size(); i < n; ++i){
		vec<Lit> lits;
		Lit la = ~Lit(B[i]->getVar());
		Lit lb = ~Lit(W[i]->getVar());
		lits.push(la); lits.push(lb);
		s.addCNF(lits);
	}

	// B[i][j] or W[i][j] => X[i][j]
	for(size_t i = 0, n = B.size(); i < n; ++i){
		vec<Lit> lits;
		Lit la = ~Lit(B[i]->getVar());
		Lit lb = ~Lit(W[i]->getVar());
		Lit lc = Lit(X[i]->getVar());
		lits.push(la); lits.push(lc);
		s.addCNF(lits);
		lits.push(lb); lits.push(lc);
		s.addCNF(lits);
	}
	// (i,j) can only be placed one time
	for(size_t i = 0; i < 15; ++i){
		for(size_t j = 0; j < 15; ++j){
			for(size_t k = 0; k < steps; ++k){
				vec<Lit> lits;
				Lit la = ~Lit(Bk[i * (15 * steps) + j * steps + k]->getVar());
				for(size_t l = k + 1; l < steps; ++l){
					Lit lb = ~Lit(Bk[i * (15 * steps) + j * steps + l]->getVar());
					lits.push(la); lits.push(lb);
					s.addCNF(lits);
				}
			}
		}
	}
	for(size_t i = 0; i < 15; ++i){
		for(size_t j = 0; j < 15; ++j){
			for(size_t k = 0; k < steps; ++k){
				vec<Lit> lits;
				Lit la = ~Lit(Wk[i * (15 * steps) + j * steps + k]->getVar());
				for(size_t l = k + 1; l < steps; ++l){
					Lit lb = ~Lit(Wk[i * (15 * steps) + j * steps + l]->getVar());
					lits.push(la); lits.push(lb);
					s.addCNF(lits);
				}
			}
		}
	}
	
	// k_th step only can be taken once
	for(size_t k = 0; i < steps; ++k){
		for(size_t i = 0; i < 15; ++i){
			for(size_t j = 0; j < 15; ++j){
				vec<Lit> lits;
				Lit la = ~Lit(Bk[i * (15 * steps) + j * steps + k]->getVar());
				for(size_t s = i; s < 15; ++s){
					for(size_t t = j; t < 15; ++t){
						Lit lb = ~Lit(Bk[s * (15 * steps) + t * steps + k]->getVar());
						lits.push(la); lits.push(lb);
						s.addCNF(lits);
					}
				}
			}
		}
	}
	for(size_t k = 0; i < steps; ++k){
		for(size_t i = 0; i < 15; ++i){
			for(size_t j = 0; j < 15; ++j){
				vec<Lit> lits;
				Lit la = ~Lit(Wk[i * (15 * steps) + j * steps + k]->getVar());
				for(size_t s = i; s < 15; ++s){
					for(size_t t = j; t < 15; ++t){
						Lit lb = ~Lit(Wk[s * (15 * steps) + t * steps + k]->getVar());
						lits.push(la); lits.push(lb);
						s.addCNF(lits);
					}
				}
			}
		}
	}
			
}

void reportResult(const SatSolver& solver, bool result)
{
   solver.printStats();
   cout << (result? "SAT" : "UNSAT") << endl;
  // if (result) {
  //    for (size_t i = 0, n = gates.size(); i < n; ++i)
  //       cout << solver.getValue(gates[i]->getVar()) << endl;
  // }
}

int main()
{
   initCircuit();

   SatSolver solver;
   solver.initialize();

   //
   genProofModel(solver);

   bool result;
   solver.assumeRelease();  // Clear assumptions
   result = solver.solve;
   reportResult(solver, result);
   cout << endl << endl << "======================" << endl;

}
