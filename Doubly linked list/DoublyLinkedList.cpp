#include <iostream>
template <typename Tip>

class Lista {
public:
    virtual ~Lista() {}
    Lista()=default;                                            
    virtual int brojElemenata() const=0;
    virtual Tip trenutni() const=0;
    virtual Tip& trenutni()=0;
    virtual bool prethodni()=0;
    virtual bool sljedeci()=0;
    virtual void pocetak()=0;
    virtual void kraj()=0;
    virtual void obrisi()=0;
    virtual void dodajIspred(const Tip& el)=0;
    virtual void dodajIza(const Tip& el)=0; 
    virtual Tip operator [](int index) const=0;
    virtual Tip &operator [] (int index)=0;
    
};

template <typename Tip>

struct Cvor{
    Tip element;
    Cvor<Tip> *before;
    Cvor<Tip> *next;
};

template <typename Tip>

class DvostrukaLista:public Lista<Tip>{
    
    int broj_el;
    Cvor<Tip> *prvi;
    Cvor<Tip> *trenutni_pok;
    Cvor<Tip> *zadnji;
    mutable int index_posljednji_trazeni;
    mutable Cvor<Tip> *posljednji_trazeni;
    
public:

    DvostrukaLista(){
        broj_el=0;
        prvi=nullptr;
        trenutni_pok=nullptr;
        zadnji=nullptr;
        index_posljednji_trazeni=0;
        posljednji_trazeni=nullptr;
    }
    
    ~DvostrukaLista(){
        while(prvi!=nullptr){
            trenutni_pok=prvi;
            prvi=prvi->next;
            delete trenutni_pok;
        }
        broj_el=0;
        zadnji=nullptr;
        index_posljednji_trazeni=0;
        posljednji_trazeni=nullptr;
    }
    
    DvostrukaLista(const DvostrukaLista &l){
        broj_el=l.broj_el;
        if(l.prvi==nullptr){
            prvi=nullptr;
            trenutni_pok=nullptr;
            zadnji=nullptr;
        }else{
            Cvor<Tip>*pomocni=l.prvi;
            while(pomocni!=nullptr){
                if(pomocni==l.prvi){
                    prvi=new Cvor<Tip> {pomocni->element, nullptr,nullptr};
                    trenutni_pok=prvi;
                }else{
                    trenutni_pok->next=new Cvor<Tip> {pomocni->element, trenutni_pok, nullptr};
                    trenutni_pok=trenutni_pok->next;
                }
                if(pomocni->next==nullptr){
                    zadnji=trenutni_pok;
                }
                pomocni=pomocni->next;
            }
        }
        index_posljednji_trazeni=0;
        posljednji_trazeni=nullptr;
    }
    
    DvostrukaLista &operator=(const DvostrukaLista &l){
        if(this==&l) return *this;
        
        //Dealocirati sve zauzeto
        
        while(prvi!=nullptr){
            trenutni_pok=prvi;
            prvi=prvi->next;
            delete trenutni_pok;
        }
        broj_el=0;
        zadnji=nullptr;
        
        //Onda kao kod konstruktora
        
        broj_el=l.broj_el;
        if(l.prvi==nullptr){
            prvi=nullptr;
            trenutni_pok=nullptr;
            zadnji=nullptr;
        }else{
            Cvor<Tip>*pomocni=l.prvi;
            while(pomocni!=nullptr){
                if(pomocni==l.prvi){
                    prvi=new Cvor<Tip> {pomocni->element, nullptr,nullptr};
                    trenutni_pok=prvi;
                }else{
                    trenutni_pok->next=new Cvor<Tip> {pomocni->element, trenutni_pok, nullptr};
                    trenutni_pok=trenutni_pok->next;
                }
                if(pomocni->next==nullptr){
                    zadnji=trenutni_pok;
                }
                pomocni=pomocni->next;
            }
        }
        index_posljednji_trazeni=0;
        posljednji_trazeni=nullptr;
        return *this;
    }
    
    int brojElemenata() const {
        return broj_el;
    }
    
    Tip trenutni() const {
        if(broj_el==0) throw "Lista je prazna!";
        return trenutni_pok->element;
    }
    
    Tip &trenutni() {
        if(broj_el==0) throw "Lista je prazna!";
        return trenutni_pok->element;
    }
    
    bool prethodni(){
        if(broj_el==0) throw "Lista je prazna!";
        if(trenutni_pok->before==nullptr) return false;
        trenutni_pok=trenutni_pok->before;
        return true;
    }
    
    bool sljedeci(){
        if(broj_el==0) throw "Lista je prazna!";
        if(trenutni_pok->next==nullptr) return false;
        trenutni_pok=trenutni_pok->next;
        return true;
    }
    
    void pocetak(){
        if(broj_el==0) throw "Lista je prazna!";
        trenutni_pok=prvi;
    }
    
    void kraj(){
        if(broj_el==0) throw "Lista je prazna!";
        trenutni_pok=zadnji;
    }
    
    void obrisi(){
        if(broj_el==0) throw "Lista je prazna!";
        
        if(trenutni_pok->before==nullptr){                     //Trenutni na pocetku
            prvi=trenutni_pok->next;
            delete trenutni_pok;
            trenutni_pok=prvi;
            if(trenutni_pok!=nullptr){
                trenutni_pok->before=nullptr;
                prvi->before=nullptr;
            }
        }else if(trenutni_pok->next==nullptr){             //Trenutni na kraju
            trenutni_pok=trenutni_pok->before;
            delete zadnji;
            trenutni_pok->next=nullptr;
            zadnji=trenutni_pok;
        }else{
            Cvor<Tip>*pomocni=trenutni_pok->before;
            pomocni->next=trenutni_pok->next;
            pomocni=trenutni_pok->next;
            pomocni->before=trenutni_pok->before;
            delete trenutni_pok;
            trenutni_pok=pomocni;
        }
        broj_el--;
    }
    
    void dodajIspred(const Tip& el){
        if(broj_el==0){
            prvi=new Cvor<Tip> {el, nullptr, nullptr};
            trenutni_pok=prvi;
            zadnji=prvi;
        }else if(trenutni_pok->before==nullptr){
            Cvor<Tip>*novi=new Cvor<Tip> {el,nullptr,nullptr};
            prvi=novi;
            prvi->next=trenutni_pok;
            trenutni_pok->before=prvi;
            zadnji->before=prvi;
        }else{
            Cvor<Tip>* pomocni=trenutni_pok->before;
            Cvor<Tip>*novi=new Cvor<Tip> {el,trenutni_pok->before,trenutni_pok};
            trenutni_pok->before=novi;
            pomocni->next=novi;
        }
        broj_el++;
    }
    
    void dodajIza(const Tip& el){
        if(broj_el==0){
            prvi=new Cvor<Tip> {el, nullptr, nullptr};
            trenutni_pok=prvi;
            zadnji=prvi;
        }else if(trenutni_pok==zadnji){
            zadnji=new Cvor<Tip> {el, trenutni_pok, nullptr};
            trenutni_pok->next=zadnji;
        }else{
            Cvor<Tip> *pomocni=trenutni_pok->next;
            pomocni->before =new Cvor<Tip> {el, trenutni_pok, trenutni_pok->next};
            trenutni_pok->next=pomocni->before;
        }
        broj_el++;
    }
    
    Tip &operator[](int i){
         if(i<0 || i>=broj_el) throw "Lista je prazna!";                     
            Cvor<Tip> *pomocni;
            int brojac;
            int m=std::abs(index_posljednji_trazeni-i);
            
            if(m<broj_el-i && m<i){
                if(i>index_posljednji_trazeni){
                    brojac=index_posljednji_trazeni;
                    pomocni=posljednji_trazeni;
                    while (brojac!=i) {
                        pomocni=pomocni->next;
                        brojac++;
                    }
                }else{
                    brojac=index_posljednji_trazeni;
                    pomocni=posljednji_trazeni;
                    while (brojac!=i) {
                        pomocni=pomocni->before;
                        brojac--;
                    }
                }
            }else if(broj_el-i<=i){
                pomocni=zadnji;
                brojac=broj_el-1;
                while (brojac!=i) {
                    pomocni=pomocni->before;
                    brojac--;
                }
            }else{
                pomocni=prvi;
                brojac=0;
                while (brojac!=i) {
                    pomocni=pomocni->next;
                    brojac++;
                }
            }
            index_posljednji_trazeni=i;
            posljednji_trazeni=pomocni;
            return pomocni->element;
        }

    Tip operator[](int i) const {
        if(i<0 || i>=broj_el) throw "Lista je prazna!";                     
            Cvor<Tip> *pomocni;
            int brojac;
            int m=std::abs(index_posljednji_trazeni-i);
            
            if(m<broj_el-i && m<i){
                if(i>index_posljednji_trazeni){
                    brojac=index_posljednji_trazeni;
                    pomocni=posljednji_trazeni;
                    while (brojac!=i) {
                        pomocni=pomocni->next;
                        brojac++;
                    }
                }else{
                    brojac=index_posljednji_trazeni;
                    pomocni=posljednji_trazeni;
                    while (brojac!=i) {
                        pomocni=pomocni->before;
                        brojac--;
                    }
                }
            }else if(broj_el-i<=i){
                pomocni=zadnji;
                brojac=broj_el-1;
                while (brojac!=i) {
                    pomocni=pomocni->before;
                    brojac--;
                }
            }else{
                pomocni=prvi;
                brojac=0;
                while (brojac!=i) {
                    pomocni=pomocni->next;
                    brojac++;
                }
            }
            index_posljednji_trazeni=i;
            posljednji_trazeni=pomocni;
            return pomocni->element;
        }
};

template <typename Tip>

Tip dajMaksimum(const Lista<Tip> &n){
    if(n.brojElemenata()==0) throw "Lista je prazna!";
    Tip max=n[0];
    for(int i=0;i<n.brojElemenata();i++){
        if(n[i]>max){
            max=n[i];
        }
    }
    return max;
}



void testDodajIza(){
    DvostrukaLista<int> l;
    for(int i=0;i<1000;i++){
        l.dodajIza(i);
    }
    int brojPrije=l.brojElemenata();
    for(int i=0;i<5;i++){
        l.dodajIza(i);
    }
    if(brojPrije+5==l.brojElemenata()){
        std::cout<<"Test OK."<<std::endl;
    }else{
        std::cout<<"Pogresan rezultat."<<std::endl;
    }
}
void testDodajIspred(){
    DvostrukaLista<int> l;
    for(int i=0;i<1000;i++){
        l.dodajIspred(i);
    }
    int brojPrije=l.brojElemenata();
    for(int i=0;i<5;i++){
        l.dodajIspred(i);
    }
    if(brojPrije+5==l.brojElemenata()){
        std::cout<<"Test OK."<<std::endl;
    }else{
        std::cout<<"Pogresan rezultat."<<std::endl;
    }
}
void testTrenutnog(){
    DvostrukaLista<int> l;
    l.dodajIza(1);
    l.trenutni()=5;
    if(l.trenutni()==5) std::cout<<"Test OK."<<std::endl;
    else std::cout<<"Pogresan rezultat."<<std::endl;
}
void testKraj(){
    DvostrukaLista<char> l;
    for(char i='A';i<'F';i++){
        l.dodajIza(i);
    }
    char trenutni_kraj=l[l.brojElemenata()-1];
    l.kraj();
    if(l.trenutni()==trenutni_kraj) std::cout<<"Test OK."<<std::endl;
    else std::cout<<"Pogresan rezultat."<<std::endl;
}
void testPocetak(){
    DvostrukaLista<int> l;
    l.dodajIza(6);
    l.dodajIspred(2);
    int trenutni_pocetak=l[0];
    l.pocetak();
    if(l.trenutni()==trenutni_pocetak) std::cout<<"Test OK."<<std::endl;
    else std::cout<<"Pogresan rezultat."<<std::endl;
}
void testSljedeci(){
    DvostrukaLista<int> l;
    l.dodajIspred(1);
    l.dodajIza(2);
    if(l.sljedeci() && l.trenutni()==2) std::cout<<"Test OK."<<std::endl;
    else std::cout<<"Pogresan rezultat."<<std::endl;
}
void testPrethodni(){
    DvostrukaLista<int> l;
    l.dodajIspred(1);
    l.dodajIspred(2);
    if(l.prethodni() && l.trenutni()==2) std::cout<<"Test OK."<<std::endl;
    else std::cout<<"Pogresan rezultat."<<std::endl;
}
void testIndeksiranja(){
    DvostrukaLista<std::string> l;
    l.dodajIspred("Sredina");
    l.dodajIspred("Prije");
    l.dodajIza("Poslije");
    if(l[0]=="Prije" && l[1]=="Sredina" && l[2]=="Poslije") std::cout<<"Test OK."<<std::endl;
    else std::cout<<"Pogresan rezultat."<<std::endl;
}
void testObrisi(){
    DvostrukaLista<int> l;
    for(int i=0;i<6;i++){
        l.dodajIza(i);
    }
    int broj_el=l.brojElemenata();
    l.obrisi();
    l.obrisi();
    if(l.brojElemenata()==broj_el-2) std::cout<<"Test OK."<<std::endl;
    else std::cout<<"Pogresan rezultat."<<std::endl;
} 
void testDajMaksimum(){
    DvostrukaLista<int> l;
    for(int i=0;i<5000;i++){
        l.dodajIza(i);
    }
    if(dajMaksimum(l)==4999){
        std::cout<<"Test OK."<<std::endl;
    }else{
        std::cout<<"Pogresan rezultat."<<std::endl;
    }  
}


int main(){
    testDodajIza();
    testDodajIspred();
    testTrenutnog();
    testKraj();
    testPocetak();
    testSljedeci();
    testPrethodni();
    testIndeksiranja();
    testObrisi();
    testDajMaksimum();
    return 0;
}



