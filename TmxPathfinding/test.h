#include <iostream>
#include <algorithm>
#include "DijkstraAlgorithm.h"
#include "../dyb/debug.h"
using std::cout;
using std::endl;
using dyb::debugCheck;


#define TEST(expression) DEBUGCHECK(expression, "test failed")
void test()
{
    using std::begin;
    using std::end;
    using std::equal;
    using namespace dyb;

    dyb::DijkstraAlgorithm dij(6);
    float inf = dyb::DijkstraAlgorithm::inf;
    dij.setAdjacencyMatrix({
        inf, inf, 10, inf, 30, 100,
        inf, inf, 5, inf, inf, inf,
        inf, inf, inf, 50, inf, inf,
        inf, inf, inf, inf, inf, 10,
        inf, inf, inf, 20, inf, 60,
        inf, inf, inf, inf, inf, inf,
    });

    dij.run(0);
    TEST(dij.getPathLengthResult(1) == inf);
    TEST(dij.getPathLengthResult(2) == 10);
    TEST(dij.getPathLengthResult(3) == 50);
    TEST(dij.getPathLengthResult(4) == 30);
    TEST(dij.getPathLengthResult(5) == 60);
    TEST(equal(begin(dij.getPathResult(1)), end(dij.getPathResult(1)), begin(vector<int>())));
    TEST(equal(begin(dij.getPathResult(2)), end(dij.getPathResult(2)), begin(vector<int>({ 0, 2 }))));
    TEST(equal(begin(dij.getPathResult(3)), end(dij.getPathResult(3)), begin(vector<int>({ 0, 4, 3 }))));
    TEST(equal(begin(dij.getPathResult(4)), end(dij.getPathResult(4)), begin(vector<int>({ 0, 4 }))));
    TEST(equal(begin(dij.getPathResult(5)), end(dij.getPathResult(5)), begin(vector<int>({ 0, 4, 3, 5 }))));

    dij.run(1);
    TEST(dij.getPathLengthResult(0) == inf);
    TEST(dij.getPathLengthResult(2) == 5);
    TEST(dij.getPathLengthResult(3) == 55);
    TEST(dij.getPathLengthResult(4) == inf);
    TEST(dij.getPathLengthResult(5) == 65);
    TEST(equal(begin(dij.getPathResult(0)), end(dij.getPathResult(0)), begin(vector<int>())));
    TEST(equal(begin(dij.getPathResult(2)), end(dij.getPathResult(2)), begin(vector<int>({ 1, 2 }))));
    TEST(equal(begin(dij.getPathResult(3)), end(dij.getPathResult(3)), begin(vector<int>({ 1, 2, 3 }))));
    TEST(equal(begin(dij.getPathResult(4)), end(dij.getPathResult(4)), begin(vector<int>())));
    TEST(equal(begin(dij.getPathResult(5)), end(dij.getPathResult(5)), begin(vector<int>({ 1, 2, 3, 5 }))));

    cout << "all test passed" << endl;
}