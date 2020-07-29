#include <iostream>

template <typename Tip>

struct Cvor{
    Tip element;
    Cvor<Tip>* before;
    Cvor<Tip>* next;
};

template <typename Tip>
class DvostraniRed{
    
    Cvor<Tip> *pocetak;
    Cvor<Tip> *kraj;
    int broj_el;
    
public:
    
    DvostraniRed(){
        broj_el=0;
        pocetak=nullptr;
        kraj=nullptr;
    }
    
    ~DvostraniRed(){
        Cvor<Tip> *pomocni=pocetak;
        while(pocetak!=nullptr){
            pomocni=pocetak->next;
            delete pocetak;
            pocetak=pomocni;
        }
        broj_el=0;
        pocetak=nullptr;
        kraj=nullptr;
    }
    
    DvostraniRed(const DvostraniRed &r){
        broj_el=r.broj_el;
        Cvor<Tip> *pomocni1, *pomocni(r.pocetak);
        pocetak=nullptr;
        kraj=nullptr;
        while(pomocni!=nullptr){
            if(pomocni==r.pocetak){
                pocetak=new Cvor<Tip> {pomocni->element, nullptr, nullptr};
                pomocni1=pocetak;
            }else{
                pomocni1->next=new Cvor<Tip> {pomocni->element, pomocni1, nullptr};
                pomocni1=pomocni1->next;
            }
            if(pomocni->next==nullptr){
                    kraj=pomocni1;
                }
            pomocni=pomocni->next;
        }
    }
    
    DvostraniRed &operator =(const DvostraniRed &r){
        if(this==&r){
            return *this;
        }
        this->brisi();
        broj_el=r.broj_el;
        Cvor<Tip> *pomocni1, *pomocni(r.pocetak);
        pocetak=nullptr;
        kraj=nullptr;
        while(pomocni!=nullptr){
            if(pomocni==r.pocetak){
                pocetak=new Cvor<Tip> {pomocni->element, nullptr, nullptr};
                pomocni1=pocetak;
            }else{
                pomocni1->next=new Cvor<Tip> {pomocni->element, pomocni1, nullptr};
                pomocni1=pomocni1->next;
            }
            if(pomocni->next==nullptr){
                    kraj=pomocni1;
                }
            pomocni=pomocni->next;
        }
        return *this;
    }
    
    
    void brisi(){
        Cvor<Tip> *pomocni=pocetak;
        while(pocetak!=nullptr){
            pomocni=pocetak->next;
            delete pocetak;
            pocetak=pomocni;
        }
        broj_el=0;
        pocetak=nullptr;
        kraj=nullptr;
    }
    
    void staviNaVrh(const Tip& el){
        if(broj_el==0){
            pocetak=new Cvor<Tip> {el, nullptr, nullptr};
            kraj=pocetak;
        }else{
            Cvor<Tip>* novi=new Cvor<Tip> {el,kraj,nullptr};
            kraj->next=novi;
            kraj=novi;
        }
        broj_el++;
    }
    
    Tip skiniSaVrha(){
        if(broj_el==0) throw "Red je prazan!";
        Tip element=kraj->element;
        Cvor<Tip>*pomocni=kraj->before;
        delete kraj;
        kraj=pomocni;
        if(kraj!=nullptr){
            kraj->next=nullptr;
        }
        broj_el--;
        if(broj_el==0){
            pocetak=nullptr;
        }
        return element;
    }
    
    void staviNaCelo(const Tip& el){
        if(broj_el==0){
            pocetak=new Cvor<Tip> {el, nullptr, nullptr};
            kraj=pocetak;
        }else{
            Cvor<Tip>* novi=new Cvor<Tip> {el,nullptr,pocetak};
            pocetak->before=novi;
            pocetak=novi;
        }
        broj_el++;
    }
    
    Tip skiniSaCela(){
        if(broj_el==0) throw "Red je prazan!";
        Tip element=pocetak->element;
        Cvor<Tip>*pomocni=pocetak->next;
        delete pocetak;
        pocetak=pomocni;
        if(pocetak!=nullptr){
            pocetak->before=nullptr;
        }
        broj_el--;
        if(broj_el==0) kraj=nullptr;
        return element;
    }
    
    Tip& vrh(){
        if(broj_el==0) throw "Red je prazan!";
        return kraj->element;
    }
    
    Tip& celo(){
        if(broj_el==0) throw "Red je prazan!";
        return pocetak->element;
    }
    
    int brojElemenata(){
        return broj_el;
    }
};

void testBrojElemenata(){
    DvostraniRed<int> r;
    for(int i=0;i<1000;i++){
        r.staviNaCelo(i);
    }
    if(r.brojElemenata()==1000){
        std::cout<<"Test OK."<<std::endl;
    }else{
        std::cout<<"Pogresan rezultat."<<std::endl;
    }
}


void testBrisi(){
    DvostraniRed<int> r;
    for(int i=0;i<15;i++){
        r.staviNaVrh(i);
    }
    r.brisi();
    if(r.brojElemenata()==0){
        std::cout<<"Test OK."<<std::endl;
    }else{
        std::cout<<"Pogresan rezultat."<<std::endl;
    }
}
void testStaviNaVrh(){
    DvostraniRed<int> r;
    for(int i=0;i<100000;i++){
        r.staviNaVrh(i);
    }
    if(r.brojElemenata()==100000){
        std::cout<<"Test OK."<<std::endl;
    }else{
        std::cout<<"Pogresan rezultat."<<std::endl;
    }
}
void testStaviNaCelo(){
    DvostraniRed<int> r;
    for(int i=0;i<100000;i++){
        r.staviNaCelo(i);
    }
    if(r.brojElemenata()==100000){
        std::cout<<"Test OK."<<std::endl;
    }else{
        std::cout<<"Pogresan rezultat."<<std::endl;
    }
}
void testSkiniSaVrha(){
    DvostraniRed<int> r;
    for(int i=1;i<=10;i++){
        r.staviNaVrh(i);
    }
    if(r.skiniSaVrha()==10 && r.brojElemenata()==9){
        std::cout<<"Test OK."<<std::endl;
    }else{
        std::cout<<"Pogresan rezultat."<<std::endl;
    }
}
void testSkiniSaCela(){
    DvostraniRed<int> r;
    for(int i=1;i<=10;i++){
        r.staviNaVrh(i);
    }
    if(r.skiniSaCela()==1 && r.brojElemenata()==9){
        std::cout<<"Test OK."<<std::endl;
    }else{
        std::cout<<"Pogresan rezultat."<<std::endl;
    }
}
void testVrh(){
    DvostraniRed<int> r;
    for(int i=0;i<=10;i++){
        r.staviNaVrh(i);
    }
    if(r.vrh()==10 && r.brojElemenata()==11){
        std::cout<<"Test OK."<<std::endl;
    }else{
        std::cout<<"Pogresan rezultat."<<std::endl;
    }
}
void testCelo(){
    DvostraniRed<int> r;
    for(int i=0;i<=10;i++){
        r.staviNaCelo(i);
    }
    if(r.celo()==10 && r.brojElemenata()==11){
        std::cout<<"Test OK."<<std::endl;
    }else{
        std::cout<<"Pogresan rezultat."<<std::endl;
    }
}



int main() {
    testBrojElemenata();
    testBrisi();
    testStaviNaVrh();
    testStaviNaCelo();
    testSkiniSaVrha();
    testSkiniSaCela();
    testVrh();
    testCelo();
    return 0;
}
