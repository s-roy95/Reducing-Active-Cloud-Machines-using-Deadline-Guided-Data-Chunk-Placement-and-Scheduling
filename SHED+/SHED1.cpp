
// Enhanced Shed+ Simulation with Online Progress, PoCD, Beta Estimation, and Greedy Replica Allocation

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>

using namespace std;

const int NUM_TASKS_POOL = 1000;
const int NUM_JOBS = 500;
const int TASKS_PER_JOB = 15;
const int TASKS_PER_VM_STORAGE = 60;  // B
const int TASKS_PARALLEL_VM = 10;      // S
const int DEADLINE = 2;               // in time units
const int MIN_EXEC_TIME = 1;
const int MAX_SPECULATIVE_ATTEMPTS = 1;
const int REEVALUATION_INTERVAL = 1;  // reevaluation interval
const double STRAGGLER_PROB = 0.1;

struct TaskInstance {
    int task_id;
    double progress = 0.0;
    double beta = 1.0;
    int speculative_attempts = 0;
    int total_copies = 1;  // includes original + speculative
    double pocd = 0.0;
};

struct VM {
    unordered_set<int> stored_tasks;
    int current_load = 0;
};

vector<vector<TaskInstance>> generateJobs() {
    vector<vector<TaskInstance>> jobs(NUM_JOBS);
    srand(time(0));
    for (int i = 0; i < NUM_JOBS; ++i) {
        unordered_set<int> unique_tasks;
        while (unique_tasks.size() < TASKS_PER_JOB) {
            unique_tasks.insert(rand() % NUM_TASKS_POOL);
        }

        for (int task : unique_tasks) {
            TaskInstance t;
            t.task_id = task;
            t.progress = ((double)rand() / RAND_MAX) * 0.5; // simulate up to 50% progress
            t.beta = DEADLINE / (DEADLINE - (1 - t.progress) * MIN_EXEC_TIME);
            jobs[i].push_back(t);
        }
    }
    return jobs;
}

double computePoCD(const TaskInstance& t) {
    double term = (1 - t.progress) * MIN_EXEC_TIME / (DEADLINE);
    return 1.0 - pow(term, t.beta * t.total_copies);
}

int simulateGreedySpeculation(vector<vector<TaskInstance>>& jobs, int& total_speculative_copies) {
    vector<VM> vms;
    total_speculative_copies = 0;

    int intervals = DEADLINE / REEVALUATION_INTERVAL;
    for (int interval = 0; interval < intervals; ++interval) {
        vector<pair<double, int>> priority_queue;
        vector<TaskInstance*> task_refs;

        // Evaluate PoCD and find tasks that would benefit most from speculation
        for (auto& job : jobs) {
            for (auto& task : job) {
                task.pocd = computePoCD(task);
                if (task.pocd < 0.80 && task.speculative_attempts < MAX_SPECULATIVE_ATTEMPTS) {
                    TaskInstance temp = task;
                    temp.speculative_attempts++;
                    temp.total_copies++;
                    double new_pocd = computePoCD(temp);
                    double gain = new_pocd - task.pocd;
                    task_refs.push_back(&task);
                    priority_queue.emplace_back(gain, task_refs.size() - 1);
                }
            }
        }

        // Sort by PoCD improvement descending
        sort(priority_queue.rbegin(), priority_queue.rend());

        // Greedily assign replicas
        for (auto& p : priority_queue) {
            double gain = p.first;
            int idx = p.second;
            TaskInstance* task = task_refs[idx];

            task->speculative_attempts++;
            task->total_copies++;
            total_speculative_copies++;

            bool assigned = false;
            for (auto& vm : vms) {
                bool has_task = vm.stored_tasks.count(task->task_id);
                bool can_store = (vm.stored_tasks.size() < TASKS_PER_VM_STORAGE);
                bool can_execute = (vm.current_load < TASKS_PARALLEL_VM * DEADLINE);

                if ((has_task || can_store) && can_execute) {
                    vm.stored_tasks.insert(task->task_id);
                    vm.current_load++;
                    assigned = true;
                    break;
                }
            }

            if (!assigned) {
                VM new_vm;
                new_vm.stored_tasks.insert(task->task_id);
                new_vm.current_load = 1;
                vms.push_back(new_vm);
            }
        }
    }

    // Assign original task copies
    for (auto& job : jobs) {
        for (auto& task : job) {
            for (int i = 0; i < 1; ++i) {
                bool assigned = false;
                for (auto& vm : vms) {
                    bool has_task = vm.stored_tasks.count(task.task_id);
                    bool can_store = (vm.stored_tasks.size() < TASKS_PER_VM_STORAGE);
                    bool can_execute = (vm.current_load < TASKS_PARALLEL_VM * DEADLINE);

                    if ((has_task || can_store) && can_execute) {
                        vm.stored_tasks.insert(task.task_id);
                        vm.current_load++;
                        assigned = true;
                        break;
                    }
                }

                if (!assigned) {
                    VM new_vm;
                    new_vm.stored_tasks.insert(task.task_id);
                    new_vm.current_load = 1;
                    vms.push_back(new_vm);
                }
            }
        }
    }

    return vms.size();
}

int main() {
    auto jobs = generateJobs();
    int total_speculative = 0;
    int vm_count = simulateGreedySpeculation(jobs, total_speculative);

    int jobs_meeting_deadline = 0;
    for (auto& job : jobs) {
        bool completed = true;
        for (auto& t : job) {
            if (computePoCD(t) < 0.80) {
                completed = false;
                break;
            }
        }
        if (completed) jobs_meeting_deadline++;
    }

    double pocd = (double)jobs_meeting_deadline / NUM_JOBS;

    cout << "\nEnhanced Shed+ Simulation:\n";
    cout << "Total VMs used: " << vm_count << endl;
    cout << "Total speculative copies: " << total_speculative << endl;
    cout << "Estimated PoCD: " << pocd * 100 << "%\n";

    return 0;
}
