#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <limits>
#include <chrono>

using namespace std;

class Graph {
private:
    int n; // число вершин
    vector<vector<pair<int, int>>> adj; // список смежности: v -> (to, weight)

public:
    Graph() : n(0) {}

    // ---------- ЧТЕНИЕ ГРАФА ----------
    void readGraph(const string& filename) {
        ifstream in(filename);
        if (!in.is_open()) {
            cerr << "Cannot open file: " << filename << endl;
            return;
        }

        int m;
        in >> n >> m;
        adj.assign(n, {});

        for (int i = 0; i < m; i++) {
            int u, v, w;
            in >> u >> v >> w;
            adj[u].push_back({ v, w });
            adj[v].push_back({ u, w }); // неориентированный
        }

        in.close();
    }

    // ---------- АЛГОРИТМ ПРИМА ----------
    Graph getSpaingTreePrima() {
        Graph mst;
        mst.n = n;
        mst.adj.assign(n, {});

        vector<int> key(n, numeric_limits<int>::max());
        vector<int> parent(n, -1);
        vector<bool> used(n, false);

        key[0] = 0; // начинаем с вершины 0

        // (ключ, вершина)
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({ 0, 0 });
        int check_edges = 0;
        while (!pq.empty()) {
            int v = pq.top().second;
            pq.pop();

            if (used[v]) continue;
            used[v] = true;

            for (auto& edge : adj[v]) {
                check_edges++;
                int to = edge.first;
                int w = edge.second;

                if (!used[to] && w < key[to]) {
                    key[to] = w;
                    parent[to] = v;
                    pq.push({ key[to], to });
                }
            }
        }
        cout << "Count checked edges: " << check_edges << endl;

        // формируем MST
        for (int v = 1; v < n; v++) {
            int u = parent[v];
            if (u != -1) {
                int w = key[v];
                mst.adj[u].push_back({ v, w });
                mst.adj[v].push_back({ u, w });
            }
        }

        return mst;
    }

    // ---------- ПРЕОБРАЗОВАНИЕ В СПИСОК СМЕЖНОСТИ ----------
    void transformToAdjList() {
        // adj уже список смежности — ничего не делаем
    }

    // ---------- ЗАПИСЬ ГРАФА ----------
    void writeGraph(const string& filename) {
        ofstream out(filename);
        if (!out.is_open()) {
            cerr << "Cannot open file: " << filename << endl;
            return;
        }

        int m = 0;
        for (int i = 0; i < n; i++)
            m += adj[i].size();
        m /= 2; // неориентированный

        out << n << " " << m << "\n";

        vector<vector<bool>> used(n, vector<bool>(n, false));

        for (int u = 0; u < n; u++) {
            for (auto& p : adj[u]) {
                int v = p.first;
                int w = p.second;
                if (!used[u][v]) {
                    out << u << " " << v << " " << w << "\n";
                    used[u][v] = used[v][u] = true;
                }
            }
        }

        out.close();
    }
};

// ------------------ MAIN ------------------

int main()
{ Graph g;
g.readGraph("MST02in.txt");
auto start = chrono::high_resolution_clock::now();

//g.readGraph("input.txt");
//Graph gg=g.getSpaingTreeBoruvka();
//Graph gg = g.getSpaingTreeKruscal();
Graph gg = g.getSpaingTreePrima();

auto end = chrono::high_resolution_clock::now();
auto time = chrono::duration_cast<chrono::microseconds>(end - start);
cout << "Time: " << time.count() << " microseconds" << endl;

gg.transformToAdjList();
gg.writeGraph("output.txt");
return 0;
}