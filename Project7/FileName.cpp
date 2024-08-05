#include <iostream>
#include <string>
#include <windows.h>
#include <iomanip>
#include <queue>
#include <utility>
using namespace std;
struct Process {
    int pno;
    int arrival_time, burst_Time, priority; //given by user
    int start_time, completion_time, turnaround_time, waiting_time; //calculated for each process
};
void non_preemptive(Process* processes, int size);
void preemptive(Process* processes, int size);
void RoundRobin(Process* processesRR, int size, int  quantum);
void sjf_np(Process* processes, int size);
void sjf_p(Process* p, int nump);
void FCFS(Process* processes, int size);

void deepCopy(Process* dest, const Process* src, int nump) {
    for (int i = 0; i < nump; ++i) {
        dest[i] = src[i]; // Perform a deep copy of each object
    }
}
int main()
{
    system("color F1");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get the number of processes from the user
    int nump;
    cout << "Enter the number of processes: ";
    cin >> nump;
    Process* processes = new Process[nump];
    Process* processesRoundrobin = new Process[nump];
    Process* processesNonPermitive = new Process[nump];
    Process* processesFCFS = new Process[nump];
    Process* processesSJFNP = new Process[nump];
    Process* processesSJFP = new Process[nump];

    system("cls");

    // Enter data in a tabular form
    cout << "Process" << setw(15) << "Arrival Time" << setw(15) << "Burst Time" << setw(15) << "Priority" << endl;
    for (int i = 0; i < nump; ++i) {
        processes[i].pno = i + 1;
        cout << processes[i].pno;

        for (int j = 0; j < 3; ++j) { // 3 columns
            COORD pos = { 15 + j * 15, i + 1 };   // Adjust column width (15 characters) and row position
            SetConsoleCursorPosition(hConsole, pos);

            // Prompt the user
            if (j == 0) {

                cin >> processes[i].arrival_time;
            }
            else if (j == 1) {

                cin >> processes[i].burst_Time;
            }
            else {

                cin >> processes[i].priority;
            }
        }
    }
    cout << endl;
    //------------------------------------------------
    int quantum = 0;
    cout << "Enter time quantum: ";
    cin >> quantum;


    deepCopy(processesRoundrobin, processes, nump);
    deepCopy(processesNonPermitive, processes, nump);
    deepCopy(processesFCFS, processes, nump);
    deepCopy(processesSJFNP, processes, nump);
    deepCopy(processesSJFP, processes, nump);

    FCFS(processesFCFS, nump);
    preemptive(processes, nump);
    RoundRobin(processesRoundrobin, nump, quantum);
    non_preemptive(processesNonPermitive, nump);
    sjf_np(processesSJFNP, nump);
    sjf_p(processesSJFP, nump);

    int i;
    cin >> i;

    return 0;

}

//********************************* Priority Non-Preemptive ****************************************//

void non_preemptive(Process* processes, int size) {
    int nump = size;
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int completed = 0, current_time = 0;
    int* is_completed = new int[nump];
    // Initialize the array to all zeros
    for (int i = 0; i < nump; ++i) {
        is_completed[i] = 0;
    }
    while (completed != nump)
    {
        int prior = 10000, index = -1;
        for (int i = 0; i < nump; i++) {
            if (processes[i].arrival_time <= current_time && is_completed[i] == 0) //el process wslt w mtnfztsh
            {
                if (processes[i].priority < prior) //priority el process(i) a3la mn priority process tanya
                {
                    prior = processes[i].priority;
                    index = i;
                }
                if (processes[i].priority == prior) //two processes have the same priority
                {
                    if (processes[i].arrival_time < processes[index].arrival_time) //execute the process that came first
                    {
                        index = i;

                    }
                }
            }
        } // exit from for loop when process is completed or when there's no any new processes arrived

        if (index != -1) { //executing process with highest priority 
            processes[index].start_time = current_time;
            processes[index].completion_time = processes[index].burst_Time + processes[index].start_time;
            processes[index].waiting_time = processes[index].completion_time - processes[index].arrival_time - processes[index].burst_Time;
            processes[index].turnaround_time = processes[index].waiting_time + processes[index].burst_Time;
            total_turnaround_time += processes[index].turnaround_time;
            total_waiting_time += processes[index].waiting_time;
            is_completed[index] = 1;
            completed++;
            current_time = processes[index].completion_time;
        }
        else { //mfesh process gdeda wslt fa hzwd el time bta3i 
            current_time++;
        }
    }
    int min_arrival_time = 10000000;
    int max_completion_time = -1;
    for (int i = 0; i < nump; i++) {
        min_arrival_time = min(min_arrival_time, processes[i].arrival_time);
        max_completion_time = max(max_completion_time, processes[i].completion_time);
    }

    avg_turnaround_time = (float)total_turnaround_time / nump;
    avg_waiting_time = (float)total_waiting_time / nump;
    float throughput = float(nump) / (max_completion_time - min_arrival_time); //max no of processes elly bttnfz per unit time
    cout << endl;
    // Output the data entered by the user
    cout << "                            ********************** Priority Non-Preemptive **********************" << endl;
    cout << endl;
    cout << "Process" << setw(15) << "Arrival Time" << setw(15) << "Burst Time" << setw(15) << "Priority" << setw(15) << "Start Time" << setw(18) << "Completion Time" << setw(18) << "Turnaround Time" << setw(18) << "Waiting Time" << endl;
    for (int i = 0; i < nump; ++i) {
        cout << processes[i].pno << setw(15);
        cout << processes[i].arrival_time << setw(15);
        cout << processes[i].burst_Time << setw(15);
        cout << processes[i].priority << setw(15);
        cout << processes[i].start_time << setw(15);
        cout << processes[i].completion_time << setw(18);
        cout << processes[i].turnaround_time << setw(18);
        cout << processes[i].waiting_time << endl;
    }
    cout << endl;
    cout << "Average Turnaround Time = " << avg_turnaround_time << endl;
    cout << "Average Waiting Time = " << avg_waiting_time << endl;
    cout << "Throughput = " << throughput << " process/unit time" << endl;
}

//********************************* Priority Preemptive ****************************************//

void preemptive(Process* processes, int size)
{
    int nump = size;
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int completed = 0, current_time = 0;
    int* is_completed = new int[nump];
    int* burst_remaining = new int[nump]; //remaining time to complete execution of process
    // Enter data in a tabular form

        // Initialize the array to all zeros
    for (int i = 0; i < nump; ++i) {
        is_completed[i] = 0;
    }
    for (int i = 0; i < nump; i++) {
        burst_remaining[i] = processes[i].burst_Time;
    }
    while (completed != nump)
    {
        int prior = 10000, index = -1;
        for (int i = 0; i < nump; i++) {
            if (processes[i].arrival_time <= current_time && is_completed[i] == 0) //el process wslt w mtnfztsh
            {
                if (processes[i].priority < prior) //priority el process(i) a3la mn priority process tanya
                {
                    prior = processes[i].priority;
                    index = i;
                }
                if (processes[i].priority == prior) //two processes have the same priority
                {
                    if (processes[i].arrival_time < processes[index].arrival_time) //execute the process that came first
                    {
                        prior = processes[i].priority;
                        index = i;

                    }
                }
            }
        } // exit from for loop when process is completed or when there's no any new processes arrived

        if (index != -1) { //executing process with highest priority l7d ma tigy process a3la mnha fl priority
            if (burst_remaining[index] == processes[index].burst_Time) { //first time executing the process
                processes[index].start_time = current_time;
            }
            burst_remaining[index] -= 1; //hfdl a3ml execution ll process w an2s el burst time l7d ma 1)anfz el process kolha aw 
            current_time++;                    //2)process tanya ttnfz with higher priority

            if (burst_remaining[index] == 0) { //process is completed 
                processes[index].completion_time = current_time;
                processes[index].waiting_time = processes[index].completion_time - processes[index].arrival_time - processes[index].burst_Time;
                processes[index].turnaround_time = processes[index].waiting_time + processes[index].burst_Time;
                total_turnaround_time += processes[index].turnaround_time;
                total_waiting_time += processes[index].waiting_time;
                is_completed[index] = 1;
                completed++;
                //current_time = processes[index].completion_time;
            }
        }
        else { //mfesh process gdeda wslt fa hzwd el time bta3i 
            current_time++;
        }
    }

    //---------------------------------------------------------
    int min_arrival_time = 10000000;
    int max_completion_time = -1;
    for (int i = 0; i < nump; i++) {
        min_arrival_time = min(min_arrival_time, processes[i].arrival_time);
        max_completion_time = max(max_completion_time, processes[i].completion_time);
    }

    avg_turnaround_time = (float)total_turnaround_time / nump;
    avg_waiting_time = (float)total_waiting_time / nump;
    float throughput = float(nump) / (max_completion_time - min_arrival_time); //max no of processes elly bttnfz per unit time
    cout << endl;
    // Output the data entered by the user
    cout << "                            ********************** Priority Preemptive **********************" << endl;
    cout << endl;
    cout << "Process" << setw(15) << "Arrival Time" << setw(15) << "Burst Time" << setw(15) << "Priority" << setw(15) << "Start Time" << setw(18) << "Completion Time" << setw(18) << "Turnaround Time" << setw(18) << "Waiting Time" << endl;
    for (int i = 0; i < nump; ++i) {
        cout << processes[i].pno << setw(15);
        cout << processes[i].arrival_time << setw(15);
        cout << processes[i].burst_Time << setw(15);
        cout << processes[i].priority << setw(15);
        cout << processes[i].start_time << setw(15);
        cout << processes[i].completion_time << setw(18);
        cout << processes[i].turnaround_time << setw(18);
        cout << processes[i].waiting_time << endl;
    }
    cout << endl;
    cout << "Average Turnaround Time = " << avg_turnaround_time << endl;
    cout << "Average Waiting Time = " << avg_waiting_time << endl;
    cout << "Throughput = " << throughput << " process/unit time" << endl;
}

//*********************************  Round Robin ****************************************//

void RoundRobin(Process* processesRR, int size, int  quantum)
{
    int nump = size;
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int completed = 0, current_time = 0;
    int* burst_remaining = new int[nump];
    int tq = quantum;
    int idx;
    queue<int> q;
    for (int i = 0; i < nump; i++)
    {
        for (int j = i + 1; j < nump; j++)
        {
            if (processesRR[i].arrival_time > processesRR[j].arrival_time)
            {
                int temp;
                temp = processesRR[i].arrival_time;
                processesRR[i].arrival_time = processesRR[j].arrival_time;
                processesRR[j].arrival_time = temp;
            }
        }
    }


    for (int i = 0; i < nump; i++)
    {
        burst_remaining[i] = processesRR[i].burst_Time;
    }

    q.push(0);
    int mark[100];
    //make array "mark" initialize it with zeros 
    //memset() -> It copies a single character for a specified number of times to an object
    memset(mark, 0, sizeof(mark));
    mark[0] = 1;

    while (completed != nump)
    {
        //h5zn el front bta3 el queue fe "idx"
        idx = q.front();
        q.pop();

        if (burst_remaining[idx] == processesRR[idx].burst_Time)
        {

            processesRR[idx].start_time = max(current_time, processesRR[idx].arrival_time);
            total_waiting_time += processesRR[idx].start_time - current_time;
            current_time = processesRR[idx].start_time;
        }
        //check if processor not finish it's burst time  y3ni lw processor lsa ba2ylo hagat burst time =5 , q=3  hytb2a "2"
        if (burst_remaining[idx] - tq > 0)
        {
            burst_remaining[idx] -= tq;
            current_time += tq;
        }

        //check if processor finish it's burst time  y3ni lw processor el burst time =3 , q=3 yb2a hnsh8lo mra bs yb2a el burst_remaining 5las
        else
        {
            current_time += burst_remaining[idx];
            burst_remaining[idx] = 0;
            completed++;

            processesRR[idx].completion_time = current_time;
            processesRR[idx].turnaround_time = processesRR[idx].completion_time - processesRR[idx].arrival_time;
            processesRR[idx].waiting_time = processesRR[idx].turnaround_time - processesRR[idx].burst_Time;


            total_turnaround_time += processesRR[idx].turnaround_time;
            total_waiting_time += processesRR[idx].waiting_time;

        }

        for (int i = 1; i < nump; i++)
        {
            //check lw gali processor el burst time bta3o < time bta3 delw2ti bdefo LL queue 
            if (burst_remaining[i] > 0 && processesRR[i].arrival_time <= current_time && mark[i] == 0)
            {
                q.push(i);
                mark[i] = 1;
            }
        }
        //lw lsa el processor el burst time bta3o m5ls4 nd5lo el queue
        if (burst_remaining[idx] > 0)
        {
            q.push(idx);
        }

        if (q.empty())
        {
            for (int i = 1; i < nump; i++)
            {
                if (burst_remaining[i] > 0)
                {
                    q.push(i);
                    mark[i] = 1;
                    break;
                }
            }
        }
    }
    int min_arrival_time = 10000000;
    int max_completion_time = -1;
    for (int i = 0; i < nump; i++) {
        min_arrival_time = min(min_arrival_time, processesRR[i].arrival_time);
        max_completion_time = max(max_completion_time, processesRR[i].completion_time);
    }

    avg_turnaround_time = (float)total_turnaround_time / nump;
    avg_waiting_time = (float)total_waiting_time / nump;
    float throughput = float(nump) / (max_completion_time - min_arrival_time); //max no of processes elly bttnfz per unit time
    cout << endl;
    // Output the data entered by the user
    cout << "                            ********************** Roung Robin **********************" << endl;
    cout << endl;
    cout << "Process" << setw(15) << "Arrival Time" << setw(15) << "Burst Time" << setw(15) << "Priority" << setw(15) << "Start Time" << setw(18) << "Completion Time" << setw(18) << "Turnaround Time" << setw(18) << "Waiting Time" << endl;
    for (int i = 0; i < nump; ++i) {
        cout << processesRR[i].pno << setw(15);
        cout << processesRR[i].arrival_time << setw(15);
        cout << processesRR[i].burst_Time << setw(15);
        cout << processesRR[i].priority << setw(15);
        cout << processesRR[i].start_time << setw(15);
        cout << processesRR[i].completion_time << setw(18);
        cout << processesRR[i].turnaround_time << setw(18);
        cout << processesRR[i].waiting_time << endl;
    }
    cout << endl;
    cout << "Average Turnaround Time = " << avg_turnaround_time << endl;
    cout << "Average Waiting Time = " << avg_waiting_time << endl;
    cout << "Throughput = " << throughput << " process/unit time" << endl;
}

//*********************************  SJF-P ****************************************//
void sjf_np(Process* p, int nump)
{
    float avg_turnaround_time;
    float avg_waiting_time;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_idle_time = 0;
    float throughput;
    int is_completed[100];
    memset(is_completed, 0, sizeof(is_completed));

    int current_time = 0;
    int completed = 0;
    int prev = 0;

    while (completed != nump) {
        int idx = -1;
        int mn = 10000000;
        for (int i = 0; i < nump; i++) {
            if (p[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (p[i].burst_Time < mn) {
                    mn = p[i].burst_Time;
                    idx = i;
                }
                if (p[i].burst_Time == mn) {
                    if (p[i].arrival_time < p[idx].arrival_time) {
                        mn = p[i].burst_Time;
                        idx = i;
                    }
                }
            }
        }
        if (idx != -1) {
            p[idx].start_time = current_time;
            p[idx].completion_time = p[idx].start_time + p[idx].burst_Time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_Time;


            total_turnaround_time += p[idx].turnaround_time;
            total_waiting_time += p[idx].waiting_time;
            total_idle_time += p[idx].start_time - prev;

            is_completed[idx] = 1;
            completed++;
            current_time = p[idx].completion_time;
            prev = current_time;
        }
        else {
            current_time++;
        }

    }

    int min_arrival_time = 10000000;
    int max_completion_time = -1;
    for (int i = 0; i < nump; i++) {
        min_arrival_time = min(min_arrival_time, p[i].arrival_time);
        max_completion_time = max(max_completion_time, p[i].completion_time);
    }

    avg_turnaround_time = (float)total_turnaround_time / nump;
    avg_waiting_time = (float)total_waiting_time / nump;
    throughput = float(nump) / (max_completion_time - min_arrival_time);

    cout << endl << endl;

    cout << "                            ********************** SJF-NP **********************" << endl;

    cout << endl;
    cout << "Process" << setw(15) << "Arrival Time" << setw(15) << "Burst Time" << setw(15) << "Priority" << setw(15) << "Start Time" << setw(18) << "Completion Time" << setw(18) << "Turnaround Time" << setw(18) << "Waiting Time" << endl;
    for (int i = 0; i < nump; ++i) {
        cout << p[i].pno << setw(15);
        cout << p[i].arrival_time << setw(15);
        cout << p[i].burst_Time << setw(15);
        cout << p[i].priority << setw(15);
        cout << p[i].start_time << setw(15);
        cout << p[i].completion_time << setw(18);
        cout << p[i].turnaround_time << setw(18);
        cout << p[i].waiting_time << endl;
    }
    cout << endl;
    cout << "Average Turnaround Time = " << avg_turnaround_time << endl;
    cout << "Average Waiting Time = " << avg_waiting_time << endl;
    cout << "Throughput = " << throughput << " process/unit time" << endl;
}

void sjf_p(Process* p, int nump) {

    float avg_turnaround_time;
    float avg_waiting_time;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_idle_time = 0;
    float throughput;
    int burst_remaining[100];
    int is_completed[100];
    memset(is_completed, 0, sizeof(is_completed));

    // Initialize the array to all zeros
    for (int i = 0; i < nump; ++i) {
        is_completed[i] = 0;
    }
    for (int i = 0; i < nump; i++) {
        burst_remaining[i] = p[i].burst_Time;
    }
    int current_time = 0;
    int completed = 0;
    int prev = 0;
    while (completed != nump) {
        int idx = -1;
        int mn = 10000000;
        for (int i = 0; i < nump; i++) {
            if (p[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (burst_remaining[i] < mn) {
                    mn = burst_remaining[i];
                    idx = i;
                }
                if (burst_remaining[i] == mn) {
                    if (p[i].arrival_time < p[idx].arrival_time) {
                        mn = burst_remaining[i];
                        idx = i;
                    }
                }
            }
        }
        if (idx != -1) {
            if (burst_remaining[idx] == p[idx].burst_Time) {
                p[idx].start_time = current_time;
                total_idle_time += p[idx].start_time - prev;
            }
            burst_remaining[idx] -= 1;
            current_time++;
            prev = current_time;

            if (burst_remaining[idx] == 0) {
                p[idx].completion_time = current_time;
                p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
                p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_Time;
                total_turnaround_time += p[idx].turnaround_time;
                total_waiting_time += p[idx].waiting_time;
                is_completed[idx] = 1;
                completed++;
            }
        }
        else {
            current_time++;
        }
    }
    int min_arrival_time = 10000000;
    int max_completion_time = -1;
    for (int i = 0; i < nump; i++) {
        min_arrival_time = min(min_arrival_time, p[i].arrival_time);
        max_completion_time = max(max_completion_time, p[i].completion_time);
    }
    avg_turnaround_time = (float)total_turnaround_time / nump;
    avg_waiting_time = (float)total_waiting_time / nump;
    throughput = float(nump) / (max_completion_time - min_arrival_time);
    cout << endl << endl;
    cout << "                            ********************** SJF-P **********************" << endl;
    cout << endl;
    cout << "Process" << setw(15) << "Arrival Time" << setw(15) << "Burst Time" << setw(15) << "Priority" << setw(15) << "Start Time" << setw(18) << "Completion Time" << setw(18) << "Turnaround Time" << setw(18) << "Waiting Time" << endl;
    for (int i = 0; i < nump; ++i) {
        cout << p[i].pno << setw(15);
        cout << p[i].arrival_time << setw(15);
        cout << p[i].burst_Time << setw(15);
        cout << p[i].priority << setw(15);
        cout << p[i].start_time << setw(15);
        cout << p[i].completion_time << setw(18);
        cout << p[i].turnaround_time << setw(18);
        cout << p[i].waiting_time << endl;
    }
    cout << endl;
    cout << "Average Turnaround Time = " << avg_turnaround_time << endl;
    cout << "Average Waiting Time = " << avg_waiting_time << endl;
    cout << "Throughput = " << throughput << " process/unit time" << endl;
}

//*********************************  FCFS ****************************************//

void sort(Process* p, int n) {
    for (int i = 0; i < n - 1; i++) {

        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                int temp;
                //sorting burst times 
                temp = p[j].burst_Time;
                p[j].burst_Time = p[j + 1].burst_Time;
                p[j + 1].burst_Time = temp;
                //sorting arrival times 
                temp = p[j].arrival_time;
                p[j].arrival_time = p[j + 1].arrival_time;
                p[j + 1].arrival_time = temp;
                //sorting their respecompletion_timeive IDs 
                temp = p[j].pno;
                p[j].pno = p[j + 1].pno;
                p[j + 1].pno = temp;
            }
        }
    }
}
void show(Process* p, int n) {
    cout << endl;
    cout << "                           ***************************** FCFS ******************************" << endl;
    cout << endl;
    //cout << "Process\tArrival\tBurst\tWaiting\tTurn Around\tCompletion\n";
    cout << "Process" << setw(15) << "Arrival Time" << setw(15) << "Burst Time" << setw(15) << "Priority" << setw(15) << "Start Time" << setw(18) << "Completion Time" << setw(18) << "Turnaround Time" << setw(16) << "Waiting Time" << endl;
    cout << endl;
    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    float throughput = 0;
    int min_arrival_time = 10000000;
    int max_completion_time = -1;
    for (int i = 0; i < n; i++) {
        min_arrival_time = min(min_arrival_time, p[i].arrival_time);
        max_completion_time = max(max_completion_time, p[i].completion_time);
        total_waiting_time = total_waiting_time + p[i].waiting_time;
        total_turnaround_time = total_turnaround_time + p[i].turnaround_time;
        throughput = float(n) / (max_completion_time - min_arrival_time);
        cout << "  P[" << p[i].pno << "]  " << setw(10) << p[i].arrival_time << setw(15) << p[i].burst_Time << setw(15) << p[i].priority << setw(15) << p[i].start_time << setw(15) << p[i].completion_time << setw(15) << p[i].turnaround_time << setw(18) << p[i].waiting_time << "\n";
    }
    cout << endl << "Average turnaround time: " << float(total_turnaround_time / n);
    cout << endl << "Average waiting time: " << float(total_waiting_time / n);
    cout << endl;
    cout << "Throughput = " << throughput << " process/unit time" << endl;

}

bool compareArrival(Process p1, Process p2)
{
    return p1.arrival_time < p2.arrival_time;
}

bool compareID(Process p1, Process p2)
{
    return p1.pno < p2.pno;
}
void FCFS(Process* p, int n) {
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    float cpu_utilisation;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    float throughput;

    sort(p, p + n, compareArrival);

    for (int i = 0; i < n; i++) {
        p[i].start_time = (i == 0) ? p[i].arrival_time : max(p[i - 1].completion_time, p[i].arrival_time);
        p[i].completion_time = p[i].start_time + p[i].burst_Time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_Time;

        total_turnaround_time += p[i].turnaround_time;
        total_waiting_time += p[i].waiting_time;
    }

    avg_turnaround_time = (float)total_turnaround_time / n;
    avg_waiting_time = (float)total_waiting_time / n;
    throughput = float(n) / (p[n - 1].completion_time - p[0].arrival_time);

    sort(p, p + n, compareID);

    show(p, n);

}

