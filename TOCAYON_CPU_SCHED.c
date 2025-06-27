#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#define MAX 10

//STEPHANIE B. TOCAYON, BSCS-3B
//CPU SCHEDULING ALGORITHMS ACTIVITY IN OPERATING SYSTEM

//stores the values 
typedef struct {
    int pid;	// Process ID
    int at;		// Arrival Time
    int bt;		// Burst Time
    int pn;		// Priority Number
    int ct;		// Completion Time
    int tat;	// Turn Around Time
    int wt;		// Waiting Time
	int rt;		// Remaining Time
} Process;
//function declarations
void duplicate_process(Process orig[], Process copy[], int p);
void gantt_chart(int gantt[], int gt[], int p);
void sort_by_AT(Process *proc, int p);
void calculate_times(Process *proc, int p);
void print_table(Process *proc, int p);
void fcfs(Process proc[], int p);
void sjf(Process *proc, int p);
void prio_p(Process *proc, int p);
void prio_np(Process *proc, int p);
void round_robin(Process *proc, int p, int q);


int main() {
	char bichoice;
    printf("=========================================================\n");
    printf("            WELCOME TO CPU SCHEDULING ALGORITHMS!\n");
    printf("=========================================================\n");

    do {
        // Declare variables for processes
        Process proc[MAX];
        int p, q, choice;

        // Prompt for number of processes
        printf("Enter the number of processes you want to be calculated: ");
        scanf("%d", &p);

        if (p <= 0) {
            printf("Please input a positive number!\n");
            continue;  // If the number of processes is invalid, restart
        }

        printf("======================================================================================\n");
        printf("Please enter arrival time, burst time and priority for the %d processes : ", p);
        printf("\n======================================================================================\n");

        // Input for all the processes
        for (int i = 0; i < p; i++) {
            printf("Process ID: %d\n", i + 1);
            proc[i].pid = i + 1;
            printf("Arrival Time:");
            scanf("%d", &proc[i].at);
            printf("Burst Time:");
            scanf("%d", &proc[i].bt);
            printf("Priority:");
            scanf("%d", &proc[i].pn);
        }

        printf("\nEnter Quantum Time (for Round Robin):");
        scanf("%d", &q);
     
        
        do {
            // Prompt for CPU Scheduling Algorithm of choice
            printf("===========================================\n");
            printf("Choose the CPU algorithm you want to use:\n");
            printf("============================================\n");
            printf("[1]. First-Come, First Served\n");
            printf("[2]. Shortest Job First (Non-Preemtive)\n");
            printf("[3]. Priority Scheduling (Non-Preemtive)\n");
            printf("[4]. Priority Scheduling (Preemtive)\n");
            printf("[5]. Round Robin\n");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    fcfs(proc, p);
                    break;
                case 2:
                    sjf(proc, p);
                    break;
                case 3:
                    prio_np(proc, p);
                    break;
                case 4:
                    prio_p(proc, p);
                    break;
                case 5:
                    round_robin(proc, p, q);
                    break;
                default:
                    printf("Invalid choice!\n");
                    break;
            }

            // Ask the user if they want to use the same values with another algorithm
            printf("\nDo you want to use the same values with another CPU algorithm? (y/n): ");
            getchar();  // Clear newline from previous input
            scanf("%c", &bichoice);

        } while (bichoice == 'y' || bichoice == 'Y');

        // Ask the user if they want to run another set of processes
        printf("Do you want to run another set of processes? (y/n): ");
        getchar();  // Clear newline from previous input
        scanf("%c", &bichoice);

        // Clear the screen for the next set of processes
        if (bichoice == 'y' || bichoice == 'Y') {
            #ifdef _WIN32
                system("cls"); // For Windows
            #else
                system("clear"); // For Linux/Unix
            #endif
        }

    } while (bichoice == 'y' || bichoice == 'Y');

    printf("\nThank you for using this CPU Scheduling Calculator.\n");
    return 0;
}
// Function to duplicate the data of the processes
void duplicate_process(Process orig[], Process copy[], int p) {
    for (int a = 0; a < p; a++) {
        copy[a] = orig[a];			// Copy the entire structure
        copy[a].wt = 0;       		// Initialize waiting time
        copy[a].rt = orig[a].bt; 	// Remaining time initialized to burst time
        copy[a].ct = 0;       		// Initialize completion time
    }
}
// Function to create and print a gantt chart
void gantt_chart(int gantt[], int gt[], int p) {
    printf("\nGantt Chart\n");
    // Print the top bar of the Gantt chart
    for (int b = 0; b < p; b++)
        printf("---------");
    printf("\n|");
	//Print the process IDs or "Idle" in the Gantt Chart
	for (int b = 0; b < p; b++) {
        if (gantt[b] == -1) {
            printf("  Idle  |");
        } else {
            printf("   P%d   |", gantt[b]);
        }
    }
    printf("\n");
	// Print the bottom bar of the Gantt chart
    for (int b = 0; b < p; b++) 
        printf("---------");
    printf("\n");

    // Print the timeline below the Gantt chart
    for (int b = 0; b <= p; b++) {
        printf("%-9d", gt[b]);
    }
}
// Function that sorts the process based on the arrival time
void sort_by_AT(Process *proc, int p) {
    for (int c = 0; c < p - 1; c++) {
        for (int d = 0; d < p - c - 1; d++) {
            ////Swaps the proc[c] and proc[d] if proc[d] arrived first
			if (proc[d].at > proc[d + 1].at) {
                Process temp = proc[d];
                proc[d] = proc[d + 1];
                proc[d + 1] = temp;
            }
        }
    }
}

// Computes for the total and average turnaround time and waiting time
void calculate_times(Process *proc, int p) {
    int total_tat = 0, total_wt = 0;
    for (int e = 0; e < p; e++) { // Loop through each process and sum up turnaround time (tat) and waiting time (wt)
        total_tat += proc[e].tat;
        total_wt += proc[e].wt;
    }

    float avg_tat = (float)total_tat / p;
    float avg_wt = (float)total_wt / p;
    printf("\nAverage Turn Around Time: %.2f.ms\n", avg_tat);
    printf("Average Waiting Time: %.2f ms\n\n", avg_wt);
}

// Function to print the process table
void print_table(Process *proc, int p) {
    int total_tat = 0;
    int total_wt = 0;

    printf("\n--------------------------------------------------------------------------------------------------------------\n");
    printf("| Priority | Process ID  | Arrival Time | Burst Time |  Completion Time |  Turnaround Time  |  Waiting Time  |\n");
    printf("--------------------------------------------------------------------------------------------------------------\n");

    // Loop through each process and print the details
    for (int f = 0; f < p; f++) {
        printf("|    %3d   |    %5d    |     %5d     |    %5d   |      %5d       |      %5d        |     %5d     |\n",
               proc[f].pn, proc[f].pid, proc[f].at, proc[f].bt, proc[f].ct, proc[f].tat, proc[f].wt);

        total_tat += proc[f].tat;
        total_wt += proc[f].wt;
    }
    printf("\nTOTAL                                                                     \t%5d              \t%5d     \n", total_tat, total_wt);
   
}

//// First Come First Serve Scheduling 
void fcfs(Process proc[], int p) {
    // Sort processes by arrival time
    sort_by_AT(proc, p);

    int gantt[p], gt[p + 1];
    int cut = proc[0].at; // Start at the arrival of the first process
    gt[0] = cut;

    int index = 0; // Index for Gantt chart
    for (int g = 0; g < p; g++) {
        if (cut < proc[g].at) {
            // If there's idle time, insert idle period in Gantt chart
            gantt[index] = -1; // Use -1 to represent idle time
            gt[index + 1] = proc[g].at; // Gantt time jumps to the next process arrival
            index++;
            cut = proc[g].at; // Update time to the process arrival time
        }

        proc[g].wt = cut - proc[g].at;  // Calculate waiting time
        proc[g].tat = proc[g].wt + proc[g].bt;  // Turnaround time = waiting + burst
        cut += proc[g].bt; // Advance time by the burst time of the current process
        proc[g].ct = cut; // Completion time

        // Gantt chart process ID and time
        gantt[index] = proc[g].pid;
        gt[index + 1] = cut;
        index++;
    }

    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("                                                    FCFS\n");
    print_table(proc, p);

    // Compute and print total/average turnaround and waiting times
    calculate_times(proc, p);

    // Print Gantt Chart
    gantt_chart(gantt, gt, index);
}

//// Shortest Job First - Non Preemptive
void sjf(Process *proc, int p) {
    sort_by_AT(proc, p);
    int completed = 0, cut = 0, shortest;
    int gantt[100], gt[101], index = 0;
    int completed_flag[p];

    for (int h = 0; h < p; h++) // Initialize the completed flag array to mark uncompleted processes
        completed_flag[h] = 0;

    while (completed != p) { // Continue scheduling until all processes are completed
        shortest = -1;
        for (int i = 0; i < p; i++) { // Find the process with the shortest burst time that has arrived
            if (proc[i].at <= cut && !completed_flag[i]) {
                if (shortest == -1 || proc[i].bt < proc[shortest].bt)
                    shortest = i;
            }
        }
        if (shortest != -1) { // If a valid shortest process is found
            gantt[index] = proc[shortest].pid;
            gt[index] = cut;
            index++;

            cut += proc[shortest].bt; // Update current time based on the process's burst time
             // Calculate turnaround time, waiting time, and completion time
            proc[shortest].tat = cut - proc[shortest].at;
            proc[shortest].wt = proc[shortest].tat - proc[shortest].bt;
            proc[shortest].ct = cut;
            completed_flag[shortest] = 1; // Mark process as completed and increase the count of completed processes
            completed++;
        } else {
            gantt[index] = -1; // Add idle time
            gt[index] = cut;
            index++;
            cut++;
        }
    }
    gt[index] = cut; // Store final time

    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("                                                SJF (Non-Preemptive)   \n");
    // Print table with all process details
    print_table(proc, p);
    // Compute and print total/average turnaround and waiting times
    calculate_times(proc, p);
    // Print Gantt Chart
    gantt_chart(gantt, gt, index);
}

//// Priority Scheduling - Non Preemptive
void prio_np(Process *proc, int p) {
    sort_by_AT(proc, p); // Ensure the processes are sorted by arrival time
    int finished = 0, cut = 0, highest;
    int gantt[100], gt[101], index = 0;
    int finished_flag[p];

    for (int j = 0; j < p; j++) finished_flag[j] = 0; // Initialize the completed flag array to mark uncompleted processes

    while (finished != p) { 
        highest = -1;
       
        for (int k = 0; k < p; k++) {
            if (proc[k].at <= cut && !finished_flag[k]) { // 
                if (highest == -1 || proc[k].pn < proc[highest].pn) // compare priority
                    highest = k;
            }
        }

        if (highest != -1) {
            gantt[index] = proc[highest].pid;
            gt[index] = cut;
            index++;

            cut += proc[highest].bt; // add burst time
            proc[highest].tat = cut - proc[highest].at; // turn around time
            proc[highest].wt = proc[highest].tat - proc[highest].bt; // waiting time
            proc[highest].ct = cut; // completion time
            finished_flag[highest] = 1;
            finished++;
        } else {
            gantt[index] = -1; // Add idle time
            gt[index] = cut;
            index++;
            cut++;
        }
    }
    gt[index] = cut;

    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("                                            Priority (Non-Preemptive) \n");
    // Print the process table
    print_table(proc, p);

    // Compute and print total/average turnaround and waiting times
    calculate_times(proc, p);

    // Print Gantt Chart
    gantt_chart(gantt, gt, index);
}
void prio_p(Process *proc, int p) {
    sort_by_AT(proc, p); // ensure processes are sorted by arrival time
    int gantt[100], gt[101], index = 0;
    int cut = 0, done = 0;
    int remaining[p];
    int done_flag[p];

    for (int l = 0; l < p; l++) {
        remaining[l] = proc[l].bt; // Remaining time initialized to burst time
        done_flag[l] = 0; // Track whether the process is completed
    }

    int last_pid = -1; // Track the last process ID added to the Gantt chart

    while (done < p) {
        int highest = -1; // Index of the highest priority process
        int all_done = 1; // Check if all processes are completed

        for (int m = 0; m < p; m++) {
            if (remaining[m] > 0) {
                all_done = 0; // At least one process is still pending
                if (proc[m].at <= cut) { // Process has arrived
                    if (highest == -1 || proc[m].pn < proc[highest].pn) {
                        highest = m; // Select the highest priority process
                    }
                }
            }
        }

        if (highest != -1) {
            if (last_pid != proc[highest].pid) {
                gantt[index] = proc[highest].pid; // Log current process in Gantt
                gt[index] = cut;
                index++;
                last_pid = proc[highest].pid;
            }
            // Process the highest priority process
            cut++;
            remaining[highest]--;
            // Check if the process is finished
            if (remaining[highest] == 0) {
                proc[highest].ct = cut;
                proc[highest].tat = proc[highest].ct - proc[highest].at; // turnaround time
                proc[highest].wt = proc[highest].tat - proc[highest].bt; // waiting time
                done++; // increase 'done' count
            }
        } else {
            if (last_pid != -1) {
                gantt[index] = -1; // log idle time
                gt[index] = cut; // log the time of the idle
                index++;
                last_pid = -1;
            }
            cut++; // increment time
        }
    }
    gt[index] = cut; // final time

    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("                                                    Priority (Preemptive)\n");
    // Print the process table
    print_table(proc, p);

    // Compute and print total/average turnaround and waiting times
    calculate_times(proc, p);

    // Print Gantt Chart
    gantt_chart(gantt, gt, index);
}

//Round Robin
void round_robin(Process *proc, int p, int q) {
    int gantt[100], gt[101], index = 0;
    int remaining[p];
    for (int n = 0; n < p; n++) {
        remaining[n] = proc[n].bt; // Remaining time initialized to burst time
    }

    int cut = 0, through = 0;
    gt[index] = cut; // Initialize Gantt chart time

    while (through < p) { // Continue scheduling until all processes are completed
        int exec = 0; // To track if any process was executed in this cycle
        for (int o = 0; o < p; o++) {
            if (proc[o].at <= cut && remaining[o] > 0) { // Execute process if it has arrived and has remaining burst time
                // Capture the process in the Gantt chart for every execution slice
                if (index == 0 || gantt[index - 1] != proc[o].pid) { // Capture the process in the Gantt chart for every execution slice
                    gantt[index] = proc[o].pid;
                    gt[index] = cut;
                    index++;
                }
                // If process needs more time than the quantum
                if (remaining[o] > q) {
                    cut += q; // Increment the current time by quantum
                    remaining[o] -= q;  // Decrease remaining burst time
                } else {
                    // Process will complete in this cycle
                    cut += remaining[o];
                    remaining[o] = 0;
					proc[o].tat = cut - proc[o].at; // Turnaround time calculation
                    proc[o].wt = proc[o].tat - proc[o].bt; // Waiting time calculation
                    proc[o].ct = cut;  // Completion time
                    through++; // Increment the number of completed processes
                }
                exec = 1;
            }
        }
        if (!exec) {
            gantt[index] = -1; // Add idle time
            gt[index] = cut;
            index++;
            cut++;
        }
    }
    gt[index] = cut; // Store the final time

    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("                                            Round  Robin      \n");
   // Print the process table
    print_table(proc, p);

    // Compute and print total/average turnaround and waiting times
    calculate_times(proc, p);

    // Print Gantt Chart
    gantt_chart(gantt, gt, index);
}
