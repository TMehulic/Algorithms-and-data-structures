#include <iostream>

template <typename Tip>
class Iterator;

template <typename Tip>

class Lista {
public:
    virtual ~Lista() {}
    Lista()=default;                                            //Implementirati samo u izvedenim klasama
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
    //Dodatne metode
};

template <typename Tip>

class NizLista:public Lista<Tip> {                                      /* NIZ LISTA */
 
    Tip**elementi;
    int broj_el;
    int index_trenutnog;
    int kapacitet;

public:
    
    NizLista(){
        elementi=new Tip*[1] {};                                //Proizvoljno velik broj, po potrebi kasnije mijenjati
        broj_el=0;
        kapacitet=1;
    }
    
    NizLista(const NizLista& l){                                    //Treba dodati u baznu klasu??
        elementi=new Tip*[l.brojElemenata()];
        kapacitet=l.brojElemenata();
        for(int i=0;i<l.brojElemenata();i++){
            elementi[i]=new Tip(*(l.elementi[i]));
        }
        broj_el=kapacitet;
    }
    
    NizLista &operator=(const NizLista &l){
        Tip **pomocni;
        pomocni=new Tip*[l.brojElemenata()];
        for(int i=0;i<l.brojElemenata();i++){
            pomocni[i]=new Tip(*(l.elementi[i]));
        }
        for(int i=0;i<broj_el;i++){
            delete elementi[i];
        }
        delete [] elementi;
        elementi=pomocni;
        broj_el=l.brojElemenata();
        kapacitet=l.brojElemenata();
        return *this;
    }
    
    ~NizLista(){
        for(int i=0;i<broj_el;i++){
            delete elementi[i];
        }
        delete [] elementi;
    }
    
    int brojElemenata() const{
        return broj_el;
    }
    
    Tip trenutni() const{
        if(broj_el==0) throw "Izuzetak trenutni";
        return *(elementi[index_trenutnog]);
    }
    
    Tip& trenutni(){
        if(broj_el==0) throw "Izuzetak trenutni";
        return *(elementi[index_trenutnog]);
    }
    
    bool prethodni() {
        if(broj_el==0) throw"Izuzetak prethodni";
        if(index_trenutnog==0) return false;
        index_trenutnog--;
        return true;
    }
    
    bool sljedeci(){
        if(broj_el==0) throw "Izuzetak sljedeci";
        if(index_trenutnog==broj_el-1) return false;
        index_trenutnog++;
        return true;
    }
    
    void pocetak(){
        if(broj_el==0) throw"Izuzetak pocetak";
        index_trenutnog=0;
    }
    
    void kraj(){
        if(broj_el==0) throw"Izuzetak kraj";
        index_trenutnog=broj_el-1;
    }
    
    void obrisi(){
        if(broj_el==0) throw"Izuzetak obrisi";
        if(index_trenutnog!=broj_el-1){
            delete elementi[index_trenutnog];
            for(int i=index_trenutnog;i<broj_el-1;i++){
                elementi[i]=elementi[i+1];
            }
        }else{
            delete elementi[index_trenutnog];
            index_trenutnog--;
        }
        broj_el--;
    }
    
    void dodajIspred(const Tip& el){
        if(broj_el==0){
            elementi[0]=new Tip(el);
            broj_el++;
            index_trenutnog=0;
        }else if(broj_el<kapacitet){
            for(int i=broj_el;i>index_trenutnog;i--){
                elementi[i]=elementi[i-1];
            }
            broj_el++;
            elementi[index_trenutnog]=new Tip(el);
            index_trenutnog++;
        }else{                                                  //Prosiriti kapacitet
            Tip**pomocni=new Tip*[kapacitet*2];
            for(int i=0;i<broj_el;i++){                         //Prebaci elemente u novi niz
                pomocni[i]=new Tip(*(elementi[i]));
                delete elementi[i];
            }
            delete [] elementi;
            elementi=pomocni;
            pomocni=nullptr;
            kapacitet*=2;
            for(int i=broj_el;i>index_trenutnog;i--){
                elementi[i]=elementi[i-1];
            }
            broj_el++;
            elementi[index_trenutnog]=new Tip(el);
            index_trenutnog++;
            
        }                                                  
    }       
    
    void dodajIza(const Tip& el){
        if(broj_el==0){                                         
            elementi[0]=new Tip(el);
            index_trenutnog=0;
            broj_el++;
        }else if(broj_el<kapacitet){                            
            if(index_trenutnog+1==broj_el){                     
                elementi[broj_el]=new Tip(el);
                broj_el++;
            }else{
                for(int i=broj_el;i>index_trenutnog+1;i--){
                    elementi[i]=elementi[i-1];
                }
                broj_el++;
                elementi[index_trenutnog+1]=new Tip(el);                
            } 
        }else{
            Tip **pomocni=new Tip*[kapacitet*2];
            for(int i=0;i<broj_el;i++){
                pomocni[i]=new Tip(*(elementi[i]));
                delete elementi[i];
            }
            delete [] elementi;
            elementi=pomocni;
            pomocni=nullptr;
            kapacitet*=2;
            if(index_trenutnog+1==broj_el){                     
                elementi[broj_el]=new Tip(el);
                broj_el++;
            }else{
                for(int i=broj_el;i>index_trenutnog+1;i--){
                    elementi[i]=elementi[i-1];
                }
                broj_el++;
                elementi[index_trenutnog+1]=new Tip(el);                
            } 
            
        }
    } 
    
    Tip &operator[](int index){
        if(index<0 || index>=broj_el) throw"Izuzetak index";
        return *elementi[index];
    }
    
    Tip operator[] (int index) const{
        if(index<0 || index>=broj_el) throw"Izuzetak index";
        return *(elementi[index]);
    }
    
    friend class Iterator<Tip>;
};



template <typename Tip>                                                 /* STRUKTURA CVOR */

struct Cvor{
    Tip element;
    Cvor* veza;
};

template <typename Tip>

class JednostrukaLista:public Lista<Tip> {                              /* JEDNOSTRUKA LISTA */
    
    int broj_el;
    Cvor<Tip> *prvi;
    Cvor<Tip> *trenutni_pok;
    
public:
    
    JednostrukaLista() {
        broj_el=0;
        prvi=nullptr;
        trenutni_pok=nullptr;
    }
    
    ~JednostrukaLista(){                
        while(trenutni_pok!=nullptr){
            trenutni_pok=prvi->veza;
            delete prvi;
            prvi=trenutni_pok;
        }
        broj_el=0;
    }
    
    JednostrukaLista(const JednostrukaLista &l){
        broj_el=l.broj_el;
        if(l.prvi==nullptr){
            prvi=nullptr;
            trenutni_pok=nullptr;
            broj_el=0;
        }else{
            Cvor<Tip> *pomocni1=l.prvi;
            while(pomocni1!=nullptr){
                if(pomocni1==l.prvi){                  //Prvi element
                    prvi=new Cvor<Tip> {pomocni1->element, nullptr};
                    trenutni_pok=prvi;
                }else{
                    trenutni_pok->veza=new Cvor<Tip> {pomocni1->element, nullptr};
                    trenutni_pok=trenutni_pok->veza;
                }
                pomocni1=pomocni1->veza;
            }
        }
    }
    
    JednostrukaLista &operator=(const JednostrukaLista&l){
        if(this==&l) return *this;          //Samododjela
        while(trenutni_pok!=nullptr){       //Dealocirati sve zauzeto
            trenutni_pok=prvi->veza;
            delete prvi;
            prvi=trenutni_pok;
        }
        broj_el=l.brojElemenata();
        
                                            //Onda kao kod konstruktora sve isto uraditi
        
        if(l.prvi==nullptr){
            prvi=nullptr;
            trenutni_pok=nullptr;
            broj_el=0;
        }else{
            Cvor<Tip> *pomocni1=l.prvi;
            while(pomocni1!=nullptr){
                if(pomocni1==l.prvi){                  //Prvi element
                    prvi=new Cvor<Tip> {pomocni1->element, nullptr};
                    trenutni_pok=prvi;
                }else{
                    trenutni_pok->veza=new Cvor<Tip> {pomocni1->element, nullptr};
                    trenutni_pok=trenutni_pok->veza;
                }
                pomocni1=pomocni1->veza;
            }
        }
        
    }
    
    int brojElemenata() const { return broj_el; }
    
    Tip trenutni() const {
        if(broj_el==0) throw "Lista je prazna!";
        return trenutni_pok->element;
    }
    
    Tip& trenutni(){
        if(broj_el==0) throw "Lista je prazna!";
        return trenutni_pok->element;
    }
    
    bool prethodni(){
        if(broj_el==0) throw "Lista je prazna!";
        if(trenutni_pok==prvi) return false;
        Cvor<Tip>* pomocni=prvi;
        while(pomocni->veza!=trenutni_pok){
            pomocni=pomocni->veza;
        }
        trenutni_pok=pomocni;
        return true;
    }
    
    bool sljedeci(){
        if(broj_el==0) throw "Lista je prazna!";
        if(trenutni_pok->veza==nullptr) return false;
        trenutni_pok=trenutni_pok->veza;
        return true;
    }
    
    void pocetak(){
        if(broj_el==0) throw "Lista je prazna!";
        trenutni_pok=prvi;
    }
    
    void kraj(){
        if(broj_el==0) throw "Lista je prazna!";
        if(prvi->veza==nullptr) trenutni_pok=prvi;
        else{
            while(trenutni_pok->veza!=nullptr){
                trenutni_pok=trenutni_pok->veza;
            }
        }
    }
    
    void obrisi(){
        if(broj_el==0) throw "Lista je prazna!";
        if(trenutni_pok==prvi){                         //Ako je trenutni prvi
            if(broj_el==1){
                prvi=nullptr;
                delete trenutni_pok;
                trenutni_pok=nullptr;
                broj_el--;
            }else{
                prvi=trenutni_pok->veza;
                delete trenutni_pok;
                trenutni_pok=prvi;
                broj_el--;
            }
            return;
        }
        Cvor <Tip>* pomocni=prvi;
        while(pomocni->veza!=trenutni_pok){
            pomocni=pomocni->veza;
        }
        if(trenutni_pok->veza==nullptr){                //Ako je trenutni zadnji
            pomocni->veza=nullptr;
            delete trenutni_pok;
            trenutni_pok=pomocni;
        }else{
            pomocni->veza=pomocni->veza->veza;          //Ostalo
            delete trenutni_pok;
            trenutni_pok=pomocni->veza;
        }
        broj_el--;
    }
    
    void dodajIspred(const Tip& el){
        if(broj_el==0){
            prvi=new Cvor<Tip>{el, nullptr};
            prvi->veza=nullptr;
            trenutni_pok=prvi;
            broj_el++;
        }else{
            if(prvi==trenutni_pok){
                prvi=new Cvor<Tip> {el, trenutni_pok};
                broj_el++;
                return;
            }
            Cvor<Tip>* pomocni=prvi;
            while(pomocni->veza!=trenutni_pok){
                pomocni=pomocni->veza;
            }
            Cvor<Tip>*novi= new Cvor<Tip> {el,trenutni_pok};
            pomocni->veza=novi;
            broj_el++;
        }
    }
    
    void dodajIza(const Tip& el){
        Cvor<Tip> *novi= new Cvor<Tip> {el, nullptr};
        if(broj_el==0){
            prvi=novi;
            trenutni_pok=prvi;
            broj_el++;
        }else{
            novi->veza=trenutni_pok->veza;
            trenutni_pok->veza=novi;
            broj_el++;
        }
    }
    
    Tip &operator [] (int index){
        if(index<0 || index>=broj_el) throw "Pogresan index!";
        if(broj_el==0) throw "Lista je prazna!";
        int brojac=0;
        Cvor<Tip>* vrijednost=prvi;
        while(brojac!=index){
            vrijednost=vrijednost->veza;
            brojac++;
        }
        return vrijednost->element;
    }
    
    Tip operator [] (int index) const{
        if(index<0 || index>=broj_el) throw "Pogresan index!";
        if(broj_el==0) throw "Lista je prazna!";
        int brojac=0;
        Cvor<Tip>* vrijednost=prvi;
        while(brojac!=index){
            vrijednost=vrijednost->veza;
            brojac++;
        }
        return vrijednost->element;
    }
    
    friend class Iterator<Tip>;

    
};

template<typename Tip>

class Iterator{
    const NizLista<Tip> *niz;
    const JednostrukaLista<Tip> *lista;
    int trenutniNiz;
    typename JednostrukaLista<Tip>::Cvor*trenutniLista;
public:
    
};





                                                                    /* F U N K C I J E */
                                                                    
                                                                    
template <typename Tip>

void dodajiIspred(Lista<Tip>& l, int n){                            //Poziva metodu dodajIza, nakong ovog ce iz kolikoIma
    for(int i=0;i<n;i++){                                           //biti testirano da li je broj unesenih jednak broju n      
        l.dodajIspred(i+1);
    }
}

template <typename Tip>

void dodajiIza(Lista<Tip>& l, int n){                               //Poziva metodu dodajIza, nakong ovog ce iz kolikoIma
    for(int i=0;i<n;i++){                                           //biti testirano da li je broj unesenih jednak broju n
        l.dodajIza(i+1);
    }
}

template <typename Tip>

int kolikoIma(const Lista<Tip>&l){                                  //Testira metodu brojElemenata
    return l.brojElemenata();
}

template <typename Tip>

void ispisi(const Lista<Tip> &l){                                   //Testira indeksiranje i da li su elementi uneseni
    for(int i=0;i<l.brojElemenata();i++){
        if(i!=l.brojElemenata()-1){
            std::cout<<l[i]<<", ";;
        } else std::cout<<l[i]<<std::endl;
    }
}

template <typename Tip>

Tip dajTrenutni(const Lista<Tip> &l){                               //Testira ispravnost metode trenutni
    return l.trenutni();
}

template <typename Tip>

void pomjeriNaKraj(Lista<Tip> &l){                                  //Testira pomjeranje trenutnog na kraj
    l.kraj();
}

template <typename Tip>

void pomjeriNaPocetak(Lista<Tip> &l){                               //Testira pomjeranje trenutnog na pocetak
    l.pocetak();
}

template <typename Tip>

bool dajSljedeci(Lista<Tip> &l){                                    //Testira prebacivanje na sljedeci element
    return l.sljedeci();
}

template <typename Tip>

bool dajPrethodni(Lista<Tip> &l){                                   //Testira prebacivanje na prethodni element
    return l.prethodni();
}

template <typename Tip>                                             //Brisanje elemenata

void brisi(Lista<Tip> &l){
    while(l.brojElemenata()!=0){
        l.obrisi();
    }
}

template <typename Tip>                                             //Izmjena vrijednosti trenutnog elementa

void izmijeniTrenutni(Lista<Tip> &l, Tip element){
    l.trenutni()=element;
}





int main() {
    Lista<int> *lista;
    std::cout<<"Pozdrav! \nUnesite slovo \"n\" ako zelite kreirati NizLista, a slovo \"j\" ako zelite kreirati jednostruku listu."<<std::endl;
    char slovo;
    std::cin>>slovo;
    std::cin.clear();
    std::cin.ignore(1000,'\n');
    if(slovo=='n'){
        lista=new NizLista<int>;
    }else if(slovo=='j'){
        lista=new JednostrukaLista<int>;
    }
    clock_t vrijeme1 = clock();
    for(int i=0;i<=100;i++){
        lista->dodajIspred(i);
    }
    clock_t vrijeme2 = clock();
    int ukvrijeme = (vrijeme2-vrijeme1) / (CLOCKS_PER_SEC /1000);
    std::cout<<ukvrijeme<<"ms.";
    
   /* vrijeme1=clock();
    for(int i=0;i<=10000;i++){
        std::cout<<*(lista[i]);
    }
    vrijeme2 = clock();
    int ukvrijeme = (vrijeme2-vrijeme1) / (CLOCKS_PER_SEC /1000); */
    delete lista; 
    return 0;
}
