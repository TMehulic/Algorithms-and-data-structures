#include <iostream>
#include <vector>

template <typename Tip>

struct Cvor{
    Tip element;
    Cvor<Tip>*veza;
};

template <typename Tip>

class Stek{
    int broj_el;
    Cvor<Tip> *top;

public:

    Stek(){
        broj_el=0;
        top=nullptr;
    }
    
    ~Stek(){
        Cvor<Tip>*pomocni=top;
        while(pomocni!=nullptr){
            pomocni=top->veza;
            delete top;
            top=pomocni;
        }
        broj_el=0;
    }
    
    Stek(const Stek<Tip> &s){
        broj_el=s.broj_el;
        Cvor<Tip>*pomocni1,*pomocni(s.top);
        pomocni1=nullptr;
        top=nullptr;
        while(pomocni!=nullptr){
            if(pomocni==s.top){
                top=new Cvor<Tip> {pomocni->element, nullptr};
                pomocni1=top;
            }else{
                pomocni1->veza=new Cvor<Tip> {pomocni->element, nullptr};
                pomocni1=pomocni1->veza;
            }
            pomocni=pomocni->veza;
        }
        
    }
    
    Stek &operator=(const Stek &s){
        if(this==&s) return *this;   //Samododjela
        this->brisi();
        broj_el=s.broj_el;
        Cvor<Tip>*pomocni1,*pomocni(s.top);
        pomocni1=nullptr;
        top=nullptr;
        while(pomocni!=nullptr){
            if(pomocni==s.top){
                top=new Cvor<Tip> {pomocni->element, nullptr};
                pomocni1=top;
            }else{
                pomocni1->veza=new Cvor<Tip> {pomocni->element, nullptr};
                pomocni1=pomocni1->veza;
            }
            pomocni=pomocni->veza;
        }
        return *this;
        
    }
    
    void brisi(){
        Cvor<Tip>*pomocni=top;
        while(pomocni!=nullptr){
            pomocni=top->veza;
            delete top;
            top=pomocni;
        }
        broj_el=0;
    }
    
    void stavi(const Tip& el){
        if(broj_el==0){
            top=new Cvor<Tip> {el, nullptr};
        }else{
            Cvor<Tip>*novi=new Cvor<Tip> {el,top};
            top=novi;
            novi=nullptr;
        }
        broj_el++;
    }
    
    Tip skini(){
        if(broj_el==0) throw "Stek je prazan!";
        Cvor<Tip>*pomocni=top;
        top=top->veza;
        Tip vrijednost=pomocni->element;
        delete pomocni;
        broj_el--;
        return vrijednost;
    }
    
    Tip& vrh(){
        if(broj_el==0) throw "Stek je prazan!";
        return top->element;
    }
    
    int brojElemenata() const{
        return broj_el;
    }
};


                                                                    /* P R E T R A G A */


template <typename Tip>
void pretraga(Stek<std::vector<Tip>>&s, Tip trazeni){
    static int broj_prije=s.brojElemenata()-1;
    bool ima=false;
    if(s.brojElemenata()==0){
        std::cout<<"Nema elementa"<<std::endl;
    }else{
        if(s.vrh().size()!=0){                  //Provjera zbog indeksiranja
            if(s.vrh()[0]<=trazeni && trazeni<=s.vrh()[s.vrh().size()-1]){
                int l=0;
                int r=s.vrh().size()-1;
                while(l<=r){
                    int m=(l+r)/2;
                    if(s.vrh()[m]==trazeni){
                        std::cout<<m<<" "<<broj_prije;
                        ima=true;
                        break;
                    }
                    if(s.vrh()[m]<trazeni){
                        l=m+1;
                    }else{
                        r=m-1;
                    }
                }
                if(!ima){
                    std::cout<<"Nema elementa"<<std::endl;
                    return;
                }
            }
        }
        if(ima) return;
        broj_prije--;
        std::vector<Tip> pomocni=s.skini();
        pretraga(s,trazeni);
        s.stavi(pomocni);
    }
    broj_prije=0;           //Ako se opet funckija pozove u istom programu
}

void test1(){
    std::vector<int> a;
    std::vector<int> b;
    std::vector<int> c;
    std::vector<int> d;
    std::vector<int> e;
    for(int i=0;i<10;i++){
        a.push_back(i);
    }
    b.push_back(100); b.push_back(200); b.push_back(300);
    c.push_back(311);c.push_back(333); c.push_back(356);
    for(int i=400;i<500;i+=100){
        d.push_back(i);
    }
    Stek<std::vector<int>>s;
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);
    s.stavi(e);
    std::cout<<"Ocekivan rezultat: 0 2"<<std::endl;
    std::cout<<"Rezultat pretrage: ";
    pretraga(s,311);
    std::cout<<std::endl<<std::endl;
}
void test2(){               //Testira efikasnost
    std::vector<int> a;
    for(int i=0;i<1000000;i++){
        a.push_back(i);
    }
    Stek<std::vector<int>> s;
    s.stavi(a);
    std::cout<<"Ocekivan rezultat: 13425 0"<<std::endl;
    std::cout<<"Rezultat pretrage: ";
    pretraga(s,13425);
    std::cout<<std::endl<<std::endl;
}
void test3(){
    //Da li ispravno radi ako nema elementa
    Stek<std::vector<int>> s;
    std::vector<int> a, b,c;
    for(int i=1;i<100;i*=2){
        a.push_back(i);
    }
    for(int i=101;i<1000;i++){
        b.push_back(i);
    }
    for(int i=1000;i<10000;i++){
        c.push_back(i);
    }
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    std::cout<<"Ocekivan rezultat: Nema elementa"<<std::endl;
    std::cout<<"Rezultat pretrage: ";
    pretraga(s,3);
}

int main() {
    test1();
    test2();
    test3();
    return 0;
}
