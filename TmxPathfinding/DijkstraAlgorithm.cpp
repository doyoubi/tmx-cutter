#include "DijkstraAlgorithm.h"
#include "debug.h"

namespace dyb
{
    const float DijkstraAlgorithm::inf = std::numeric_limits<float>::infinity();

    DijkstraAlgorithm::DijkstraAlgorithm(int nodeNumber)
        : nodeNum(nodeNumber), adjacency_matrix(nodeNumber, nodeNumber),
        path(nodeNumber), pathLen(nodeNumber), tag(nodeNumber)
    {
        DEBUGCHECK(nodeNumber > 1, "nodeNumber should be larger than one");
    }

    DijkstraAlgorithm::DijkstraAlgorithm(Graph && otherGraph)
        : DijkstraAlgorithm(otherGraph.get_width())
    {
        adjacency_matrix = std::move(otherGraph);
    }

    DijkstraAlgorithm::DijkstraAlgorithm(const Graph & otherGraph)
        : DijkstraAlgorithm(otherGraph.get_width())
    {
        adjacency_matrix = otherGraph;
    }


    int DijkstraAlgorithm::getNodeNumber() const
    {
        return nodeNum;
    }

    void DijkstraAlgorithm::setAdjacencyMatrix(std::initializer_list<float> adjancencyMatrix)
    {
        adjacency_matrix.set_data(adjancencyMatrix);
        adjacency_matrix.transpose();
    }

    const vector<int> & DijkstraAlgorithm::getPathResult(int terminal) const
    {
        DEBUGCHECK(terminal != startNode, "getPathResult: ternimal node equal to start node");
        return resultPath[terminal];
    }

    float DijkstraAlgorithm::getPathLengthResult(int terminal) const
    {
        DEBUGCHECK(terminal != startNode, "getPathLengthResult: ternimal node equal to start node");
        return resultPathLen[terminal];
    }

    void DijkstraAlgorithm::clearResult()
    {
        startNode = -1;
        path = vector<vector<int>>(nodeNum);
        pathLen = vector<float>(nodeNum);
        tag = vector<int>(nodeNum, 0);

        resultPath = vector<vector<int>>(nodeNum);
        resultPathLen = vector<float>(nodeNum, inf);
    }

    void DijkstraAlgorithm::run(int start)
    {
        // clear result 
        // AND let the sizes of path, pathLen, tag, resultPath, resultPathLen become nodeNum,
        // initialize the element of resultPathLen to inf, tag to 0
        clearResult();
        startNode = start;

        tag[start] = 1;
        for (int i = 0; i < nodeNum; i++)
        {
            if (i == start) continue;
            pathLen[i] = adjacency_matrix[i][start];
            // require adjacency_matrix use DijkstraAlgorithm::inf to represent infinite value
            // and pathLen[i] < inf is true only if pathLen[i] == inf
            if (pathLen[i] < inf)
            {
                path[i].push_back(start);
                path[i].push_back(i);
            }
        }
        for (int ct = 1; ct < nodeNum; ct++) // nodeNum - 1 left
        {
            // find smallest
            int index = -1;
            float smallest = inf;
            for (int i = 0; i < nodeNum; i++)
            {
                if (!tag[i] && pathLen[i] < smallest)
                {
                    index = i;
                    smallest = pathLen[i];
                }
            }
            if (index == -1) { cout << "can't find path any more" << endl; return; }
            tag[index] = 1;
            resultPathLen[index] = pathLen[index];
            resultPath[index] = path[index];

            for (int i = 0; i < nodeNum; i++)
            {
                if (tag[i]) continue;
                if (adjacency_matrix[i][index] == inf) continue;
                if (smallest + adjacency_matrix[i][index] >= pathLen[i]) continue;
                pathLen[i] = smallest + adjacency_matrix[i][index];
                path[i] = path[index];
                path[i].push_back(i);
            }
        }
    }

}
