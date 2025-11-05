#include <iostream>
#include <vector>
#include<set>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

/*Stores the chunks in job along with its deadline*/
vector<pair<vector<long long>,long long>> chunks;
long long B;
long long S;
long long C;
long long chunks_sum = 0;
int dmax;
bool comparePairs(const pair<vector<long long>, long long>& a, const pair<vector<long long>, long long>& b) {
    return a.second > b.second; // Sort in descending order based on second value of the pair
}

bool compareCustom(const pair<int, vector<int>>& a, const pair<int, vector<int>>& b) {
    return a.second.size() > b.second.size();
}

/*Function to check if all occurence of a single chunk can fit in one vm*/
bool fits_in_vm(vector<int>&v){
    sort(v.begin(),v.end());
    for(int i=0;i<v.size();i++){
        if(i+1 > v[i]){
            return false;
        }
    }
    return true;
}




/*The final schedule*/
vector<vector<vector<int>>> schedule;

/*Find free slot for a chunk in a given node with a given deeadline*/
/*Returns the deadline at which chunk is scheduled else gives -1*/
int find_free(int node, int chunk, int deadline, vector<vector<vector<int>>> sch){
    vector<set<int>> temp(deadline);
    set<int> tempp;
    for(int i = deadline-1;i>=0;i--){
        for(int k=0;k<S;k++){
            if(sch[node][k][i] != 0){
                temp[i].insert(sch[node][k][i]);
            }
        }
    }
    for(int i = dmax-1;i>=0;i--){
        for(int k=0;k<S;k++){
            if(sch[node][k][i] != 0){
                tempp.insert(sch[node][k][i]);
            }
        }
    }
    if(tempp.size()>=B && tempp.find(chunk) == tempp.end()){
        return -1;
    }

    
    for(int i = deadline-1;i>=0;i--){
        if(temp[i].find(chunk) == temp[i].end() && temp[i].size() < S){
            return i;
        }
    }
    return -1;
}

/*Checks if a chunk and all its occurence can be scheduled in a particular node*/
bool single_schedulable(int x,int node, vector<pair<int,vector<int>>> &chunk_deadline, int chunk, vector<int> list_deadline){
    bool scheduled = false;
    vector<vector<vector<int>>> sch = schedule;
    for(int j=0;j<list_deadline.size();j++){
        scheduled = false;
        if(find_free(node,chunk,list_deadline[j],sch) != -1){
            int col = find_free(node,chunk,list_deadline[j],sch);
            for(int k=0;k<S;k++){
                if(sch[node][k][col] == 0){
                    sch[node][k][col] = chunk;
                    scheduled = true;
                    break;
                }
            }
        }
        if(!scheduled){
            return false;
        }
    }
    // cout<<"Checking for chunk "<<chunk<<" at node "<<node<<endl;
    return true;
}


/*Schedules the chunks present in the vector*/
/*Always schedule all occurences of a chunk on a single node*/
/*chunk_deadline -> stores the {chunk,list of deadlines}  for all chunks which are single schedulable*/
bool singleVMscheduler(int x, vector<pair<int,vector<int>>> &chunk_deadline){
    bool scheduled = false;
    int node = 0;
    int going = 0;
    int last_scheduled = 0;
    for(auto it = chunk_deadline.begin();it!=chunk_deadline.end();it++){
        int chunk = it->first;
        vector<int> list_deadline = it->second;
        scheduled = false;
        if(going == 0){
            for(int i = node;i<x;i++){
                if(single_schedulable(x,i,chunk_deadline,chunk,list_deadline)){
                    scheduled = true;
                    for(int j=0;j<list_deadline.size();j++){
                        int col = find_free(i,chunk,list_deadline[j],schedule);
                        for(int k=0;k<S;k++){
                            if(schedule[i][k][col] == 0){
                                schedule[i][k][col] = chunk;
                                break;
                            }
                        }
                    }
                    node = i+1;
                    break;
                }
            }
        }
        
        if(node == x){
            going  = 1;
            node = x-1;
        }
        if(!scheduled && going == 1){
            for(int i = node;i>=0;i--){
                if(single_schedulable(x,i,chunk_deadline,chunk,list_deadline)){
                    scheduled = true;
                    for(int j=0;j<list_deadline.size();j++){
                        int col = find_free(i,chunk,list_deadline[j],schedule);
                        for(int k=0;k<S;k++){
                            if(schedule[i][k][col] == 0){
                                schedule[i][k][col] = chunk;
                                break;
                            }
                        }
                    }
                    node = i-1;
                    break;
                }
            }
        }
        if(node == -1){
            going = 0;
            node = 0;
        }
        if(!scheduled){
            return false;
        }

    }   
    return true;
}

/*Schedules the given chunk in x nodes in any random order*/
bool random_scheduler(int x,map<int,vector<int>> &chunk_deadline){
    dmax = chunks[0].second;
    int start_node=0;
    int next_node = 0;
    int next_vm = 0;
    int next_deadline = dmax-1;
    int J = chunks.size();
    bool scheduled = false;
    vector<vector<vector<int>>> sch(x, vector<vector<int>>(S, vector<int>(dmax)));
    schedule = sch;
    for(auto it = chunk_deadline.begin();it!=chunk_deadline.end();it++){
        int chunk = it->first;
        vector<int> list_deadline = it->second;
        for(int j=0;j<list_deadline.size();j++){
            scheduled = false;
            for(int node = 0;node<x;node++){
                if(find_free(node,chunk,list_deadline[j],schedule) != -1){
                    int col = find_free(node,chunk,list_deadline[j],schedule);
                    for(int k=0;k<S;k++){
                        if(schedule[node][k][col] == 0){
                            schedule[node][k][col] = chunk;
                            scheduled = true;
                            break;
                        }
                    }
                }
                if(scheduled)   break;
            }
            if(!scheduled){
                return false;
            }
        }
    }

    return true;
}

/*Schedules the given chunks in x nodes in sequential order*/
bool sequential_scheduler(int x, vector<pair<int,vector<int>>> &chunk_deadline){
    bool scheduled = false;
    int node = 0;
    int last_scheduled = 0;
    for(auto it = chunk_deadline.begin();it!=chunk_deadline.end();it++){
        int chunk = it->first;
        vector<int> list_deadline = it->second;
        for(int j=0;j<list_deadline.size();j++){
            scheduled = false;
            for(node = 0;node<x;node++){
                if(find_free(node,chunk,list_deadline[j],schedule) != -1){
                    int col = find_free(node,chunk,list_deadline[j],schedule);
                    for(int k=0;k<S;k++){
                        if(schedule[node][k][col] == 0){
                            schedule[node][k][col] = chunk;
                            scheduled = true;
                            break;
                        }
                    }
                }
                if(scheduled)   break;
            }
            if(!scheduled){
                return false;
            }
        }
    }   
    return true;
}

/*Schedules the given chunks in x nodes in a heuristic order*/
bool heuristic_scheduler(int x, map<int,vector<int>> &chunk_deadline){
    schedule.clear();
    dmax = chunks[0].second;
    vector<vector<vector<int>>> sch(x, vector<vector<int>>(S, vector<int>(dmax)));
    schedule = sch;
    vector<pair<int, vector<int>>> one_vm;     
    vector<pair<int, vector<int>>> more_vm;
    for(auto it = chunk_deadline.begin();it!=chunk_deadline.end();it++){
        if(fits_in_vm(it->second)){
            one_vm.push_back({it->first, it->second});
        }
        else{
            more_vm.push_back({it->first, it->second});
        }
    }
    sort(one_vm.begin(),one_vm.end(),compareCustom);
    sort(more_vm.begin(),more_vm.end(),compareCustom);
    return singleVMscheduler(x,one_vm) && sequential_scheduler(x,more_vm);
}

int main(){
    /*Stores the list of deadline corresponding to a particular chunk*/
    map<int,vector<int>> chunk_deadline;
    /*Stores the schedule for heuristic scheduler*/
    vector<vector<vector<int>>> ans_sch;
    /*Stores the schedule for random scheduler*/
    vector<vector<vector<int>>> ans_sch1;
    long long final_ans;
    ifstream inputFile("input.txt"); // Assuming input is stored in a file named "input.txt"
    if (!inputFile.is_open()) {
        cout << "Failed to open the input file." << endl;
        return 1;
    }

    
    inputFile >> C;


    for (long long i = 0; i < C; i++) {
        long long x, y, d;
        inputFile >> d >> x;
        chunks_sum += x;
        vector<long long> v;
        for (long long j = 0; j < x; j++) {
            inputFile >> y;
            v.push_back(y);
            chunk_deadline[y].push_back(d);
        }
        chunks.push_back({v, d});
    }

    inputFile >> S >> B;

    inputFile.close();
    
    sort(chunks.begin(), chunks.end(), comparePairs);
    

    long long left = 1;
    long long right = chunks_sum;
    long long ans = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (heuristic_scheduler(mid,chunk_deadline)) {
            ans = mid;
            right = mid - 1;
            ans_sch = schedule;
        }
        else {
            left = mid + 1;
        }
    }
    final_ans = ans;

    
    left = 1;
    right = chunks_sum;
    ans = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (random_scheduler(mid,chunk_deadline)) {
            ans = mid;
            right = mid - 1;
            ans_sch1 = schedule;
        }
        else {
            left = mid + 1;
        }
    }

    if(ans<final_ans){
        for (int layer = 0; layer < ans; ++layer) {
            cout << "Node " << layer+1 << ":" << endl;
            for (int row = 0; row < S; ++row) {
                cout<<"VM "<<row+1<<":  ";
                for (int col = 0; col < dmax; ++col) {
                    cout << ans_sch[layer][row][col] << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
        cout<<ans<<endl;
    }
    else{
        for (int layer = 0; layer < final_ans; ++layer) {
            cout << "Node " << layer+1 << ":" << endl;
            for (int row = 0; row < S; ++row) {
                cout<<"VM "<<row+1<<":  ";
                for (int col = 0; col < dmax; ++col) {
                    cout << ans_sch1[layer][row][col] << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
        cout<<final_ans<<endl;
    }
    return 0;
}