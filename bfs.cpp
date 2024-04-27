#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace chrono;

class Graph
{
  int V;
  vector<vector<int>> adj;

public:
  Graph(int V)
  {
    this->V = V;
    adj.resize(V);
  }

  void addEdge(int u, int v)
  {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  // Sequential BFS
  void sequentialBFS(int source)
  {
    vector<bool> visited(V, false);
    queue<int> q;
    visited[source] = true;
    q.push(source);

    while (!q.empty())
    {
      int u = q.front();
      q.pop();
      cout << u << " ";

      for (int v : adj[u])
      {
        if (!visited[v])
        {
          visited[v] = true;
          q.push(v);
        }
      }
    }
  }

  // Parallel BFS using OpenMP
  void parallelBFS(int source)
  {
    vector<bool> visited(V, false);
    queue<int> q;
    visited[source] = true;
    q.push(source);

    while (!q.empty())
    {
      int u = -1;

#pragma omp parallel
      {
#pragma omp single
        {
          u = q.front();
          cout << u << " ";
          q.pop();
        }

#pragma omp for nowait
        for (int i = 0; i < adj[u].size(); ++i)
        {
          int v = adj[u][i];
          if (!visited[v])
          {
#pragma omp critical
            {
              if (!visited[v])
              {
                visited[v] = true;
                q.push(v);
              }
            }
          }
        }
      }
    }
  }

  // Sequential DFS
  void sequentialDFSUtil(int v, vector<bool> &visited)
  {
    visited[v] = true;
    cout << v << " ";

    for (int u : adj[v])
    {
      if (!visited[u])
      {
        sequentialDFSUtil(u, visited);
      }
    }
  }

  void sequentialDFS(int source)
  {
    vector<bool> visited(V, false);
    sequentialDFSUtil(source, visited);
  }

  // Parallel DFS using OpenMP
  void parallelDFSUtil(int v, vector<bool> &visited)
  {
    visited[v] = true;
    cout << v << " ";

#pragma omp parallel for
    for (int i = 0; i < adj[v].size(); ++i)
    {
      int u = adj[v][i];
      if (!visited[u])
      {
        parallelDFSUtil(u, visited);
      }
    }
  }

  void parallelDFS(int source)
  {
    vector<bool> visited(V, false);
    parallelDFSUtil(source, visited);
  }
};

int main()
{
  Graph g(6); // Create a tree structure
  g.addEdge(0, 1);
  g.addEdge(0, 2);
  g.addEdge(1, 3);
  g.addEdge(1, 4);
  g.addEdge(2, 4);
  g.addEdge(3, 5);
  g.addEdge(4, 5);

  cout << "Sequential BFS starting from vertex 0: ";
  auto start = high_resolution_clock::now();
  g.sequentialBFS(0);
  auto stop = high_resolution_clock::now();
  auto durationSeqBFS = duration_cast<milliseconds>(stop - start);
  cout << "\nTime taken by Sequential BFS: " << durationSeqBFS.count() << " ms\n";

  cout << "Parallel BFS starting from vertex 0: ";
  start = high_resolution_clock::now();
  g.parallelBFS(0);
  stop = high_resolution_clock::now();
  auto durationParBFS = duration_cast<milliseconds>(stop - start);
  cout << "\nTime taken by Parallel BFS: " << durationParBFS.count() << " ms\n";

  cout << "Sequential DFS starting from vertex 0: ";
  start = high_resolution_clock::now();
  g.sequentialDFS(0);
  stop = high_resolution_clock::now();
  auto durationSeqDFS = duration_cast<milliseconds>(stop - start);
  cout << "\nTime taken by Sequential DFS: " << durationSeqDFS.count() << " ms\n";

  cout << "Parallel DFS starting from vertex 0: ";
  start = high_resolution_clock::now();
  g.parallelDFS(0);
  stop = high_resolution_clock::now();
  auto durationParDFS = duration_cast<milliseconds>(stop - start);
  cout << "\nTime taken by Parallel DFS: " << durationParDFS.count() << " ms\n";

  return 0;
}
