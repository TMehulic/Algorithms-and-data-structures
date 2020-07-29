#include <iostream>

template <typename Tip>

struct Cvor{
    Tip element;
    Cvor<Tip> *veza;
};

template <typename Tip>

class Red{
    
    Cvor<Tip>* pocetak;
    Cvor<Tip>* kraj;
    int broj_el;
    
public:
    
    Red(){
        pocetak=nullptr;
        kraj=nullptr;
        broj_el=0;
    }
    
    ~Red(){
        Cvor<Tip>*pomocni=pocetak;
        while(pocetak!=nullptr){
            pomocni=pocetak->veza;
            delete pocetak;
            pocetak=pomocni;
        }
        broj_el=0;
        pocetak=nullptr;
        kraj=nullptr;
    }
    
    Red(const Red &r){
        broj_el=r.broj_el;
        Cvor<Tip> *pomocni1, *pomocni(r.pocetak);
        pocetak=nullptr;
        kraj=nullptr;
        while(pomocni!=nullptr){
            if(pomocni==r.pocetak){
                pocetak=new Cvor<Tip> {pomocni->element,nullptr};
                pomocni1=pocetak;
            }else{
                pomocni1->veza=new Cvor<Tip> {pomocni->element, nullptr};
                pomocni1=pomocni1->veza;
            }
            if(pomocni->veza==nullptr){
                kraj=pomocni;
            }
            pomocni=pomocni->veza;
        }
    }
    
    Red &operator=(const Red &r){
        if(this==&r) return *this;
        this->brisi();
        broj_el=r.broj_el;
        Cvor<Tip> *pomocni1, *pomocni(r.pocetak);
        pocetak=nullptr;
        kraj=nullptr;
        while(pomocni!=nullptr){
            if(pomocni==r.pocetak){
                pocetak=new Cvor<Tip> {pomocni->element,nullptr};
                pomocni1=pocetak;
            }else{
                pomocni1->veza=new Cvor<Tip> {pomocni->element, nullptr};
                pomocni1=pomocni1->veza;
            }
            if(pomocni->veza==nullptr){
                kraj=pomocni;
            }
            pomocni=pomocni->veza;
        }
        return *this;
    }
    
    void brisi(){
        Cvor<Tip>*pomocni=pocetak;
        while(pocetak!=nullptr){
            pomocni=pocetak->veza;
            delete pocetak;
            pocetak=pomocni;
        }
        broj_el=0;
        pocetak=nullptr;
        kraj=nullptr;
    }
    
    void stavi(const Tip& el){
        if(broj_el==0){
            pocetak=new Cvor<Tip> {el,nullptr};
            kraj=pocetak;
        }else if(broj_el==1){
            pocetak->veza=new Cvor<Tip> {el, nullptr};
            kraj=pocetak->veza;
        }else{
            Cvor<Tip> *novi=new Cvor<Tip> {el, nullptr};
            kraj->veza=novi;
            kraj=novi;
        }
        broj_el++;
    }
    
    Tip skini(){
        if(broj_el==0) throw "Red je prazan!";
        Tip element=pocetak->element;
        Cvor<Tip>* pomocni=pocetak->veza;
        delete pocetak;
        pocetak=pomocni;
        broj_el--;
        return element;
    }
    
    Tip& celo(){
        if(broj_el==0) throw "Red je prazan!";
        return pocetak->element;
    }
    
    int brojElemenata(){
        return broj_el;
    }
    
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


void pomocna(Stek<int> &s,Red<int> &r){
    if(s.brojElemenata()==0) return;
    int el=s.vrh();
    s.skini();
    if(el%2!=0){
        r.stavi(el);
        r.stavi(r.skini());
    }
    pomocna(s,r);
    if(el%2==0){
        s.stavi(el);
    }
}

void premjestiNeparne(Stek<int> &s, Red<int> &r){
    pomocna(s,r);  
    int n=r.brojElemenata();
    for(int i=0;i<n;i++){
        s.stavi(r.skini());
    }
}


int main() {
    Stek<int> s;
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);
    s.stavi(4);
    s.stavi(5);
    s.stavi(6);
    s.stavi(7);
    s.stavi(8);
    Red<int> r;
    premjestiNeparne(s,r);
    int n=s.brojElemenata();
    for(int i=0;i<n;i++){
        std::cout<<s.skini();
    }
    return 0;
}
