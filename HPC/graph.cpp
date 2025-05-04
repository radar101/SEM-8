// Online C++ compiler to run C++ program online
#include<bits/stdc++.h>
#include<omp.h>
#include<chrono>
using namespace std;
using namespace chrono;

class Graph {
    int V;
    vector<vector<int>> adj;
    public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    void sequentialBFS(int start)
    {
        vector<bool> vis(V, false);
        queue<int>q;
        q.push(start);
        vis[start] = true;
        while(!q.empty())
        {
            int top = q.front();
            q.pop();
            cout<<top<<" ";
            for(auto i : adj[top])
            {
                if(!vis[i])
                {
                    vis[i] = true;
                    q.push(i);
                }
            }
        }
        cout<<endl;
    }
    
    void parallelBFS(int start)
    {
        vector<bool> vis(V, false);
        queue<int> q;
        q.push(start);
        vis[start] = true;
        
        while(!q.empty())
        {
            int size = q.size();
            vector<int> frontier;
            #pragma omp parallel for shared(q, vis, frontier)
            for(int i=0;i<size;i++)
            {
                int node;
                #pragma omp critical
                {
                    node = q.front();
                    q.pop();
                    cout<<node<<" ";
                }
                
                for(auto neighbor: adj[node])
                {
                    if(!vis[neighbor])
                    {
                        #pragma omp critical
                        {
                            if(!vis[neighbor])
                            {
                                vis[neighbor] = true;
                                frontier.push_back(neighbor);
                            }
                        }
                    }
                }
            }
            for (int node : frontier)
                q.push(node);
        }
        cout<<endl;
    }
    
    void sequentialDFS(int node, vector<bool> &vis)
    {
        vis[node] = true;
        cout<<node<<" ";
        for(auto it : adj[node])
        {
            if(!vis[it])
            {
                sequentialDFS(it, vis);
            }
        }
    }
    void parallelDFS(int node, vector<bool> &vis)
    {
        vis[node] = true;
        cout<<node<<" ";
        
        #pragma omp parallel for
        for(auto it : adj[node])
        {
            if(!vis[it])
            {
                #pragma omp critical
                if(!vis[it])
                {
                    parallelDFS(it, vis);
                }
            }
        }
    }
    
    void sequentialDFSUtil(int start)
    {
        vector<bool> vis(V, false);
        sequentialDFS(start, vis);
        cout<<endl;
    }
    
    void parallelDFSUtil(int start)
    {
        vector<bool> vis(V, false);
        parallelDFS(start, vis);
        cout<<endl;
    }
    
    void measureperformance(int start)
    {
        auto start_time = high_resolution_clock::now();
        sequentialBFS(start);
        auto end_time = high_resolution_clock::now();
        cout << "Time for Sequential BFS: " << duration_cast<microseconds>(end_time - start_time).count() << " ms\n";
        
        start_time = high_resolution_clock::now();
        parallelBFS(start);
        end_time = high_resolution_clock::now();
        cout<<"Time for Parallel BFS: "<<duration_cast<microseconds>(end_time - start_time).count()<<"ms\n";
        
        start_time = high_resolution_clock::now();
        sequentialDFSUtil(start);
        end_time = high_resolution_clock::now();
        cout<<"Time for Sequential DFS: "<<duration_cast<microseconds>(end_time - start_time).count()<<"ms\n";
        
        start_time = high_resolution_clock::now();
        parallelDFSUtil(start);
        end_time = high_resolution_clock::now();
        cout<<"Time for parallel DFS: "<<duration_cast<microseconds>(end_time - start_time).count()<<"ms\n";
    }
};



int main()
{
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    
    g.measureperformance(0);
}

/*
Q1. What is the difference between BFS and DFS?
A: BFS explores neighbors level by level using a queue, while DFS explores as far as possible along each branch using a stack.

Q2. Why use OpenMP in BFS/DFS?
A: OpenMP enables parallel traversal of graph nodes, speeding up exploration of large graphs by processing multiple nodes or neighbors simultaneously.

Q3. Is BFS easier to parallelize than DFS?
A: Yes, because BFS inherently processes all nodes at the current level simultaneously, making it suitable for data-parallel execution.

Q4. What are the challenges in parallel DFS?
A: DFS is recursive and depends on deep exploration, making it difficult to divide tasks evenly without complex synchronization.

Q5. What does #pragma omp parallel for do?
A: It tells the compiler to parallelize the following for loop using multiple threads.

Q6. What is a critical section in OpenMP?
A: A block of code that must be executed by only one thread at a time to avoid race conditions.

Q7. What performance metrics did you measure?
A: Execution time (in milliseconds) for both sequential and parallel BFS/DFS.

Q8. Can parallel BFS/DFS be faster in all cases?
A: Not necessarily. For small graphs, overhead of thread management might result in slower performance.

Q9. How do you ensure correctness in parallel graph traversal?
A: By using synchronization techniques like #pragma omp critical to safely update shared data (e.g., visited nodes).

Q10. What types of graphs can be used with BFS/DFS?
A: Both BFS and DFS can work on directed, undirected, and weighted graphs (though weight is ignored in plain traversal).
*/



