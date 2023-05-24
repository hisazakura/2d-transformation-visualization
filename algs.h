#pragma once

#include <vector>

using namespace std;

vector<vector<int>> matmul3x3(vector<vector<int>> A, vector<vector<int>> B) {
    vector<vector<int>> C(3, vector<int>(3, 0));

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

vector<int> matmul3x1(vector<vector<float>> A, vector<int> B) {
    vector<int> C(3);

    // Check that the matrices have the correct dimensions
    if (A.size() != 3 || A[0].size() != 3 || B.size() != 3) {
        return C;
    }

    // Perform matrix multiplication
    for (int i = 0; i < 3; i++) {
        float sum = 0.0f;
        for (int j = 0; j < 3; j++) {
            sum += A[i][j] * (float)B[j];
        }
        C[i] = (int)sum;
    }

    return C;
}

