#include <iostream>
#include <regex>
#include <thread>
#include <mutex> 

using namespace std;

mutex mtx;

void handle(string s, int self){
    regex dot("\\.");
    sregex_token_iterator it(s.begin(), s.end(), dot, -1);
    sregex_token_iterator reg_end;
    if(it!=reg_end){
        string name = *it;
        it++;
        if(it!=reg_end){
            string type = *it;
            if(type=="cpp"){
                string cmd = "g++ " + name + "." + type + " -o " + name;
                system(cmd.c_str());
                mtx.lock();
                cout << "Running " << name << "." << type << endl;
                system(name.c_str());
                cout << endl;
                mtx.unlock();
            }
        }
    }
}

void run(void){
    while(true){
        cout << "$";
        cout << flush;
        string s;
        getline(cin, s);
        if(s.length()>0){
            if(s=="exit"){
                cout.clear();
                break;
            }
            // break into words based on space
            regex ws_re("\\s+");
            sregex_token_iterator it(s.begin(), s.end(), ws_re, -1);
            sregex_token_iterator reg_end;
            thread t[10];
            int i = 0;
            while(it!=reg_end){
                t[i] = thread(handle, *it, i);
                it++;
                i++;
                if(i==10){
                    break;
                }
            }
            for(int i=0; i<10; i++){
                if(t[i].joinable()){
                    t[i].join();
                }
            }
        }
    }
}

int main(){
    run();
}