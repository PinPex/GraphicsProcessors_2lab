#include <iostream>
#include <vector>
#include <pthread.h>
#include <malloc.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <chrono>

using namespace std::chrono;
using namespace std;

vector<int> makeVector(int size, int a, int b){
    vector<int> v(size);
    for(auto& i: v){
        i = rand() % b + a; 
    }
    return v;
}

void printVector(vector<int> v, char* str){
    cout << str << endl;
    for(auto i: v){
        cout << i << " ";
    }
    cout << endl;
}

struct vectors{
    vector<int> a;
    vector<int> b;
    vector<int> c;
    int start = 0;
    int stop = 0;
    vectors(vector<int> first, vector<int> second) 
                    : a(first), b(second){

        c = vector<int>(a.size(), 0);
    };
    vectors(){
    };

    void setLimits(int first, int second){
        start = first;
        stop = second;
    }

    void setVectors(vector<int> first, vector<int> second){
        a = first;
        b = second;
        c = vector<int>(a.size(), 0);
    }

    void printLimits(){
        cout << start << " " << stop << endl;
    }
};

struct package{
    vector<int> a;
    vector<int> b;
    int start;
    int stop;
    package(vector<int> a, vector<int> b, int start, int stop): 
            a(a), b(b), start(start), stop(stop){

    }
};
vector<int> result;

void* add_vec_thr(void* arg){
    package* v = static_cast<package*>(arg);
    for(int i = v->start; i < v->stop; ++i){
        result[i] = v->a[i] + v->b[i];
    }
    return NULL;
}



vector<int> makeThreads(vector<int> a, vector<int> b, int threads_count){
    pthread_t *threads = (pthread_t*)malloc(threads_count*sizeof(pthread_t));
    result.resize(a.size(), 0);
    for(int i = 0; i < threads_count; ++i){
        package* pack = new package(a, b, i * a.size() / threads_count, 
                        (i + 1) * a.size() / threads_count);
        pthread_create(&threads[i], NULL, add_vec_thr, pack);
    }
    for(int i = 0; i < threads_count; ++i){
        pthread_join(threads[i],NULL);
    }
    free(threads);
    return result;
}


int main(){
    srand(time(NULL));
    vector<int> first, second;
    vector<int> c;


    int size = 10;
    first = makeVector(size, 0, 9);
    second = makeVector(size, 0, 9);

    printVector(first, "first massive:");
    
    printVector(second, "second massive:");
    steady_clock::time_point start, end;
    nanoseconds elapsed;
    for(int i = 2; i < 16; ++i){
        
        start = steady_clock::now();
        vector<int> result = makeThreads(first, second, i);
        end = steady_clock::now();
        elapsed = duration_cast<nanoseconds>(end - start);
        cout << "Count threads " << i << " with countdown " << elapsed.count() << " ns" << endl;
    }
    
    return 0;
}