#include <fstream>
#include <limits>
#include <queue>
#include <unordered_map>
#include "graph_matrix.h"

using namespace std;

// Function to calculate the shortest path from a given start node
template <typename TYPE>
std::unordered_map<TYPE, std::pair<int, TYPE>> calculateShortestPaths(GraphMatrix<TYPE> &graph, TYPE startNode)
{
    int numNodes = graph.getNodeCount();
    std::unordered_map<TYPE, std::pair<int, TYPE>> shortestPaths;

    // Initialize distances to all nodes as infinity
    std::unordered_map<TYPE, int> distances;
    for (int i = 0; i < numNodes; ++i)
    {
        TYPE node = graph.getNodeAtIndex(i);
        distances[node] = std::numeric_limits<int>::max();
    }

    // Set distance to the start node as 0
    distances[startNode] = 0;

    // Priority queue to store nodes with their distances
    std::priority_queue<std::pair<int, TYPE>, std::vector<std::pair<int, TYPE>>, std::greater<std::pair<int, TYPE>>> pq;
    pq.push({0, startNode});

    while (!pq.empty())
    {
        TYPE currentNode = pq.top().second;
        int currentDistance = pq.top().first;
        pq.pop();

        // Iterate through neighbors of the current node
        for (int i = 0; i < numNodes; ++i)
        {
            TYPE neighborNode = graph.getNodeAtIndex(i);
            int weight = graph.getLink(currentNode, neighborNode);

            // Relaxation step
            if (currentDistance + weight < distances[neighborNode])
            {
                distances[neighborNode] = currentDistance + weight;
                pq.push({distances[neighborNode], neighborNode});
                shortestPaths[neighborNode] = {currentDistance + weight, currentNode};
            }
        }
    }

    // Handle the case where the starting node is not connected to any other nodes
    if (shortestPaths.find(startNode) == shortestPaths.end())
    {
        shortestPaths[startNode] = {0, startNode};
    }

    return shortestPaths;
}

// Function to print the shortest paths from the start node
template <typename TYPE>
void printShortestPaths(GraphMatrix<TYPE> &graph, TYPE startNode)
{
    auto shortestPaths = calculateShortestPaths(graph, startNode);

    for (const auto &entry : shortestPaths)
    {
        TYPE node = entry.first;
        int distance = entry.second.first;
        TYPE previousNode = entry.second.second;

        // Print the shortest path to the current node
        cout << node << ": " << distance;
        if (node != startNode)
        {
            cout << "(" << previousNode << ")";
        }
        cout << ", ";
    }
    cout << endl;
}

int main()
{
    // Read graph from file
    ifstream inputFile("graph.txt");
    if (!inputFile.is_open())
    {
        cout << "Error opening file." << endl;
        return 1;
    }

    // Read node labels
    string nodeLabels;
    getline(inputFile, nodeLabels);

    // Create a graph with node labels
    GraphMatrix<char> graph;
    for (char label : nodeLabels)
    {
        graph.addNode(label);
    }

    // Read matrix values
    for (int i = 0; i < graph.getNodeCount(); ++i)
    {
        for (int j = 0; j < graph.getNodeCount(); ++j)
        {
            int weight;
            inputFile >> weight;
            graph.addLink(i, j, weight);
        }
    }

    // Close the file
    inputFile.close();

    // Print the adjacency matrix
    cout << "Adjacency Matrix:" << endl;
    graph.printAdjacencyMatrix();
    cout << endl;

    // Prompt the user to select a starting node
    cout << "Select a starting node: ";
    char startNode;
    cin >> startNode;

    // Calculate and print shortest paths from the selected node
    cout << "Shortest Paths:" << endl;
    printShortestPaths(graph, startNode);

    return 0;
}
