// bonus.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <tuple>
#include <string>
#include <sstream>
#include <chrono>
using namespace std;

class Graph {
private:
	int N; // |V|
	int M; // |E|
	int D; // ориентированный/нет
	int W; //взвешенный/нет
	char view; // представление графа (матрица смежности C, список смежности L, список рёбер E)

	//vector<vector<int>> matrix_adjacency; // Матрица смежности
	vector<vector<pair<int, int>>> list_adjacency; // Список смежности 
	//vector<tuple<int, int, int>> list_edges; // Список рёбер

public:
	Graph() { //конструктор
		N = 0;
		M = 0;
		D = 0;
		W = 0;
		view = 'N';
	}
	Graph(int countVertices) {
		N = countVertices;
		M = 0;
		D = 0;
		W = 1;
		view = 'L';

		list_adjacency.resize(N);
	}
	void addEdge(int from, int to, int weight) {
		list_adjacency[from].push_back({ to, weight });
		if (D == 0) {
			list_adjacency[to].push_back({ from, weight });
		}
		M++;
	}
	void transformToAdjList() {
		view = 'L';
	}
	void readGraph(string fileName) {
		ifstream fin(fileName);
		if (!fin.is_open()) {
			cout << "file not found: " << fileName << endl;
			return;
		}
		fin >> view;
		switch (view) {
			case 'C': { //матрица смежности
				fin >> N  >> D >> W;
				list_adjacency.clear();
				list_adjacency.resize(N);
				for (int i=0; i<N; i++) {
					for (int j=0; j<N; j++) {
						int weight=0;
						fin >> weight;
						if (weight!=0) list_adjacency[i].push_back({ j, weight });
					}
				}
				break;
			}
			case 'L': { //список смежности
				fin >> N >> D >> W;
				list_adjacency.clear();
				list_adjacency.resize(N);
				string line;
				getline(fin, line);
				for (int i = 0; i < N; i++) {
					getline(fin, line);
					stringstream ss(line);
					int to, weight;
					while (ss >> to >> weight) {
						list_adjacency[i].push_back({ to - 1, weight });
					}
				}
				break;
			}
			case 'E': {
				fin >> N >> M >> D >> W;
				list_adjacency.clear();
				list_adjacency.resize(N);
				int countEdges = M;
				M = 0;
				for (int i = 0; i < countEdges; i++) {
					int from, to, weight;
					fin >> from >> to >> weight;
					addEdge(from - 1, to - 1 , weight);
				}
				break;
			}
		}
		fin.close();
	}
	/*void writeGraph(string fileName) {
		ofstream fout(fileName);
		fout << view << " " << N << endl;
		fout << D << " " << W << endl;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < list_adjacency[i].size(); j++) {
				fout << list_adjacency[i][j].first + 1 << " "
					<< list_adjacency[i][j].second << " ";
			}
			fout << endl;
		}
		fout.close();
	}*/
	void writeGraph(string fileName) {
		ofstream fout(fileName);
		int sum = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < list_adjacency[i].size(); j++) {
				sum += list_adjacency[i][j].second;
			}
		}
		if (D == 0) {
			sum /= 2;
		}
		fout << sum;
		fout.close();
	}
	Graph getSpaingTreePrima() {
		Graph spanningTree(N);//граф на выход
		spanningTree.D = 0;
		spanningTree.W = 1;
		spanningTree.view = 'L';
		vector<bool> new_v(N, false);//флаги новых вершин
		if (N == 0) {
			return spanningTree;
		}
		new_v[0] = true;//флаг первая вершина
		int check_edges = 0;
		for (int i = 0; i < N - 1; i++) {
			pair<int, int> min_edge_pair; // {to, weight} для мин
			int from_min = 0; // from для мин 
			bool v_found = false;// флаг найденного мин 

			for (int j = 0; j < N; j++) {//проход по вершинам
				if (new_v[j]) {//если вершина в new_v, то проходим по её рёбрам
					for (int k = 0; k < list_adjacency[j].size(); k++) {//ищем вершину с мин ребром
						check_edges++;
						if (!new_v[list_adjacency[j][k].first] &&
							(!v_found || min_edge_pair.second > list_adjacency[j][k].second)) {
							//если вершина не в new_v и мин не найден или вес мин больше текущего ребра, то обновляем мин
							min_edge_pair = list_adjacency[j][k];
							from_min = j;
							v_found = true;
						}
					}
				}
			}
			if (!v_found) {
				break;
			}
			new_v[min_edge_pair.first] = true;//подняли флаг для новой вершины
			spanningTree.addEdge(from_min, min_edge_pair.first, min_edge_pair.second);//добавляем мин ребро в ост дерево
		}
		cout << "Count checked edges: " << check_edges << endl;
		return spanningTree;
	}
};

int main()
{
	Graph g;
	g.readGraph("MST02in.txt");
	auto start = chrono::high_resolution_clock::now();

	//g.readGraph("input.txt");
	//Graph gg=g.getSpaingTreeBoruvka();
	//Graph gg = g.getSpaingTreeKruscal();
	Graph gg=g.getSpaingTreePrima();

	auto end = chrono::high_resolution_clock::now();
	auto time = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "Time: " << time.count() << " microseconds" << endl;

	gg.transformToAdjList();
	gg.writeGraph("output.txt");
	return 0;
}
