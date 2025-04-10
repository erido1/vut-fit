/*
 *   Author: Erika Do <xdoeri00@vutbr.cz>
 *   File: oets.cpp
 *   Date: 2025-03-12
 *   Description: This program implements the Odd-even transposition sort algorithm using MPI for parallel processing. 
 *   It reads an unsorted sequence of integers from the file 'numbers', distributes the data among multiple processes, 
 *   and performs sorting through iterative exchanges between even- and odd-ranked processes.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>

#define TAG_EVEN 0
#define TAG_ODD 1

const std::string fileName = "numbers";

/*
 * Reads a sequence of unsigned 8-bit integers from a binary file and stores them 
 * as unsigned integers in the provided vector.
 *
 * @param numbers A reference to a vector of unsigned integers where the read numbers will be stored.
 */
void readNumbers(std::vector<unsigned int> &numbers) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << fileName << "!" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (buffer.empty()) {
        std::cerr << "Error: File " << fileName << " is empty!" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    numbers.reserve(buffer.size());
    for (unsigned char byte : buffer) {
        numbers.push_back(static_cast<unsigned int>(byte));
    }
}

/*
 * Prints the contents of a vector of unsigned integers to the standard output.
 *
 * @param numbers A reference to a vector of unsigned integers to be printed.
 * @param delim A character to be used as a delimiter between each printed number.
 */
void printNumbers(std::vector<unsigned int> &numbers, char delim) {
    for (unsigned int num : numbers) {
        std::cout << num << delim;
    }

    std::cout << std::flush;
}

/*
 * Calculates the ranks of the neighbouring processes.
 *
 * @param rank The rank of the current process.
 * @param procs The total number of processes.
 * @param rank_prev A reference to store the rank of the previous process.
 * @param rank_next A reference to store the rank of the next process.
 */
void getNeighbours(int rank, int procs, int &rank_prev, int &rank_next) {
    rank_prev = (rank - 1 >= 0) ? rank - 1 : MPI_PROC_NULL;
    rank_next = (rank + 1 < procs) ? rank + 1 : MPI_PROC_NULL;
}

/*
 * Performs the Odd-Even Transposition Sort algorithm across multiple processes using MPI.
 * The function alternates between "even" and "odd" phases where neighboring processes compare and potentially swap their values.
 *
 * @param number A reference to the number on the current process that needs to be sorted. It will be updated after the sorting.
 * @param rank The rank of the current process.
 * @param procs The total number of processes.
 */
void oddEvenTransposition(unsigned int &number, int rank, int procs) {
    // Inspired by: https://www.youtube.com/watch?v=c0C9mQaxsD4&t=674s
    unsigned int num_mine = number, num_other;
    int rank_prev, rank_next;
    MPI_Status stat;

    getNeighbours(rank, procs, rank_prev, rank_next);

    for (int i = 0; i < procs; i++) {
        // Even phase - Each odd processor p_i connects with its neighbor p_(i+1) and compares their values. 
        if (i % 2 == 0) { 
            if (rank % 2 == 0 && rank_next != MPI_PROC_NULL) {
                // I: Even-ranked process receives from next process and compares
                MPI_Recv(&num_other, 1, MPI_UNSIGNED, rank_next, TAG_EVEN, MPI_COMM_WORLD, &stat);
                // If y_i > y_(i+1), the processors exchange their values.
                if (num_mine > num_other) {
                    std::swap(num_mine, num_other);
                }
                // II: Sends updated value back to the next process
                MPI_Send(&num_other, 1, MPI_UNSIGNED, rank_next, TAG_EVEN, MPI_COMM_WORLD);
            } else if (rank % 2 != 0 && rank_prev != MPI_PROC_NULL) {
                // I: Odd-ranked process sends to previous process
                MPI_Send(&num_mine, 1, MPI_UNSIGNED, rank_prev, TAG_EVEN, MPI_COMM_WORLD);
                // II: Receives updated value
                MPI_Recv(&num_mine, 1, MPI_UNSIGNED, rank_prev, TAG_EVEN, MPI_COMM_WORLD, &stat);
            }
        }

        MPI_Barrier(MPI_COMM_WORLD); // Syncs all processes after the even phase
        // Odd phase - Each even processor p_i connects with its neighbor p_(i+1) and compares their values. 
        if (i % 2 != 0) {  
            if (rank % 2 != 0 && rank_next != MPI_PROC_NULL) {
                // I: Odd-ranked process receives from next process and compares
                MPI_Recv(&num_other, 1, MPI_UNSIGNED, rank_next, TAG_ODD, MPI_COMM_WORLD, &stat);
                // If y_i > y_(i+1), the processors exchange their values.
                if (num_mine > num_other) {
                    std::swap(num_mine, num_other);
                }
                // II: Sends updated value back to the next process
                MPI_Send(&num_other, 1, MPI_UNSIGNED, rank_next, TAG_ODD, MPI_COMM_WORLD);
            } else if (rank % 2 == 0 && rank_prev != MPI_PROC_NULL) {
                // I: Even-ranked process sends to previous process
                MPI_Send(&num_mine, 1, MPI_UNSIGNED, rank_prev, TAG_ODD, MPI_COMM_WORLD);
                // II: Receives updated value
                MPI_Recv(&num_mine, 1, MPI_UNSIGNED, rank_prev, TAG_ODD, MPI_COMM_WORLD, &stat);
            }
        }
        // Syncs all processes after the odd phase
        MPI_Barrier(MPI_COMM_WORLD); 
    }
    // Updates the number after sorting
    number = num_mine; 
}


int main(int argc, char** argv) {
    int rank, num_procs;
    std::vector<unsigned int> numbers;  
    unsigned int given_num;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (rank == 0) {
        readNumbers(numbers);
        printNumbers(numbers, ' ');
        std::cout << std::endl;
    }
    // Scatters the numbers to all processes
    MPI_Scatter(numbers.data(), 1, MPI_UNSIGNED, &given_num, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD); 

    oddEvenTransposition(given_num, rank, num_procs);
    // Gathers the sorted numbers back to root process
    MPI_Gather(&given_num, 1, MPI_UNSIGNED, numbers.data(), 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD); 

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        printNumbers(numbers, '\n');
    }

    MPI_Finalize();
    return 0;
}
