#include <iostream>
#include <fstream>
#include <string>

template<typename Tip>
void bubble_sort(Tip* niz, int vel){
    for(int i=vel-1;i>0;i--){
        for(int j=1;j<=i;j++){
            if(niz[j-1]>niz[j]){
                Tip element=niz[j];
                niz[j]=niz[j-1];
                niz[j-1]=element;
            }
        }
    }
}

template<typename Tip>
void selection_sort(Tip* niz, int vel){
    for(int i=0;i<vel-1;i++){
        Tip min=niz[i];
        int index_min=i;
        for(int j=i;j<vel;j++){
            if(niz[j]<min){
                min=niz[j];
                index_min=j;
            }
        }
        niz[index_min]=niz[i];
        niz[i]=min;
    }
}

template<typename Tip>
int particija(Tip*niz, int prvi, int zadnji){
    Tip pivot=niz[prvi];
    int p=prvi+1;
    while(p<=zadnji && niz[p]<pivot){
        p++;
    }
    for(int i=p+1;i<=zadnji;i++){                                    
        if(niz[i]<pivot){
            Tip temp=niz[i];
            niz[i]=niz[p];
            niz[p]=temp;
            p++;
        }
    }
    Tip temp2=niz[prvi];
    niz[prvi]=niz[p-1];
    niz[p-1]=temp2;
    return p-1;
}

template<typename Tip>
void quick_sort2(Tip*niz, int prvi, int zadnji){
    if(prvi<zadnji){
        int j=particija(niz,prvi,zadnji);
        quick_sort2(niz,prvi,j-1);
        quick_sort2(niz,j+1,zadnji);
    }
}

template<typename Tip>
void quick_sort(Tip* niz, int vel){
    quick_sort2(niz,0,vel-1);
}


template<typename Tip>
void merge(Tip* niz, int l, int p, int q, int u){
    int i=0;
    int j=q-l;
    int k=l;
    Tip* niz2=new Tip[u+1-l];
    for(int m=0; m<=u-l;m++){
        niz2[m]=niz[l+m];
    }
    while(i<=p-l && j<=u-l){
        if(niz2[i]<niz2[j]){
            niz[k]=niz2[i];
            i++;
        }else{
            niz[k]=niz2[j];
            j++;
        }
        k++;
    }
    while(i<=p-l){
        niz[k]=niz2[i];
        k++;
        i++;
    }
    while(j<=u-l){
        niz[k]=niz2[j];
        k++;
        j++;
    }
    delete [] niz2;
}

template<typename Tip>
void merge_sort2(Tip* niz, int l, int u){
    if(u>l){
        int p=(l+u-1)/2;
        int q=p+1;
        merge_sort2(niz,l,p);
        merge_sort2(niz,q,u);
        merge(niz,l,p,q,u);
    }
}


template<typename Tip>
void merge_sort(Tip* niz, int vel){
    merge_sort2(niz,0, vel-1);
}


                                                                        /* F U N K C I J E */
                                                                        
void generisi(std::string filename, int vel){
    std::ofstream izlazni_tok(filename);
    for(int i=0;i<vel;i++){
        izlazni_tok<<std::rand()<<" ";
    }
    izlazni_tok.close();
}

void ucitaj(std::string filename, int*& niz, int &vel){
    std::ifstream ulazni_tok(filename);
    if(!ulazni_tok) std::cout<<"Greska prilikom otvaranja datoteke!"<<std::endl;
    else{
        int broj;
        while(ulazni_tok>>broj){
            vel++;
        }
        niz=new int[vel];
        ulazni_tok.close();
        ulazni_tok.open(filename);
        int i=0;
        while(ulazni_tok>>broj){
            niz[i]=broj;
            i++;
        }
    }
    if(!ulazni_tok.eof()) std::cout << "Greska!"<<std::endl;
    ulazni_tok.close();
}

void meni(int*& niz, int vel){
    std::cout<<"U datoteku pod nazivom \"datoteka.txt\" je generisano 100 nasumicnih brojeva!"<<std::endl;
   
    //Ucitavanje niza
    ucitaj("datoteka.txt",niz,vel);
    if(vel==100){
        std::cout<<"Uspjesno su ucitani brojevi iz navedene datoteke!"<<std::endl;
    }else{
        std::cout<<"Doslo je do greske prilikom ucitavanja iz datoteke!"<<std::endl;
        return ;
    }
  
    //Opcije
    std::cout<<"Izaberite algoritam za sortiranje: "<<std::endl;
    std::cout<<"1 - Bubble Sort"<<std::endl;
    std::cout<<"2 - Selection Sort"<<std::endl;
    std::cout<<"3 - Quick Sort"<<std::endl;
    std::cout<<"4 - Merge Sort"<<std::endl;
    int n;
    do{
        std::cin>>n;
        if(n>=1 && n<=4) break;
        std::cout<<"Pogresan izbor, unesite ponovo!"<<std::endl;
        std::cin.ignore(1000,'\n');
        std::cin.clear();
    }while(n<1 || n>4);
   
    //Vrijeme
    int ukvrijeme;
    clock_t vrijeme1;
    clock_t vrijeme2;
    switch(n){
        case 1:
            std::cout<<"Izabrali ste Bubble Sort!"<<std::endl;
            vrijeme1 = clock();
            bubble_sort(niz,vel);
            vrijeme2 = clock();
            ukvrijeme = (vrijeme2-vrijeme1) / (CLOCKS_PER_SEC /1000);
            std::cout<<"Vrijeme izvrsavanja algoritma: "<<ukvrijeme<<"ms."<<std::endl;
            break;
        case 2:
            std::cout<<"Izabrali ste Selection Sort!"<<std::endl;
            vrijeme1 = clock();
            selection_sort(niz,vel);
            vrijeme2 = clock();
            ukvrijeme = (vrijeme2-vrijeme1) / (CLOCKS_PER_SEC /1000);
            std::cout<<"Vrijeme izvrsavanja algoritma: "<<ukvrijeme<<"ms."<<std::endl;
            break;
        case 3:
            std::cout<<"Izabrali ste Quick Sort!"<<std::endl;
            vrijeme1 = clock();
            quick_sort(niz,vel);
            vrijeme2 = clock();
            ukvrijeme = (vrijeme2-vrijeme1) / (CLOCKS_PER_SEC /1000);
            std::cout<<"Vrijeme izvrsavanja algoritma: "<<ukvrijeme<<"ms."<<std::endl;
            break;
        case 4:
            std::cout<<"Izabrali ste Merge Sort!"<<std::endl;
            vrijeme1 = clock();
            merge_sort(niz,vel);
            vrijeme2 = clock();
            ukvrijeme = (vrijeme2-vrijeme1) / (CLOCKS_PER_SEC /1000);
            std::cout<<"Vrijeme izvrsavanja algoritma: "<<ukvrijeme<<"ms."<<std::endl;
    }
    
    //Test uspjesnosti
    bool uspjesnoSortiran=true;
    for(int i=0;i<vel-1;i++){
        if(niz[i+1]<niz[i]){
            uspjesnoSortiran=false;
            break;
        }
    }
    if(uspjesnoSortiran){
        std::cout<<"Vas niz je uspjesno sortiran!"<<std::endl;
    }else{
        std::cout<<"Doslo je do greske prilikom sortiranja vaseg niza!"<<std::endl;
        return;
    }
    
    //Upis u novu datoteku
    std::cout<<"Unesite naziv datoteke u koju zelite da upisete sortirani niz."<<std::endl;
    std::string izlaz;
    std::cin.ignore(1000,'\n');
    std::cin.clear();
    std::getline(std::cin,izlaz);
    
    std::ofstream izlazni_tok(izlaz);
    for(int i=0;i<vel;i++){
        izlazni_tok<<niz[i]<<" ";
    }
    izlazni_tok.flush();
    izlazni_tok.close();
}



int main() {
    int* niz;
    int vel=0;
    generisi("datoteka.txt",100);
    meni(niz,vel);
    delete [] niz; 
    return 0;
}
