#include <stdio.h>
#include <stdlib.h>

typedef struct Process {
    int pid; // Process ID
    int bt;  // Burst Time
    int art; // Arrival Time
    int wt;  // Waiting Time
    int tat; // Turnaround Time
    int pri; // Priority
} ProcessType;

// Function to compare processes for sorting based on priority
int comparePriority(const void *a, const void *b) {
    return ((ProcessType *)a)->pri - ((ProcessType *)b)->pri;
}

// Function to find waiting time for processes using Priority scheduling
void findWaitingTimePriority(ProcessType plist[], int n) {
    qsort(plist, n, sizeof(ProcessType), comparePriority);

    // Apply FCFS algorithm to find waiting time
    plist[0].wt = 0;
    for (int i = 1; i < n; i++) {
        plist[i].wt = plist[i - 1].wt + plist[i - 1].bt;
    }
}

// Function to find waiting time for processes using Shortest Job First (SJF)
void findWaitingTimeSJF(ProcessType plist[], int n) {
    // Assuming processes are already sorted based on burst time (SJF)
    plist[0].wt = 0;
    for (int i = 1; i < n; i++) {
        plist[i].wt = plist[i - 1].wt + plist[i - 1].bt;
    }
}

// Function to find waiting time for processes using Round Robin (RR)
void findWaitingTimeRR(ProcessType plist[], int n, int quantum) {
    int bt_rem[n];
    for (int i = 0; i < n; i++) {
        bt_rem[i] = plist[i].bt;
    }

    int t = 0;
    while (1) {
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (bt_rem[i] > 0) {
                done = 0;
                if (bt_rem[i] > quantum) {
                    t += quantum;
                    bt_rem[i] -= quantum;
                } else {
                    t += bt_rem[i];
                    plist[i].wt = t - plist[i].bt;
                    bt_rem[i] = 0;
                }
            }
        }
        if (done == 1)
            break;
    }
}

// Function to calculate turnaround time for processes
void findTurnAroundTime(ProcessType plist[], int n) {
    for (int i = 0; i < n; i++) {
        plist[i].tat = plist[i].wt + plist[i].bt;
    }
}

int main() {
    int n, quantum;

    // Read the number of processes and quantum from input
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter the time quantum for Round Robin: ");
    scanf("%d", &quantum);

    // Allocate memory for processes
    ProcessType plist[n];

    // Read process information from input
    for (int i = 0; i < n; i++) {
        printf("Enter details for process %d:\n", i + 1);
        plist[i].pid = i + 1;

        printf("Burst Time: ");
        scanf("%d", &plist[i].bt);

        // Assuming arrival time is 0 for simplicity
        plist[i].art = 0;

        printf("Priority: ");
        scanf("%d", &plist[i].pri);
    }

    // Call functions to calculate waiting times for each scheduling algorithm
    findWaitingTimePriority(plist, n);
    findWaitingTimeSJF(plist, n);
    findWaitingTimeRR(plist, n, quantum);

    // Call function to calculate turnaround time
    findTurnAroundTime(plist, n);

    // Print the results
    printf("\nProcess\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].pri, plist[i].wt, plist[i].tat);
    }

    return 0;
}
