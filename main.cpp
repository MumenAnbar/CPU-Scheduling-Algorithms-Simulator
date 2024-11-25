#include <iostream>
#include <bits/stdc++.h>

using namespace std;

struct Process {

    int process_num ;
    int arrivalTime ;
    int burstTime ;
    int comesBackAfter ;
    int priority ;
    int waitingTime ;
    int inReadyQueue ;
    int lastExitTime ;

};

struct Process* CreateProcess(int p_number, int a_time, int b_time, int c_time, int prio) {

    struct Process* temp ;
    temp = (struct Process*)malloc(sizeof(struct Process)) ;
    temp->process_num = p_number ;
    temp->arrivalTime = a_time ;
    temp->burstTime = b_time ;
    temp->comesBackAfter = c_time ;
    temp->priority = prio ;
    temp->waitingTime = 0 ;
    temp->lastExitTime = -1 ;
    temp->inReadyQueue = 0 ;

    return temp ;

}

void printAverage_waiting_turnaround_Time ( vector <struct Process*> processes ){

    double waitingTimeSum = 0 ;
    double turnaroundTimeSum = 0 ;
    int nonExcutedProcesses = 0 ;


    for (int i = 0 ; i < processes.size() ; i ++ ){


        //waitingTimeSum += processes[i] -> waitingTime - ( 200 - min( 200 , processes[i] -> lastExitTime + processes[i] -> comesBackAfter )) ;
        cout <<  processes[i] -> waitingTime - ( 200 - min( 200 , processes[i] -> lastExitTime + processes[i] -> comesBackAfter )) << endl ;
        cout <<  (processes[i] -> lastExitTime) - (processes[i] -> arrivalTime) << endl ;
        if ( processes[i] -> lastExitTime != -1 ){
            turnaroundTimeSum += (processes[i] -> lastExitTime) - (processes[i] -> arrivalTime) ;
            waitingTimeSum += processes[i] -> waitingTime - ( 200 - min( 200 , processes[i] -> lastExitTime + processes[i] -> comesBackAfter ));
            cout << processes[i] -> waitingTime - ( 200 - min( 200 , processes[i] -> lastExitTime + processes[i] -> comesBackAfter )) << "---<\n";
        }
        else
            nonExcutedProcesses ++ ;

    }

    double numOfProcesses = processes.size() ;

    cout << "Average waiting time = " << waitingTimeSum/(numOfProcesses - nonExcutedProcesses) << endl  ;
    cout << "Average turnaround time = " << turnaroundTimeSum/( numOfProcesses - nonExcutedProcesses ) << endl ;

}

void printGantChart( vector < pair <int , int> > timeChunks , vector <int> processesExecuted ){


    cout << "GANTT CHART:" << endl ;

    int Size = timeChunks.size() ;

    for (int i = 0 ; i < Size ; i ++  ){

        cout << timeChunks[i].first <<"  P" << processesExecuted[i] << "  " ;

    }
    cout << timeChunks[Size - 1 ].second << endl  ;



}


void Fcfs_simulation() {

    vector<struct Process*> processes;

    processes.push_back(CreateProcess(1, 0, 10, 2, 3));
    processes.push_back(CreateProcess(2, 1, 8, 4, 2));
    processes.push_back(CreateProcess(3, 3, 14, 6, 3));
    processes.push_back(CreateProcess(4, 4, 7, 8, 1));
    processes.push_back(CreateProcess(5, 6, 5, 3, 0));
    processes.push_back(CreateProcess(6, 7, 4, 6, 1));
    processes.push_back(CreateProcess(7, 8, 6, 9, 2));

    sort(processes.begin(), processes.end(), [](const struct Process* one, const struct Process* two) { return one->arrivalTime < two->arrivalTime; });

    int time = 0;
    int currentProcessNumber = -1 ;
    queue<int> ready_queue;
    int currentBurstTime ;
    map<int, int> waitQueue;
    int Chunk_startTime = 0;
    vector<int> currentArrivals;
    vector<pair<int, int>> TimeChunks;
    vector<int> processExecuted;
    map <int , struct Process* > id_process ;

    for (int i = 0; i < processes.size(); i++) {


        waitQueue[processes[i] -> process_num] = processes[i] -> arrivalTime ;
        id_process[processes[i] -> process_num] = processes[i] ;

    }

    int initialProcess = 1 ;
    int processEnded = 0 ;

    while ( time <= 200 ){

        for( auto& p : waitQueue ){

            if ( p.second == 0 ){
                p.second = -1 ;
                currentArrivals.push_back(p.first) ;
            }

        }

        for (int i = 0 ; i < currentArrivals.size() ; i ++ ){

            ready_queue.push(currentArrivals[i]) ;
            id_process[currentArrivals[i]] -> inReadyQueue = 1 ;

        }
        currentArrivals.clear() ;

        if ( initialProcess && ready_queue.size() > 0 ){

            initialProcess = 0 ;

            currentProcessNumber = ready_queue.front() ;
            ready_queue.pop();
            id_process[currentProcessNumber] -> inReadyQueue = 0 ;

            processExecuted.push_back(currentProcessNumber) ;
            currentBurstTime = id_process[currentProcessNumber] -> burstTime ;

        }

        if ( processEnded ){

            if (currentProcessNumber != -1){

                TimeChunks.push_back({Chunk_startTime , time}) ;
                Chunk_startTime = time ;

            }

            id_process[currentProcessNumber] -> lastExitTime = time ;
            waitQueue[currentProcessNumber] = id_process[currentProcessNumber] -> comesBackAfter ;
            currentProcessNumber = -1 ;

            if ( ready_queue.size() > 0 ){

                currentProcessNumber = ready_queue.front() ;
                ready_queue.pop() ;
                id_process[currentProcessNumber] -> inReadyQueue = 0 ;

                if ( time != Chunk_startTime ){

                    processExecuted.push_back(-1) ;
                    TimeChunks.push_back( {Chunk_startTime , time}) ;


                }

                Chunk_startTime = time ;
                processExecuted.push_back(currentProcessNumber) ;
                currentBurstTime = id_process[currentProcessNumber] -> burstTime ;
                processEnded = 0 ;

            }

        }

        currentBurstTime -- ;
        time ++ ;

        if ( time > 200 ){
            break ;
        }

        if ( currentBurstTime == 0 ){

            processEnded = 1 ;

        }


        for (auto &p : waitQueue ){

            if ( p.second != -1 ){
                p.second = p.second - 1 ;
            }

        }

        for (auto& p : processes ){

            if ( p -> inReadyQueue == 1  ){

                p -> waitingTime = p -> waitingTime + 1 ;

            }

        }

    }

    if ( processExecuted.size() > TimeChunks.size() ){

        TimeChunks.push_back({Chunk_startTime , time - 1}) ;


        if (currentProcessNumber == -1 ){
            processExecuted.push_back(-1) ;
        }
        else{
            id_process[currentProcessNumber] -> lastExitTime = 200 ;
        }

    }

    printGantChart( TimeChunks , processExecuted) ;
    cout << endl ;
    printAverage_waiting_turnaround_Time(processes) ;

}

struct CompareTuples {

    bool operator()(const tuple<int, int, int>& one, const tuple<int, int, int>& two) const {

        if (get<0>(one) != get<0>(two)) {
            return get<0>(one) > get<0>(two); // Order by the first element in ascending order
        }

        if (get<1>(one) != get<1>(two)) {
            return get<1>(one) > get<1>(two);
        }

        return get<2>(one) > get<2>(two);
    }

};


void Sjf(){

    vector<struct Process*> processes;

    processes.push_back(CreateProcess(1, 0, 10, 2, 3));
    processes.push_back(CreateProcess(2, 1, 8, 4, 2));
    processes.push_back(CreateProcess(3, 3, 14, 6, 3));
    processes.push_back(CreateProcess(4, 4, 7, 8, 1));
    processes.push_back(CreateProcess(5, 6, 5, 3, 0));
    processes.push_back(CreateProcess(6, 7, 4, 6, 1));
    processes.push_back(CreateProcess(7, 8, 6, 9, 2));


    map <int , int > waitingTime ;
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, CompareTuples> readyQueue ;// burstTime , time is enters , processNumber
    int currentProcessNum = -1 ;
    int currentProcessBurstTime = -1;
    int time = 0 ;
    vector<pair<int, int>> TimeChunks ;
    vector<int> processExecuted ;
    int Chunk_startTime = 0 ;
    int processesEnded = 1 ;
    map <int , struct Process* > id_process ;

    for (int i = 0 ; i < processes.size() ; i ++ ){

        waitingTime[processes[i] -> process_num ] = processes[i] -> arrivalTime ;
        id_process[processes[i] -> process_num ] = processes[i] ;

    }


    while ( time <= 200 ){
        //cout << "this is time" << time << endl ;
        //cout << "this is ready queue size " << readyQueue.size() << endl ;
        //cout << "this is current process executing " << currentProcessNum << endl ;
        for (auto& p : waitingTime ){

            if ( p.second == 0 ){

                struct Process* current = id_process[p.first] ;
                readyQueue.push({current ->burstTime , time , current ->process_num }) ;
                current -> inReadyQueue = 1 ;
                p.second = -1 ;

            }

        }

        if ( processesEnded == 1 ){

            if ( currentProcessNum == -1 ){

                if ( readyQueue.size() > 0 ){

                    Chunk_startTime = time ;

                    tuple <int,int,int> temp = readyQueue.top() ;
                    currentProcessBurstTime =  get<0>(temp) ;
                    currentProcessNum = get<2>(temp) ;
                    readyQueue.pop() ;
                    id_process[currentProcessNum] -> inReadyQueue = 0 ;

                    processesEnded = 0 ;
                    processExecuted.push_back(currentProcessNum) ;

                }

            }
            else{

                TimeChunks.push_back({Chunk_startTime , time}) ;
                waitingTime[currentProcessNum] = id_process[currentProcessNum] ->comesBackAfter ;
                id_process[currentProcessNum] -> lastExitTime = time ;

                if ( readyQueue.size() > 0 ){

                    Chunk_startTime = time ;

                    tuple <int,int,int> temp = readyQueue.top() ;
                    currentProcessBurstTime =  get<0>(temp) ;
                    currentProcessNum = get<2>(temp) ;
                    readyQueue.pop() ;
                    id_process[currentProcessNum] -> inReadyQueue = 0 ;

                    processesEnded = 0 ;
                    processExecuted.push_back(currentProcessNum) ;

                }
                else{
                    currentProcessNum = -1 ;
                }

            }

        }

        currentProcessBurstTime -- ;
        time ++ ;

        if ( time > 200 ){
            break ;
        }

        if ( currentProcessBurstTime == 0 ){
            processesEnded = 1 ;
        }

        for (auto &p : waitingTime ){

            if ( p.second != -1 ){
                p.second = p.second - 1 ;
            }

        }

        for (auto& p : processes ){

            if ( p -> inReadyQueue == 1 ){

                p -> waitingTime = ( p -> waitingTime ) + 1 ;

            }

        }

    }

    if ( processExecuted.size() > TimeChunks.size() ){

        TimeChunks.push_back({Chunk_startTime , time - 1}) ;

        if (currentProcessNum == -1 ){
            processExecuted.push_back(-1) ;
        }
        else{
            id_process[currentProcessNum] -> lastExitTime = time ;
        }

    }

    printGantChart( TimeChunks , processExecuted) ;
    cout << endl ;
    printAverage_waiting_turnaround_Time(processes) ;

}


void ShortestRemainingTimeFirst(){

    vector<struct Process*> processes;

    processes.push_back(CreateProcess(1, 0, 10, 2, 3));
    processes.push_back(CreateProcess(2, 1, 8, 4, 2));
    processes.push_back(CreateProcess(3, 3, 14, 6, 3));
    processes.push_back(CreateProcess(4, 4, 7, 8, 1));
    processes.push_back(CreateProcess(5, 6, 5, 3, 0));
    processes.push_back(CreateProcess(6, 7, 4, 6, 1));
    processes.push_back(CreateProcess(7, 8, 6, 9, 2));


    map <int , int > waitingTime ;
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, CompareTuples> readyQueue ;// burstTime , time is enters , processNumber
    int currentProcessNum = -1 ;
    int currentProcessBurstTime = -1;
    int time = 0 ;
    vector<pair<int, int>> TimeChunks ;
    vector<int> processExecuted ;
    int Chunk_startTime = 0 ;
    int processesEnded = 1 ;
    map <int , struct Process* > id_process ;

    for (int i = 0 ; i < processes.size() ; i ++ ){

        waitingTime[processes[i] -> process_num ] = processes[i] -> arrivalTime ;
        id_process[processes[i] -> process_num ] = processes[i] ;

    }


    while ( time <= 200 ){
        //cout << "this is time" << time << endl ;
        //cout << "this is ready queue size " << readyQueue.size() << endl ;
        //cout << "this is current process executing " << currentProcessNum << endl ;
        for (auto& p : waitingTime ){

            if ( p.second == 0 ){

                struct Process* current = id_process[p.first] ;
                readyQueue.push({current ->burstTime , time , current ->process_num }) ;
                id_process[current ->process_num ] -> inReadyQueue = 1 ;
                p.second = -1 ;

            }

        }

        if ( readyQueue.size() > 0 && currentProcessNum != -1 && processesEnded != 1 ){

            tuple <int , int ,int > readyQueueFront = readyQueue.top() ;

            if ( get<0>(readyQueueFront) < currentProcessBurstTime ){

                TimeChunks.push_back({ Chunk_startTime , time }) ;
                Chunk_startTime = time ;

                readyQueue.pop() ;
                id_process[currentProcessNum] -> lastExitTime = time ;
                id_process[currentProcessNum] -> inReadyQueue = 1 ;
                readyQueue.push({currentProcessBurstTime , time , currentProcessNum} ) ;

                currentProcessBurstTime =  get<0>(readyQueueFront) ;
                currentProcessNum = get<2>(readyQueueFront) ;
                processExecuted.push_back(currentProcessNum) ;
                id_process[currentProcessNum] -> inReadyQueue = 0 ;

            }

        }




        if ( processesEnded == 1 ){

            if ( currentProcessNum == -1 ){

                if ( readyQueue.size() > 0 ){

                    Chunk_startTime = time ;

                    tuple <int,int,int> temp = readyQueue.top() ;
                    currentProcessBurstTime =  get<0>(temp) ;
                    currentProcessNum = get<2>(temp) ;
                    readyQueue.pop() ;
                    id_process[currentProcessNum] -> inReadyQueue = 0 ;

                    processesEnded = 0 ;
                    processExecuted.push_back(currentProcessNum) ;

                }

            }
            else{

                TimeChunks.push_back({Chunk_startTime , time}) ;
                waitingTime[currentProcessNum] = id_process[currentProcessNum] ->comesBackAfter ;
                id_process[currentProcessNum] -> lastExitTime = time ;

                if ( readyQueue.size() > 0 ){

                    Chunk_startTime = time ;

                    tuple <int,int,int> temp = readyQueue.top() ;
                    currentProcessBurstTime =  get<0>(temp) ;
                    currentProcessNum = get<2>(temp) ;
                    readyQueue.pop() ;
                    id_process[currentProcessNum] -> inReadyQueue = 0 ;

                    processesEnded = 0 ;
                    processExecuted.push_back(currentProcessNum) ;

                }
                else{
                    currentProcessNum = -1 ;
                }

            }

        }

        currentProcessBurstTime -- ;
        time ++ ;

        if ( time > 200 ){
            break ;
        }

        if ( currentProcessBurstTime == 0 ){
            processesEnded = 1 ;
        }

        for (auto &p : waitingTime ){

            if ( p.second != -1 ){
                p.second = p.second - 1 ;
            }

        }

        for (auto& p : processes ){

            if ( p -> inReadyQueue == 1 ){

                p -> waitingTime = ( p -> waitingTime ) + 1 ;

            }

        }

    }

    if ( processExecuted.size() > TimeChunks.size() ){

        TimeChunks.push_back({Chunk_startTime , time - 1}) ;

        if (currentProcessNum == -1 ){
            processExecuted.push_back(-1) ;
        }
        else {
            id_process[currentProcessNum] -> lastExitTime = 200 ;
        }
    }

    printGantChart( TimeChunks , processExecuted) ;
    cout << endl ;
    printAverage_waiting_turnaround_Time(processes) ;

}



void RR_simulation() {

    vector<struct Process*> processes;

    processes.push_back(CreateProcess(1, 0, 10, 2, 3));
    processes.push_back(CreateProcess(2, 1, 8, 4, 2));
    processes.push_back(CreateProcess(3, 3, 14, 6, 3));
    processes.push_back(CreateProcess(4, 4, 7, 8, 1));
    processes.push_back(CreateProcess(5, 6, 5, 3, 0));
    processes.push_back(CreateProcess(6, 7, 4, 6, 1));
    processes.push_back(CreateProcess(7, 8, 6, 9, 2));

    sort(processes.begin(), processes.end(), [](const struct Process* one, const struct Process* two) { return one->arrivalTime < two->arrivalTime; });

    int time = 0;
    int currentProcessNumber = -1 ;
    queue<pair <int ,int > > ready_queue;
    int currentBurstTime ;
    map<int, int> waitQueue;
    int Chunk_startTime = 0;
    vector<int> currentArrivals;
    vector<pair<int, int>> TimeChunks;
    vector<int> processExecuted;
    map <int , struct Process* > id_process ;

    for (int i = 0; i < processes.size(); i++) {

        waitQueue[processes[i] -> process_num] = processes[i] -> arrivalTime ;
        id_process[processes[i] -> process_num] = processes[i] ;

    }

    int initialProcess = 1 ;
    int processEnded = 0 ;
    int qCounter = 0 ;

    while ( time <= 200 ){

        for( auto& p : waitQueue ){

            if ( p.second == 0 ){
                p.second = -1 ;
                currentArrivals.push_back(p.first) ;
            }

        }

        for (int i = 0 ; i < currentArrivals.size() ; i ++ ){

            ready_queue.push( { currentArrivals[i]  ,  id_process[currentArrivals[i]] -> burstTime } ) ;
            id_process[currentArrivals[i]] -> inReadyQueue = 1 ;

        }
        currentArrivals.clear() ;

        if ( qCounter == 5 && processEnded != 1 ){

            if (currentProcessNumber != -1){

                TimeChunks.push_back({Chunk_startTime , time}) ;
                Chunk_startTime = time ;

            }

            ready_queue.push({currentProcessNumber , currentBurstTime}) ;
            id_process[currentProcessNumber] -> inReadyQueue = 1 ;
            id_process[currentProcessNumber] -> lastExitTime = time ;

            currentProcessNumber = ready_queue.front().first ;
            currentBurstTime = ready_queue.front().second ;
            ready_queue.pop() ;
            id_process[currentProcessNumber] -> inReadyQueue = 0 ;

            processExecuted.push_back(currentProcessNumber) ;
            qCounter = 0 ;

        }

        if ( initialProcess && ready_queue.size() > 0 ){

            initialProcess = 0 ;

            currentProcessNumber = ready_queue.front().first ;
            currentBurstTime =  ready_queue.front().second ;
            ready_queue.pop();
            id_process[currentProcessNumber] -> inReadyQueue = 0 ;

            processExecuted.push_back(currentProcessNumber) ;
            processEnded = 0 ;
            qCounter = 0 ;


        }

        if ( processEnded ){



            if (currentProcessNumber != -1){

                TimeChunks.push_back({Chunk_startTime , time}) ;
                Chunk_startTime = time ;
                id_process[currentProcessNumber] -> lastExitTime = time ;

            }


            waitQueue[currentProcessNumber] = id_process[currentProcessNumber] -> comesBackAfter ;
            currentProcessNumber = -1 ;

            if ( ready_queue.size() > 0 ){

                currentProcessNumber = ready_queue.front().first ;
                id_process[currentProcessNumber] -> inReadyQueue = 0 ;

                if ( time != Chunk_startTime ){

                    processExecuted.push_back(-1) ;
                    TimeChunks.push_back( {Chunk_startTime , time}) ;


                }

                Chunk_startTime = time ;
                processExecuted.push_back(currentProcessNumber) ;
                currentBurstTime = ready_queue.front().second ;
                processEnded = 0 ;
                qCounter = 0 ;
                ready_queue.pop() ;

            }

        }

        currentBurstTime -- ;
        time ++ ;
        qCounter ++ ;

        if ( time > 200 ){
            break ;
        }

        if ( currentBurstTime == 0 ){

            processEnded = 1 ;

        }


        for (auto &p : waitQueue ){

            if ( p.second != -1 ){
                p.second = p.second - 1 ;
            }

        }

        for (auto& p : processes ){

            if ( p -> inReadyQueue == 1  ){

                p -> waitingTime = ( p -> waitingTime ) + 1 ;

            }

        }

    }

    if ( processEnded != 0 || processExecuted.size() > TimeChunks.size() ){

        TimeChunks.push_back({Chunk_startTime , time - 1}) ;

        if (currentProcessNumber == -1 ){
            processExecuted.push_back(-1) ;
        }
        else {
            id_process[currentProcessNumber] -> lastExitTime = 200 ;
        }

    }

    printGantChart( TimeChunks , processExecuted) ;
    cout << endl ;
    printAverage_waiting_turnaround_Time(processes) ;

}


map <int , int > timeInReadyQueuePreemptive  ;

struct CompareTuplesFor_psp {

    bool operator()(const tuple<int, int, int>& one, const tuple<int, int, int>& two) const {

        if (  max (get<0>(one) - (timeInReadyQueuePreemptive[ get<2>(one) ] / 5 ) , 0 ) != max(get<0>(two) - ( timeInReadyQueuePreemptive[ get<2>(two) ] /5) , 0 ) ) {
            return max (get<0>(one) - (timeInReadyQueuePreemptive[ get<2>(one) ] / 5 ) , 0 ) > max(get<0>(two) - ( timeInReadyQueuePreemptive[ get<2>(two) ] /5) , 0 ); // Order by the first element in ascending order
        }

        if (get<1>(one) != get<1>(two)) {
            return get<1>(one) > get<1>(two);
        }

        return get<2>(one) > get<2>(two);
    }

};


priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,CompareTuplesFor_psp> updateReadyQueue( priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,CompareTuplesFor_psp> readyQueue ){

    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,CompareTuplesFor_psp> temp ;

    while ( readyQueue.size() > 0 ){

        //cout << "process " << get<2>(readyQueue.top()) << " : priority = " << get<0>(readyQueue.top()) << endl ;
        temp.push(readyQueue.top()) ;
        readyQueue.pop() ;


    }
    return temp ;

}

void prioritySchdulingPreemptive(){

    vector<struct Process*> processes;

    processes.push_back(CreateProcess(1, 0, 10, 2, 3));
    processes.push_back(CreateProcess(2, 1, 8, 4, 2));
    processes.push_back(CreateProcess(3, 3, 14, 6, 3));
    processes.push_back(CreateProcess(4, 4, 7, 8, 1));
    processes.push_back(CreateProcess(5, 6, 5, 3, 0));
    processes.push_back(CreateProcess(6, 7, 4, 6, 1));
    processes.push_back(CreateProcess(7, 8, 6, 9, 2));


    map <int , int > waitingTime ;
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,CompareTuplesFor_psp> readyQueue ;// burstTime , time is enters , processNumber
    int currentProcessNum = -1 ;
    int currentProcessBurstTime = -1;
    int time = 0 ;
    vector<pair<int, int>> TimeChunks ;
    vector<int> processExecuted ;
    int Chunk_startTime = 0 ;
    int processesEnded = 1 ;
    map <int , struct Process* > id_process ;
    map <int , int > preemptidBurstTime ;

    for (int i = 0 ; i < processes.size() ; i ++ ){

        waitingTime[processes[i] -> process_num ] = processes[i] -> arrivalTime ;
        id_process[processes[i] -> process_num ] = processes[i] ;
        timeInReadyQueuePreemptive[processes[i] -> process_num ] = -1 ;
        preemptidBurstTime[processes[i] -> process_num] = -1 ;

    }


    while ( time <= 200 ){

        readyQueue = updateReadyQueue(readyQueue) ;

        for (auto& p : waitingTime ){

            if ( p.second == 0 ){

                struct Process* current = id_process[p.first] ;
                readyQueue.push({current ->priority , time , current ->process_num }) ;
                id_process[current -> process_num ] -> inReadyQueue = 1 ;
                p.second = -1 ;
                timeInReadyQueuePreemptive[current -> process_num] = 0 ;

            }

        }

        if ( readyQueue.size() > 0 && currentProcessNum != -1 && processesEnded != 1 ){

            tuple <int , int ,int > readyQueueFront = readyQueue.top() ;

            if ( get<0>(readyQueueFront) < id_process[currentProcessNum] -> priority ){

                TimeChunks.push_back({ Chunk_startTime , time }) ;
                Chunk_startTime = time ;

                readyQueue.pop() ;
                readyQueue.push({id_process[currentProcessNum] -> priority , time , currentProcessNum} ) ;
                id_process[currentProcessNum] -> inReadyQueue = 1 ;
                id_process[currentProcessNum] -> lastExitTime = time ;
                timeInReadyQueuePreemptive[currentProcessNum] = 0 ;
                preemptidBurstTime[currentProcessNum] = currentProcessBurstTime ;

                currentProcessNum = get<2>(readyQueueFront) ;
                if ( preemptidBurstTime[currentProcessNum] == -1 )
                    currentProcessBurstTime = id_process[currentProcessNum] -> burstTime ;
                else
                    currentProcessBurstTime = preemptidBurstTime[currentProcessNum] ;
                processExecuted.push_back(currentProcessNum) ;
                timeInReadyQueuePreemptive[currentProcessNum] = -1 ;
                id_process[currentProcessNum] -> inReadyQueue = 0 ;

            }

        }


        if ( processesEnded == 1 ){

            if ( currentProcessNum == -1 ){

                if ( readyQueue.size() > 0 ){

                    Chunk_startTime = time ;

                    tuple <int,int,int> temp = readyQueue.top() ;
                    currentProcessNum = get<2>(temp) ;
                    if ( preemptidBurstTime[currentProcessNum] == -1 )
                        currentProcessBurstTime = id_process[currentProcessNum] -> burstTime ;
                    else
                        currentProcessBurstTime = preemptidBurstTime[currentProcessNum] ;

                    readyQueue.pop() ;
                    id_process[currentProcessNum] -> inReadyQueue = 0 ;
                    processesEnded = 0 ;
                    processExecuted.push_back(currentProcessNum) ;
                    timeInReadyQueuePreemptive[currentProcessNum] = -1 ;

                }

            }

            else{

                TimeChunks.push_back({Chunk_startTime , time}) ;
                waitingTime[currentProcessNum] = id_process[currentProcessNum] ->comesBackAfter ;
                id_process[currentProcessNum] -> lastExitTime = time ;
                preemptidBurstTime[currentProcessNum] = -1 ;

                if ( readyQueue.size() > 0 ){

                    Chunk_startTime = time ;

                    tuple <int,int,int> temp = readyQueue.top() ;
                    currentProcessNum = get<2>(temp) ;
                    if ( preemptidBurstTime[currentProcessNum] == -1 )
                        currentProcessBurstTime = id_process[currentProcessNum] -> burstTime ;
                    else
                        currentProcessBurstTime = preemptidBurstTime[currentProcessNum] ;

                    readyQueue.pop() ;
                    id_process[currentProcessNum] -> inReadyQueue = 0 ;
                    processesEnded = 0 ;
                    processExecuted.push_back(currentProcessNum) ;
                    timeInReadyQueuePreemptive[currentProcessNum] = -1 ;

                }

                else{
                    currentProcessNum = -1 ;
                }

            }

        }

        currentProcessBurstTime -- ;
        time ++ ;

        if ( time > 200 ){
            break ;
        }

        if ( currentProcessBurstTime == 0 ){
            processesEnded = 1 ;
        }

        for (auto &p : waitingTime ){

            if ( p.second != -1 ){
                p.second = p.second - 1 ;
            }

        }

        for (auto& p : processes ){

            if ( p -> inReadyQueue == 1 ){

                p -> waitingTime = ( p -> waitingTime ) + 1 ;

            }

        }

        for (auto& p : timeInReadyQueuePreemptive){

            if ( p.second != -1 ){
                p.second = p.second + 1 ;
            }

        }

    }

    if ( processExecuted.size() > TimeChunks.size() ){

        TimeChunks.push_back({Chunk_startTime , time - 1}) ;

        if (currentProcessNum == -1 ){
            processExecuted.push_back(-1) ;
        }
        else {
            id_process[currentProcessNum] -> lastExitTime = 200 ;
        }

    }

    printGantChart( TimeChunks , processExecuted) ;
    cout << endl ;
    printAverage_waiting_turnaround_Time(processes) ;

}

map <int , int > timeInReadyQueueNonPreemptive  ;

struct CompareTuplesFor_psnp {

    bool operator()(const tuple<int, int, int>& one, const tuple<int, int, int>& two) const {

        if (  max (get<0>(one) - (timeInReadyQueueNonPreemptive[ get<2>(one) ] / 5 ) , 0 ) != max(get<0>(two) - ( timeInReadyQueueNonPreemptive[ get<2>(two) ] /5) , 0 ) ) {
            return max (get<0>(one) - (timeInReadyQueueNonPreemptive[ get<2>(one) ] / 5 ) , 0 ) > max(get<0>(two) - ( timeInReadyQueueNonPreemptive[ get<2>(two) ] /5) , 0 ); // Order by the first element in ascending order
        }

        if (get<1>(one) != get<1>(two)) {
            return get<1>(one) > get<1>(two);
        }

        return get<2>(one) > get<2>(two);
    }

};



priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,CompareTuplesFor_psnp> updateReadyQueueNonPreemptive ( priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,CompareTuplesFor_psnp> readyQueue ){

    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,CompareTuplesFor_psnp> temp ;

    while ( readyQueue.size() > 0 ){

        //cout << "process " << get<2>(readyQueue.top()) << " : priority = " << get<0>(readyQueue.top()) << endl ;
        temp.push(readyQueue.top()) ;
        readyQueue.pop() ;


    }
    return temp ;

}

void prioritySchdulingNonPreemptive(){

    vector<struct Process*> processes;

    processes.push_back(CreateProcess(1, 0, 10, 2, 3));
    processes.push_back(CreateProcess(2, 1, 8, 4, 2));
    processes.push_back(CreateProcess(3, 3, 14, 6, 3));
    processes.push_back(CreateProcess(4, 4, 7, 8, 1));
    processes.push_back(CreateProcess(5, 6, 5, 3, 0));
    processes.push_back(CreateProcess(6, 7, 4, 6, 1));
    processes.push_back(CreateProcess(7, 8, 6, 9, 2));


    map <int , int > waitingTime ;
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,CompareTuplesFor_psnp> readyQueue ;// burstTime , time is enters , processNumber
    int currentProcessNum = -1 ;
    int currentProcessBurstTime = -1;
    int time = 0 ;
    vector<pair<int, int>> TimeChunks ;
    vector<int> processExecuted ;
    int Chunk_startTime = 0 ;
    int processesEnded = 1 ;
    map <int , struct Process* > id_process ;

    for (int i = 0 ; i < processes.size() ; i ++ ){

        waitingTime[processes[i] -> process_num ] = processes[i] -> arrivalTime ;
        id_process[processes[i] -> process_num ] = processes[i] ;
        timeInReadyQueueNonPreemptive[processes[i] -> process_num ] = -1 ;

    }


    while ( time <= 200 ){
        //cout << "this is time" << time << endl ;
        //cout << "this is ready queue size " << readyQueue.size() << endl ;
        //cout << "this is current process executing " << currentProcessNum << endl ;
        //cout << "this is time = " << time << endl  ;
        readyQueue = updateReadyQueueNonPreemptive(readyQueue) ;

        for (auto& p : waitingTime ){

            if ( p.second == 0 ){

                struct Process* current = id_process[p.first] ;
                readyQueue.push({current ->priority , time , current ->process_num }) ;
                p.second = -1 ;
                timeInReadyQueueNonPreemptive[current -> process_num] = 0 ;
                id_process[current ->process_num ] -> inReadyQueue = 1 ;

            }

        }

        if ( processesEnded == 1 ){

            if ( currentProcessNum == -1 ){

                if ( readyQueue.size() > 0 ){

                    Chunk_startTime = time ;

                    tuple <int,int,int> temp = readyQueue.top() ;
                    currentProcessNum = get<2>(temp) ;
                    currentProcessBurstTime =  id_process[currentProcessNum] -> burstTime ;

                    readyQueue.pop() ;
                    id_process[currentProcessNum] -> inReadyQueue = 0 ;
                    processesEnded = 0 ;
                    processExecuted.push_back(currentProcessNum) ;
                    timeInReadyQueueNonPreemptive[currentProcessNum] = -1 ;

                }

            }
            else{

                TimeChunks.push_back({Chunk_startTime , time}) ;
                waitingTime[currentProcessNum] = id_process[currentProcessNum] ->comesBackAfter ;
                id_process[currentProcessNum] -> lastExitTime = time ;

                if ( readyQueue.size() > 0 ){

                    Chunk_startTime = time ;

                    tuple <int,int,int> temp = readyQueue.top() ;
                    currentProcessNum = get<2>(temp) ;
                    currentProcessBurstTime =  id_process[currentProcessNum] -> burstTime ;

                    readyQueue.pop() ;
                    id_process[currentProcessNum] -> inReadyQueue = 0 ;
                    processesEnded = 0 ;
                    processExecuted.push_back(currentProcessNum) ;
                    timeInReadyQueueNonPreemptive[currentProcessNum] = -1 ;

                }
                else{
                    currentProcessNum = -1 ;
                }

            }

        }

        currentProcessBurstTime -- ;
        time ++ ;

        if ( time > 200 ){
            break ;
        }

        if ( currentProcessBurstTime == 0 ){
            processesEnded = 1 ;
        }

        for (auto &p : waitingTime ){

            if ( p.second != -1 ){
                p.second = p.second - 1 ;
            }

        }

        for (auto& p : processes ){

            if ( p -> inReadyQueue ){

                p -> waitingTime = ( p -> waitingTime ) + 1 ;

            }

        }

        for (auto& p : timeInReadyQueueNonPreemptive){

            if ( p.second != -1 ){
                p.second = p.second + 1 ;
            }

        }

    }

    if ( processExecuted.size() > TimeChunks.size() ){

        TimeChunks.push_back({Chunk_startTime , time - 1}) ;

        if (currentProcessNum == -1 ){
            processExecuted.push_back(-1) ;
        }
        else {
            id_process[currentProcessNum] -> lastExitTime = 200 ;
        }

    }

    printGantChart( TimeChunks , processExecuted) ;
    cout << endl ;
    printAverage_waiting_turnaround_Time(processes) ;


}


int main() {

    cout << "First come First served :" << endl ;
    Fcfs_simulation();
    cout << endl ;

    cout << "Shortest job first:" << endl ;
    Sjf();
    cout << endl ;

    cout << "Shortest remaining time :"<<endl ;
    ShortestRemainingTimeFirst() ;
    cout << endl ;

    cout << "Round robin :" << endl ;
    RR_simulation() ;
    cout << endl ;

    cout << "Priority scheduling preemptive:" << endl ;
    prioritySchdulingPreemptive() ;
    cout << endl ;

    cout << "Priority Scheduling Non Preemptive:" << endl ;
    prioritySchdulingNonPreemptive() ;
    cout << endl ;

    return 0 ;

}
