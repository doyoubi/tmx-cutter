#ifndef DYB_DIJKSTRA_ALGORITHM
#define DYB_DIJKSTRA_ALGORITHM

#include <vector>
#include <initializer_list>
#include <limits>
#include "../dyb/2d/array2d.h"

namespace dyb
{
    using std::vector;

    class DijkstraAlgorithm
    {
    public:
        DijkstraAlgorithm(int nodeNumber);
        int getNodeNumber() const;

        // should use DijkstraAlgorithm::inf to represent infinite
        void setAdjacencyMatrix(std::initializer_list<float> adjancencyMatrix); // row-major order

        void run(int start);

        const vector<int> & getPathResult(int terminal) const;
        float getPathLengthResult(int terminal) const;

        static const float inf/* = std::numeric_limits<float>::infinity()*/;

    private:
        void clearResult();

        const int nodeNum;

        typedef dyb::array2d<float> mat;
        mat adjacency_matrix;

        int startNode; // ternimal node should not be equal to start node
        // used in getPathResult, getPathLengthResult

        vector<float> pathLen;
        vector<vector<int>> path;
        vector<int> tag;

        vector<float> resultPathLen;
        vector<vector<int>> resultPath;
    };
}

#endif