#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <vector>
#include <pthread.h>

int n, amountOfThreads = 8;  // number of threads and n
std::ifstream in;            // input file
std::ofstream out;           // output file

std::set<int8_t> getDigits(uint64_t n) {  // function to get digits of number
    std::set<int8_t> digits;
    while (n > 0) {
        digits.insert(n % 10);  // insert digit in set
        n /= 10;
    }
    return digits;
}

bool checkThatOneSetHaveAllDigitsFromAnother(
    const std::set<int8_t>& first,
    const std::set<int8_t>& second) {  // check if one set have all digits from another
    for (auto digit : first) {
        if (second.find(digit) == second.end()) {
            return false;
        }
    }
    return true;
}

void* checkNumbers(void* arg) {                           // function to check numbers
    uint64_t start = 1e4;                                 // start of range
    uint64_t end = 1e9;                                   // end of range
    uint64_t step = (end - start) / amountOfThreads;      // step
    uint64_t currentStart = start + step * (int64_t)arg;  // current start
    uint64_t currentEnd = currentStart + step;            // current end
    for (uint64_t i = currentStart; i < currentEnd; ++i) {
        if (checkThatOneSetHaveAllDigitsFromAnother(getDigits(i), getDigits(i * n))) {
            // output to file
            out << i << " ";
        }
    }
    return nullptr;
}

int getRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

int main(int argc, char** argv) {
    // Get amount of threads from command line arguments.
    // If there are no arguments amount of threads will be 8.
    // If there are two arguments, that the first one is amount of threads and the second one is
    // output file name. If there are three arguments, that the first one is amount of threads, the
    // second one is input file name and the third one is output file name.
    if (argc == 2) {
        amountOfThreads = atoi(argv[1]);
        n = getRandomNumber(1, 9);
        out = std::ofstream("output.txt", std::ios::out);
    } else if (argc == 3) {
        amountOfThreads = atoi(argv[1]);
        n = getRandomNumber(1, 10);
        out = std::ofstream(argv[2], std::ios::out);
    } else if (argc == 4) {
        amountOfThreads = atoi(argv[1]);
        in = std::ifstream(argv[2], std::ios::in);
        in >> n;
        in.close();
        out = std::ofstream(argv[3], std::ios::out);
    }

    // create array of threads
    pthread_t threads[amountOfThreads];
    // create array of steps
    int steps[amountOfThreads];
    // create threads
    for (int i = 0; i < amountOfThreads; ++i) {
        steps[i] = i;
        pthread_create(&threads[i], nullptr, checkNumbers, (void*)&steps[i]);
    }

    // wait for threads to finish
    for (int i = 0; i < amountOfThreads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    out.close();

    return 0;
}