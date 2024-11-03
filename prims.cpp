//Nicholas Guidice

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Structure to represent an edge
struct Edge {
    int vertex; // The destination vertex of the edge
    int weight; // The weight (cost) of the edge

    Edge(int v, int w) : vertex(v), weight(w) {}
};

// Class representing the graph using an adjacency list
class Graph {
public:
    int numVertices; // Total number of vertices in the graph
    vector<vector<Edge>> adjList; // Adjacency list to store edges

    Graph(int vertices) : numVertices(vertices) {
        adjList.resize(vertices); // Resize the adjacency list to the number of vertices 
    }

    // Function to add an edge to the graph
    void addEdge(int u, int v, int weight) {
        adjList[u].emplace_back(v, weight); // Add an edge from vertex u to vertex v with the specified weight
        adjList[v].emplace_back(u, weight); // Since the graph is undirected, add edge from v to u as well
    }
};

// Class for a priority queue using a binary heap
class PriorityQueue {
private:
    vector<pair<int, int>> heap; // Min-heap to store pairs of (weight, vertex)
    vector<int> verIndex; // Index to keep track of vertices in the heap

public:
    PriorityQueue(int size) {
        verIndex.resize(size, -1); // Initialize all vertex indices to -1 (not in the heap)
    }

    // Function to push a vertex into the priority queue
    void push(int vertex, int weight) {
        if (verIndex[vertex] != -1) {
            decreaseKey(vertex, weight);
            return;
        }

        heap.emplace_back(weight, vertex);
        verIndex[vertex] = heap.size() - 1; // Store the index of the vertex
        percolateUp(heap.size() - 1); // Maintain heap properties 
    }

    // Function to pop the vertex with the minimum weight
    pair<int, int> pop() {
        if (heap.empty()) return make_pair(-1, -1);

        pair<int, int> minElement = heap[0];
        verIndex[minElement.second] = -1; // Mark the vertex as not in the heap
        heap[0] = heap.back(); // Move the last element to the root
        heap.pop_back();

        if (!heap.empty()) {
            verIndex[heap[0].second] = 0;
            percolateDown(0);
        }
        return minElement; // Return the minimum element
    }

    // Check if the priority queue is empty
    bool isEmpty() const {
        return heap.empty();
    }

private:
    // Percolate down the element at index index
    void percolateDown(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;
            if (left < size && heap[left].first < heap[smallest].first)
                smallest = left;
            if (right < size && heap[right].first < heap[smallest].first)
                smallest = right;
            if (smallest == index) break;

            swap(heap[index], heap[smallest]);
            verIndex[heap[index].second] = index;
            verIndex[heap[smallest].second] = smallest;
            index = smallest;
        }
    }

    // Percolate up the element at index index
    void percolateUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index].first >= heap[parent].first) break;
            swap(heap[index], heap[parent]);
            verIndex[heap[index].second] = index;
            verIndex[heap[parent].second] = parent;
            index = parent;
        }
    }

    // Decrease the key of a vertex
    void decreaseKey(int vertex, int newWeight) {
        int index = verIndex[vertex];
        if (index != -1 && newWeight < heap[index].first) {
            heap[index].first = newWeight;
            percolateUp(index);
        }
    }
};

//Prim's algorithm implementation
int prims(Graph& graph) {

    vector<bool> MST(graph.numVertices, false); //Track vertices in MST
    PriorityQueue pq(graph.numVertices);

    int numWeight = 0;

    //Start from vertex 0
    pq.push(0, 0);
    while (!pq.isEmpty()) {
        pair<int, int> minElement = pq.pop();
        int currentVertex = minElement.second;
        if (!MST[currentVertex]) { //If already included in MST skip
        MST[currentVertex] = true;

        numWeight += minElement.first; //Add the weight of the edge to the total weight
        //Iterate over each neighbor of the current vertex
        for (const Edge& edge : graph.adjList[currentVertex]) {   
            if (!MST[edge.vertex]) {
                pq.push(edge.vertex, edge.weight); //Add the edge to the priority queue if the vertex is not in MST
            }
        }
    }
 }
    return numWeight; //Return the total weight of the minimum spanning tree
}

int main() {
    int numVertices, numEdges; //Read the number of vertices and edges from input
    cin >> numVertices >> numEdges;

    Graph graph(numVertices); //Create Graph with given amount of vertices

    for (int i = 0; i < numEdges; ++i) //Read each edge and add it to the graph
    { 
        int weight;
        int u;
        int v;
        cin >> u >> v >> weight;
        graph.addEdge(u, v, weight); }
    
    //Call Prim's algorithm
    int mstNum = prims(graph);
    
    //Print the total length of the minimum spanning tree
    cout << mstNum << "\n";

    return 0;
}
