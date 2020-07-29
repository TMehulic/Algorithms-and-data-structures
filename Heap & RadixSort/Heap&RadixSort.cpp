#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void radixSort(vector<int> &a) {
    int max=*max_element(a.begin(),a.end());    
    int cifra=1;
    while(max/cifra>0){                         //Dok najveci broj ima cifara
    
        vector<int>pomocni(a.size());           //Privremeni rezultati
        
        int brojac[10]={0};
        for(int i=0;i<a.size();i++){
            int index=(a[i]/cifra)%10;
            brojac[index]++;
        }
        
        for(int i=1;i<10;i++){
            brojac[i]+=brojac[i-1];
        }
        
        for(int i=a.size()-1;i>=0;i--){
            brojac[(a[i]/cifra)%10]--;
            pomocni[brojac[(a[i]/cifra)%10]]=a[i];
        }
        
        for(int i=0;i<a.size();i++){
            a[i]=pomocni[i];
        }
        cifra*=10;
    }
}

bool jeLiLIst(vector<int> a, int i,int velicina){
    return (i>=velicina/2) && (i<velicina);
}

int roditelj(int i){
    return (i-1)/2;
}

int lijevoDijete(int i){
    return 2*i+1;
}

int desnoDijete(int i){
    return 2*i+2;
}

void popraviDolje(vector<int>&a,int i,int velicina){
    while (!jeLiLIst(a,i,velicina)) {
        int veci=lijevoDijete(i);
        int dd=desnoDijete(i);
        if(dd<velicina && a[dd]>a[veci]){
            veci=dd;
        }
        if(a[i]>a[veci]) return;
        int temp=a[i];
        a[i]=a[veci];
        a[veci]=temp;
        i=veci;
    }
}

void popraviGore(vector<int> &a,int i){
    while(i!=0 && a[i]>a[roditelj(i)]){
        int temp=a[i];
        a[i]=a[roditelj(i)];
        a[roditelj(i)]=temp;
        i=roditelj(i);
    }
}

void stvoriGomilu(vector<int> &a){
    int velicina=a.size();
    for(int i=velicina/2;i>=0;i--){
        popraviDolje(a,i,velicina);
    }
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina){
    a.push_back(umetnuti);
    velicina++;
    popraviGore(a,velicina-1);
}

int izbaciPrvi(vector<int> &a, int &velicina){
    if(velicina==0) throw "Gomila je prazna!";
    velicina--;
    int temp=a[0];
    a[0]=a[velicina];
    a[velicina]=temp;
    if(velicina!=0){
        popraviDolje(a,0,velicina);
    }
    return a[velicina];
}

void gomilaSort(vector<int> &a){
    stvoriGomilu(a);
    int velicina=a.size();
    for(int i=0;i<a.size()-1;i++){
        izbaciPrvi(a,velicina);
    }
}

int main() {
    return 0;
}
