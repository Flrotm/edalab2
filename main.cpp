#include <iostream>
#include <pthread.h>
#include <vector>
#include <algorithm>
using namespace std;

int n=0;
int nthreads=0;

int find_max(vector<int> vec){
    return *max_element(vec.begin(), vec.end());
}

void print(vector<int> v){
    for (auto &i: v) {
        cout << i << ' ';
    }
    cout << std::endl;
}


struct thread_data {
    int t_id;
    vector<int> particion;
};
int main() {


    pthread_t threads[nthreads];
    vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        int a=rand()%100+1;
        v[i]=a;
    }
    for (int j = 0; j < v.size()-1; ++j) {
        cout<<v[j]<<" ";
    }
    int splitsize=n/nthreads;
    vector<int> vec[splitsize];
    for (int k = 0; k < nthreads; ++k){
        auto start_itr = next(v.cbegin(), k*splitsize);
        auto end_itr =next(v.cbegin(), k*splitsize + splitsize);

        vec[k].resize(splitsize);
        if (k*splitsize + splitsize > v.size()){
            end_itr = v.cend();
            vec[k].resize(v.size() - k*splitsize);
        }
        copy(start_itr, end_itr, vec[k].begin());
    }

    for (int i = 0; i < nthreads; i++) {
        print(vec[i]);
        find_max(vec[i]);
    }

    int rc;
    struct thread_data td[nthreads];
    for (int l = 0; l < nthreads; ++l) {
        td[l].particion=vec[l];
        td[l].t_id=l;
        rc = pthread_create(&threads[l], NULL,find_max(), (void *)l);

    }
}