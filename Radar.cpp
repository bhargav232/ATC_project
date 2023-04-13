// Logic for printing plan view of the system every 5 second!
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>

#define PLAN_VIEW_HEIGHT 200
#define PLAN_VIEW_WIDTH  200

using namespace std;

void update_plan_view(char plan_view[PLAN_VIEW_HEIGHT][PLAN_VIEW_WIDTH], int x, int y, int z);
void print_plan_view(char plan_view[PLAN_VIEW_HEIGHT][PLAN_VIEW_WIDTH]);

int main()
{
    ifstream file("positions.csv");
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return -1;
    }

    char plan_view[PLAN_VIEW_HEIGHT][PLAN_VIEW_WIDTH];
    memset(plan_view, '-', sizeof(plan_view));

    auto start_time = chrono::steady_clock::now();
    while (true) {
        // Read positions from file and update plan view
        string line;
        while (getline(file, line)) {
            int x, y, z;
            sscanf(line.c_str(), "%d,%d,%d", &x, &y, &z);
            update_plan_view(plan_view, x, y, z);
        }

        // Print plan view if 5 seconds have passed
        auto current_time = chrono::steady_clock::now();
        auto elapsed_time = chrono::duration_cast<chrono::seconds>(current_time - start_time).count();
        if (elapsed_time >= 5) {
            print_plan_view(plan_view);
            start_time = current_time;
        }

        // Wait for a short period before checking again
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    return 0;
}

void update_plan_view(char plan_view[PLAN_VIEW_HEIGHT][PLAN_VIEW_WIDTH], int x, int y, int z)
{
    if (x >= 0 && x < PLAN_VIEW_WIDTH && y >= 0 && y < PLAN_VIEW_HEIGHT) {
        plan_view[y][x] = '*';
    }
}

void print_plan_view(char plan_view[PLAN_VIEW_HEIGHT][PLAN_VIEW_WIDTH])
{
    for (int i = 0; i < PLAN_VIEW_HEIGHT; i++) {
        for (int j = 0; j < PLAN_VIEW_WIDTH; j++) {
            cout << plan_view[i][j];
        }
        cout << endl;
    }
}

