#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <algorithm>
#include <climits>

using namespace std;

class Graph
{
private:
    vector<vector<pair<int, int>>> ways;
    vector<vector<int> > table;
    bool* b;
    int point;
    int edge ;
    int s, f;
public:
    Graph(/* args */) : b(nullptr), point(0), edge(0) {};
    Graph(ifstream& in, int type_of_input = 1);
    Graph(istream& in, int type_of_input = 1);

    ~Graph();

    //int weight();
    pair<int, vector<int>> path();
    void print();
};

Graph::Graph(ifstream& in, int type_of_input){
    if (type_of_input == 1)
    {
        in >> point >> s >> f;
        edge = 0;

        //cout << "Point: " << point << endl;

        ways.resize(point+1);
        table.resize(point);
        for (auto &i : table)
        {
            i.resize(point);
        }

        for (int i = 0; i < point; i++)
        {
            for (int j = 0; j < point; j++)
            {
                in >> table[i][j];
                //cout << table[i][j] << " ";
                if (table[i][j] >= 0 && i != j)
                {
                    ways[i+1].push_back({j+1, table[i][j]});
                    edge++;
                }
            }
            //cout << endl;
        }
    }
    else
    {
        in >> point >> edge;
        ways.resize(point+1);
        table.resize(point);
        for (auto &i : table)
        {
            i.resize(point);
        }
            int u, v, w;
        while (in >> u >> v >> w)
        {
            ways[u].push_back({v, w});
            table[u-1][v-1] = w;
        }
    }
    
    b = new bool[point+1];
    for (int i = 0; i < point+1; i++)
    {
        b[i] = false;
    }
};

Graph::Graph(istream& in, int type_of_input){
    if (type_of_input == 1)
    {
        in >> point >> s >> f;
        edge = 0;

        ways.resize(point+1);
        table.resize(point);
        for (auto &i : table)
        {
            i.resize(point);
        }

        for (int i = 0; i < point; i++)
        {
            for (int j = 0; j < point; j++)
            {
                in >> table[i][j];
                if (table[i][j] >= 0 && i != j)
                {
                    ways[i+1].push_back({j+1, table[i][j]});
                    edge++;
                }
            }
        }
    }
    else
    {
        in >> point >> edge;
        ways.resize(point+1);
        table.resize(point);
        for (auto &i : table)
        {
            i.resize(point);
        }
        int u, v, w;
        while (in >> u >> v >> w)
        {
            ways[u].push_back({v, w});
            table[u-1][v-1] = w;
        }
    }

    b = new bool[point+1];
    for (int i = 0; i < point+1; i++)
    {
        b[i] = false;
    }
};

void Graph::print()
{
    for (int i = 0; i < point; i++)
    {
        cout << i+1 << ": ";
        for (int j = 0; j < point; j++)
        {
            cout << table[i][j] << " ";
        }
        cout << endl ;
    }
    cout << endl;
    for (int i = 1; i <= point; i++)
    {
        
        cout << "Way: " << i << ": ";
        for (const auto &edge : ways[i])
        {
            cout << "(" << edge.first << ", " << edge.second << ") ";
        }
        cout << endl;
    }
    cout << endl;
}

pair<int, vector<int>> Graph::path()
{
    vector<int> dist(point+1, INT_MAX);
    vector<int> parent(point+1, -1);
    vector<bool> used(point+1, false);

    dist[s] = 0;

    for (int iter = 1; iter <= point; iter++)
    {
        int u = -1;
        int best = INT_MAX;
        for (int v = 1; v <= point; v++)
        {
            if (!used[v] && dist[v] < best)
            {
                best = dist[v];
                u = v;
            }
        }

        if (u == -1 || best == INT_MAX) 
            break;

        used[u] = true;

        for (const auto &edge : ways[u])
        {
            int v = edge.first;
            int w = edge.second;
            if (!used[v] && dist[u] != INT_MAX && dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                parent[v] = u;
            }
        }
    }

    if (dist[f] == INT_MAX)
        return {-1, vector<int>()};

    vector<int> way;
    for (int v = f; v != -1; v = parent[v])
        way.push_back(v);
    reverse(way.begin(), way.end());

    return {dist[f], way};
}

Graph::~Graph()
{
    delete[] b;
}

bool checkfile(ifstream&);

int main(int argc, char const *argv[])
{
    Graph g(cin, 1);
    auto result = g.path();
    if (result.first == -1)
    {
        cout << -1;
        return 0;
    }

    const vector<int>& way = result.second;
    for (size_t i = 0; i < way.size(); i++)
    {
        if (i)
            cout << " ";
        cout << way[i];
    }
    return 0;
}



bool checkfile(ifstream &in)
{
    if (!in.is_open())
    {
        cout << "Error opening file" << endl;
        return false;
    }
    return true;
}
