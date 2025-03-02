//
//  shuruiz
//

#include<iostream>
#include<float.h>
#include<cmath>
#include<stdio.h>
#include<stack>
#include<utility>
#include<omp.h>
using namespace std;

stack < pair<double, double> > _stack;
double epsilon=pow(10, -6);
int s=12;
double fx(double x);
double M= -DBL_MAX; // initialize M to the -max_double, infinite small
int _current=0; // threads

omp_lock_t masterlock;
omp_lock_t workerlock;

int main(int argc, char** argv)
{
    
    _stack.push(make_pair(1, 100));
    double starttime=omp_get_wtime();
    omp_init_lock(&masterlock);
    omp_init_lock(&workerlock);
    
#pragma omp parallel
    {
        while(true)
        {
            omp_set_lock(&workerlock);
            // elimiate cout command to shorten the running time.
            // cout<<"on thread: "<<omp_get_thread_num()<<endl;
            if(_stack.empty()){ // assign work
                if(_current==0){
                    omp_unset_lock(&workerlock);
                    break;}
                else
                    omp_unset_lock(&workerlock);
            }
            else{
                _current++;
                double c=_stack.top().first; //get left of the interval
                double d=_stack.top().second; //get right of the interval
      
                _stack.pop();
                omp_unset_lock(&workerlock);
                
                double f_c=fx(c);
                double f_d=fx(d);
                
                omp_set_lock(&masterlock);
                
                if(f_c>M || f_d>M)
                {
                    //set M to max{M, fc, fd}
                    M=max(f_c, f_d);
                }
                
                if(((f_c+f_d+s*(d-c))/2)>=M+epsilon) // computing the max of interval [c,d]
                {
                    omp_set_lock(&workerlock);
                    // breadth-first generating intervals
                    _stack.push(make_pair(c, (c+d)/2));
                    _stack.push(make_pair((c+d)/2, d));
                    _current--;
                    omp_unset_lock(&workerlock);
                }
                
                else
                //get rid of this interval, no need to compute because max interval [a,b] < M+epsilon
                {
                    omp_set_lock(&workerlock);
                    _current--;
                    omp_unset_lock(&workerlock);
                }
                omp_unset_lock(&masterlock);
            }
        }
    }
    double endtime=omp_get_wtime();
    double total_time=endtime-starttime;
    printf("total runtime is %f", total_time );
    printf("final result is %f", M);
    // cout<<"The runtime is: "<<total_time<<endl;
    // cout<<"The final result is: "<<M<<endl;
    return 0;
}

//f(x) sum below, double precision
double fx(double x){
    double result=0;
    for(int i=100; i>=1; i--)
    {
        double tmp=0;
        for(int j=i; j>=1; j--)
        {
            tmp+=pow(x+0.5*j, -3.3);
        }
        result+=(sin(x+tmp)/pow(1.3, i));
    }
    return result;
}

