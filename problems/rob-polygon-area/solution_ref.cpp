#include <vector>
#include <cmath>
#include <utility>

double polygonArea(std::vector<std::pair<double,double>> vertices) {
    int n = vertices.size();
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        int j = (i+1) % n;
        sum += vertices[i].first * vertices[j].second;
        sum -= vertices[j].first * vertices[i].second;
    }
    return std::fabs(sum) / 2.0;
}
