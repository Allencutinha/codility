//
// Test executable for Lesson 14 - Binary Search Algorithm/MinMaxDivision.cpp
//
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>

#include "macros.h"

//
// Configure command line arguments
//
#include "gflags/gflags.h"
DEFINE_bool(assert, false, "If set, tests will fail if they do no match the brute force method");
DEFINE_int32(test, 0, "Specific test to run, 0 is all");

using namespace std;

//
// These should match the function signatures in MinMaxDivision.cpp
//
int solution(int, int, vector<int> &);
int bruteForce(int, int, vector<int> &);

//
// Run the actual test
//
void test(int testNum, int runTestNum, int K, int M, vector<int> A, bool assert = false, bool printA = false, bool runBruteForce = true) 
{
    if ( runTestNum && testNum != runTestNum ) {
        return;
    }
    cout << "---- Test " << testNum << " ----" << endl;
    if (printA) {
        PRINT_VECTOR(A);
    }

    PRINT_VAR(K);
    PRINT_VAR(M);

    auto start = chrono::high_resolution_clock::now();

	cout << "calculating solution..." << endl;
    auto answer = solution(K, M, A);

    auto stop = chrono::high_resolution_clock::now();

	PRINT_VAR(answer);

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start); 
    cout << duration.count() << " us" << endl; 

	if ( !runBruteForce ) {
		cout << "skipping brute force answer..." << endl;
		return;
	}

	cout << "calculating brute force answer..." << endl;
   	auto correct = bruteForce(K, M, A);

    cout << endl;
    cout << "Test " << testNum << " Results" << endl;
    cout << endl;

	PRINT_VAR(correct);

    cout << answer << " =? " << correct << endl;

	if (assert) {
		assert( answer == correct );
	}

    // Nice debug statement if we are not asserting
    cout << ((answer == correct) ? " pass " : " *** FAIL *** ") << endl;
}

vector<int> generateRandomSequence(int N, int M)
{
    vector<int> A(N);
    srand(time(0));

    for (int i=0; i<N; i++) {
        A[i] = rand() % (M+1);
    }
    return A;
}

vector<int> generateOneOrMSequence(int N, int M, double prob)
{
    vector<int> A(N);
    srand(time(0));

    for (int i=0; i<N; i++) {
        A[i] = rand() % (M+1);
        ( A[i] < (prob*M) ) ? (A[i] = 1 ) : (A[i]=M);
    }
    return A;
}

//
// Configure tests to run
//
int main(int argc, char **argv) 
{

    gflags::SetUsageMessage("Runs the tests for MinMaxDivision");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if ( argc > 1 ) {
        cerr << "Command line arguments not recognized, please execute:" << endl;
        cerr << "  " << argv[0] << " --help" << endl;
        exit(1);
    }

    cout << "========================================" << endl;
    cout << "==              START TEST            ==" << endl;
    cout << "========================================" << endl;

    // We don't want to print the A vector for all tests (can be quite large)
    // Also, we do want to skip brute force for some tests too (where it is slow)
    bool printA = true;
	int testNum = 1;
    bool runBruteForce = true;

   	test(testNum++, FLAGS_test, 1, 5, {0, 0}, FLAGS_assert, printA, runBruteForce); 
   	test(testNum++, FLAGS_test, 4, 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, FLAGS_assert, printA, runBruteForce); 
   	test(testNum++, FLAGS_test, 4, 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}, FLAGS_assert, printA, runBruteForce); 
   	test(testNum++, FLAGS_test, 1, 5, {2, 1, 5, 1, 2, 2, 2}, FLAGS_assert, printA, runBruteForce); 
   	test(testNum++, FLAGS_test, 2, 5, {2, 1, 5, 1, 2, 2, 2}, FLAGS_assert, printA, runBruteForce); 
   	test(testNum++, FLAGS_test, 3, 5, {2, 1, 5, 1, 2, 2, 2}, FLAGS_assert, printA, runBruteForce); 
   	test(testNum++, FLAGS_test, 4, 5, {2, 1, 5, 1, 2, 2, 2}, FLAGS_assert, printA, runBruteForce); 
   	test(testNum++, FLAGS_test, 5, 5, {2, 1, 5, 1, 2, 2, 2}, FLAGS_assert, printA, runBruteForce); 
   	test(testNum++, FLAGS_test, 20, 5, {0, 5, 0}, FLAGS_assert, printA, runBruteForce); 
   	test(testNum++, FLAGS_test, 4000, 5, {0, 5, 0}, FLAGS_assert, printA, runBruteForce); 
   	test(testNum++, FLAGS_test, 10000, 5, {2, 1, 5, 1, 2, 2, 2}, FLAGS_assert, printA, runBruteForce); 
   	test(testNum++, FLAGS_test, 10000, 10000, {0, 10000, 0}, FLAGS_assert, printA, runBruteForce); 

    // Calculated array tests
    //
    // Do some mini random sequence tests
	int N = 20;
	int M = 5;
	int K = 5;
    {
		vector<int> A = generateRandomSequence(N, M);
	    test(testNum++, FLAGS_test, K, M, A, FLAGS_assert, printA, runBruteForce);
    }

    {
        N = 10;
        M = 10000;
		vector<int> A = generateRandomSequence(N, M);
        test(testNum++, FLAGS_test, K, M, A, FLAGS_assert, printA, runBruteForce);
    }

    {
        N = 20;
        M = 10000;
		vector<int> A = generateOneOrMSequence(N, M, 0.90);
        test(testNum++, FLAGS_test, K, M, A, FLAGS_assert, printA, runBruteForce);
    }

	// Don't run brute force test below here because brute force
    // is O(N^K)
    FLAGS_assert = false;
    printA = false;
    runBruteForce = false;

	N = 100000;
	M = 10000;
	K = 100000;
	vector<int> T(N);

    // Test max values with max K
    {
        for (int i=0; i<N; i++) {
            T[i] = M;
        }
        test(testNum++, FLAGS_test, K, M, T, FLAGS_assert, printA, runBruteForce);
    }

    // Make it so that K blocks need to be calculated
	M = 10000;
	K = 10000;

    // Test max values
    {
        for (int i=0; i<N; i++) {
            T[i] = M;
        }
        test(testNum++, FLAGS_test, K, M, T, FLAGS_assert, printA, runBruteForce);
    }

    // Test binary values
    {
		vector<int> A = generateRandomSequence(N, 2);
        test(testNum++, FLAGS_test, K, M, T, FLAGS_assert, printA, runBruteForce);
    }
    
    // Test random values from [0 .. M] but with K=10000
    {
        vector<int> A = generateRandomSequence(N, M);
        test(testNum++, FLAGS_test, K, M, A, FLAGS_assert, printA, runBruteForce);
    }
}
