#include <iostream>

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

bool provjeri_zagrade(std::string tekst){
    int i=0;
    Stek<char> stek;
    while(i<tekst.length()){
        if(i==0){
            stek.stavi(tekst[i]);
            i++;
            continue;
        }
    }
    
}

int main(){
    if(provjeri_zagrade("[()]{}{[()()]()}")) std::cout<<"OK!"<<std::endl;
    return 0;
}