#include <cmath>
#include <vector>
#include <iostream>
#include <stdio.h>

using namespace std;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef long long L;
typedef vector<L> VL;
typedef vector<VL> VVL;
typedef pair<int, int> PII;
typedef vector<PII> VPII;
const L INF = 9999999;//numeric_limits<L>::max() / 4;

struct MinCostMaxFlow {
  int N;
  VVL cap, flow, cost;
  VI found;
  VL dist, pi, width;
  VPII dad;
  MinCostMaxFlow(int N) : 
    N(N), cap(N, VL(N)), flow(N, VL(N)), cost(N, VL(N)), 
    found(N), dist(N), pi(N), width(N), dad(N) {}
  
  void AddEdge(int from, int to, L cap, L cost) {
    this->cap[from][to] = cap;
    this->cost[from][to] = cost;
  }
  void Relax(int s, int k, L cap, L cost, int dir) {
    L val = dist[s] + pi[s] - pi[k] + cost;
    if (cap && val < dist[k]) {
      dist[k] = val;
      dad[k] = make_pair(s, dir);
      width[k] = min(cap, width[s]);
    }
  }
  L Dijkstra(int s, int t) {
    fill(found.begin(), found.end(), false);
    fill(dist.begin(), dist.end(), INF);
    fill(width.begin(), width.end(), 0);
    dist[s] = 0;
    width[s] = INF;  
    while (s != -1) {
      int best = -1;
      found[s] = true;
      for (int k = 0; k < N; k++) {
        if (found[k]) continue;
        Relax(s, k, cap[s][k] - flow[s][k], cost[s][k], 1);
        Relax(s, k, flow[k][s], -cost[k][s], -1);
        if (best == -1 || dist[k] < dist[best]) best = k;
      }
      s = best;
    }
    for (int k = 0; k < N; k++)
      pi[k] = min(pi[k] + dist[k], INF);
    return width[t];
  }
  pair<L, L> GetMaxFlow(int s, int t) {
  	
    L totflow = 0, totcost = 0;
    while (L amt = Dijkstra(s, t)) {
      totflow += amt;
      for (int x = t; x != s; x = dad[x].first) {
        if (dad[x].second == 1) {
          flow[dad[x].first][x] += amt;
          totcost += amt * cost[dad[x].first][x];
        } else {
          flow[x][dad[x].first] -= amt;
          totcost -= amt * cost[x][dad[x].first];
        }
      }
    }
    return make_pair(totflow, totcost);
  }
  
  VVL getCap() {
  	return flow;
  }
  
  VVL getMaxCap() {
  	return cap;
  }
  
  VVL getCost() {
  	return cost;
  }
};

int main() {
	//Matriz de costo de produccion de la energia j en el estado i
	int c[5][3] = {{18,17,11},{24,14,27},{9,22,23},{12,15,13},{19,11,24}};
	//Matriz de limite de produccion de energia de la fuente j en el estado i
	int l[5][3] = {{360,2784,1568},{690,5410,3173},{9785,5017,5594},{3470,2784,6241},{340,1874,1654}};
	//Matriz de costo de transporte de una unidad de energia del estado i al estado j
	int k[5][5] = {{0,8,5,12,6},{8,0,10,22,18},{5,10,0,4,4},{12,22,4,0,2},{6,18,4,2,0}};
	//Arreglo de necesidad energetica del estado i
	int p[5] = {4710,9270,20390,12490,3860};
	
	//37 nodos en total, incluyendo nodo fuente y destino
	MinCostMaxFlow *maxFlow = new MinCostMaxFlow(37);
	
	for(int i = 0; i < 37; i++) {
		for(int j = 0; j < 37; j++) {
			maxFlow->AddEdge(i, j, 0, INF); // inicial a producción
		}
	}
	
	for(int i = 1; i <= 15; i++) {
		maxFlow->AddEdge(0, i, INF, 0); // inicial a producción
	}
	
	int cont = 1;
	for(int i = 0; i < 5; i++) {
		for(int j = 0; j < 3; j++) {
       		maxFlow->AddEdge(cont,cont + 15, l[i][j],c[i][j]); // producción a envío
			cont++;
    	}
	}
	
	//Inicializa de envio a consumo
	cont = 16;
	for(int i = 0; i < 5; i++) {
		for(int k1 = 0; k1 < 3; k1++) {
			for(int j = 0; j < 5; j++) {
				maxFlow->AddEdge(cont,j+31,INF,k[i][j]);
			}
			cont++;
		}
	}
	
	//Inicilaiza de consumo a final
	for(int i = 31; i <= 35; i++) {
		maxFlow->AddEdge(i,36,p[i-31],0);
	}
	
	std::pair <int,int> resp = maxFlow->GetMaxFlow(0, 36);
	
	printf("Energia producida: %lli Costo total: %lli \n", resp.first, resp.second);
	
	std::cout << "\n\n";
	
	//Imprimir X
	cont = 1;
	std::cout << "Produccion por estado y enegria \n"; 
	for(int i = 0; i < 5; i++) {
		for(int j = 0; j < 3; j++) {
       		std::cout << "Estado " << i << " Energia " << j << ": " << maxFlow->getCap()[cont][cont+15] << "     ";
			cont++;
    	}
    	std::cout << "\n";
	}
	
	std::cout << "\n\n";
	
	//Imprmir Y
	for(int j = 0; j < 5; j++) {
		int sum = 0;
		
		for(int i = 16; i <=30; i++) {
			if((i-16)/3 != j) {
				sum += 	maxFlow->getCap()[i][j+31];	
			}
		}
		std::cout << "Estado " << j << " Compro " << sum << "\n";
	}
}
