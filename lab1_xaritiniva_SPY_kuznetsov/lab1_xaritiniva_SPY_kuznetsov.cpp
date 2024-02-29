#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <climits>

using namespace std;

// Structure to represent a vertex in the graph
struct Vertex {
    int earliestStart = 0; // Earliest start time of the task
    int latestStart = INT_MAX; // Latest start time of the task
    vector<int> nextVertices; // List of vertices that can be reached from this vertex
    vector<int> prevVertices; // List of vertices that precede this vertex
};

// Global variables to store graph information
vector<Vertex> graph; // Adjacency list representation of the graph
vector<int> durations; // Duration of tasks
vector<int> inDegree; // In-degree of vertices
stack<int> order;     // Stack to store the topological order of vertices
vector<int> topologicalOrder; // Topological order of vertices
vector<int> criticalPath; // Vertices in the critical path
vector<int> slackTimes; // Slack time for each vertex

// Function to read the graph from the user
void readGraph(int n) {
    graph.clear();
    graph.resize(n);
    inDegree.resize(n, 0);

    cout << "Enter the adjacency matrix (0 or 1) for the directed graph:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int edge;
            cin >> edge;
            if (edge == 1) {
                graph[i].nextVertices.push_back(j);
                inDegree[j]++;
                graph[j].prevVertices.push_back(i);
                // Adding reverse edges for backtracking in later stages
            }
        }
    }
}

// Function to read task durations from the user
void readDurations(int n) {
    durations.clear();
    durations.resize(n);

    cout << "Enter the durations of tasks:\n";
    for (int i = 0; i < n; ++i) {
        cin >> durations[i];
    }
}

// Function to find the critical path of the graph
void findCriticalPath(int n) {
    queue<int> zeroInDegree;
    for (int i = 0; i < n; ++i) {
        if (inDegree[i] == 0) {
            zeroInDegree.push(i);
        }
    }

    // Forward pass to calculate earliest start times
    while (!zeroInDegree.empty()) {
        int u = zeroInDegree.front();
        zeroInDegree.pop();
        topologicalOrder.push_back(u);

        // Iterating over each next vertex of the current vertex 'u'
        for (int v : graph[u].nextVertices) {
            // Decrement the in-degree of the next vertex 'v' since 'u' is being processed
            inDegree[v]--;

            // If in-degree becomes zero after decrement, add it to the queue
            if (inDegree[v] == 0) {
                zeroInDegree.push(v);
            }

            // Update the earliest start time of 'v' based on the duration of 'u'
            graph[v].earliestStart = max(graph[v].earliestStart, graph[u].earliestStart + durations[u]);
        }
    }

    int maxEarliestStart = 0;
    for (int i = 0; i < n; ++i) {
        if (graph[i].nextVertices.empty()) {
            maxEarliestStart = max(maxEarliestStart, graph[i].earliestStart);
            graph[i].latestStart = maxEarliestStart;
        }
    }

    // Backward pass to calculate latest start times
    for (auto it = topologicalOrder.rbegin(); it != topologicalOrder.rend(); ++it) {
        int u = *it;
        if (graph[u].nextVertices.empty()) {
            graph[u].latestStart = maxEarliestStart;
        }
        for (int v : graph[u].nextVertices) {
            graph[u].latestStart = min(graph[u].latestStart, graph[v].latestStart - durations[u]);
        }
        for (int prev : graph[u].prevVertices) {
            graph[prev].latestStart = min(graph[prev].latestStart, graph[u].latestStart - durations[prev]);
        }
    }

    criticalPath.clear();
    slackTimes.clear();
    slackTimes.resize(n, 0);
    for (int i = 0; i < n; ++i) {
        if (graph[i].earliestStart == graph[i].latestStart) {
            criticalPath.push_back(i);
        }
        else {
            slackTimes[i] = graph[i].latestStart - graph[i].earliestStart;
        }
    }

    // Output the results
    cout << "Length of the critical path: " << maxEarliestStart << "\n";
    cout << "Vertices in the critical path: ";
    for (int vertex : criticalPath) {
        cout << vertex << " ";
    }
    cout << "\nSlack time for non-critical path vertices: ";
    for (int slackTime : slackTimes) {
        cout << slackTime << " ";
    }
    cout << "\n";
}

int main() {
    setlocale(LC_ALL, "ru");
    int n;
    cout << "Enter the number of vertices in the graph: ";
    cin >> n;

    // Read graph and task durations
    readGraph(n);
    readDurations(n);

    // Find and display the critical path
    findCriticalPath(n);

    // Output critical path and slack times
    cout << "Critical Path: ";
    for (int v : criticalPath) {
        cout << v << " ";
    }
    cout << "\n";

    cout << "Slack Times: ";
    for (int i = 0; i < n; ++i) {
        cout << slackTimes[i] << " ";
    }
    cout << "\n";

    return 0;
}
