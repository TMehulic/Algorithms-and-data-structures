#include <iostream>
#include <functional>
#include <string>

//Jednostavna hash funkcija
unsigned int hashfun(int ulaz, unsigned int max){
    return ulaz+1;
}


template <typename TipKljuca, typename TipVrijednosti>

class Mapa{

public:
    Mapa()=default;
    virtual ~Mapa(){};
    virtual int brojElemenata() const=0;
    virtual TipVrijednosti &operator[] (const TipKljuca& kljuc)=0;
    virtual TipVrijednosti operator[] (const TipKljuca& kljuc)const=0;
    virtual void obrisi() =0;
    virtual void obrisi(const TipKljuca& kljuc)=0;
    
};

template <typename TipKljuca, typename TipVrijednosti>

class NizMapa:public Mapa<TipKljuca,TipVrijednosti>{
    std::pair<TipKljuca,TipVrijednosti> *parovi;
    int broj_el;
    int kapacitet;
public:
    NizMapa(){
        parovi=new std::pair<TipKljuca,TipVrijednosti> [100];
        kapacitet=100;
        broj_el=0;
    }
    
    NizMapa(const NizMapa& mapa){
        parovi=new std::pair<TipKljuca,TipVrijednosti> [mapa.brojElemenata()];
        for(int i=0;i<mapa.brojElemenata();i++){
            parovi[i]=mapa.parovi[i];
        }
        broj_el=mapa.brojElemenata();
        kapacitet=mapa.brojElemenata();
    }
    
    NizMapa &operator=(const NizMapa &mapa){
        if(this==&mapa){
            return *this;
        }
        delete [] parovi;
        parovi=new std::pair<TipKljuca,TipVrijednosti> [mapa.kapacitet];
        for(int i=0;i<mapa.brojElemenata();i++){
            parovi[i]=mapa.parovi[i];
        }
        kapacitet=mapa.kapacitet;
        broj_el=mapa.brojElemenata();
        return *this;
    }
    
    ~NizMapa(){
        delete [] parovi;
        parovi=nullptr;
        broj_el=0;
    }
    
    int brojElemenata() const override{
        return broj_el;
    }
    
    TipVrijednosti &operator[] (const TipKljuca& kljuc){
        for(int i=0;i<broj_el;i++){
            if(parovi[i].first==kljuc){
                return parovi[i].second;
            }
        }
        
        if(broj_el<kapacitet){
            std::pair<TipKljuca,TipVrijednosti> par=std::pair<TipKljuca,TipVrijednosti> {kljuc,TipVrijednosti()}; 
            parovi[broj_el]=par;
            broj_el++;
            return parovi[broj_el-1].second;
        }
        
        std::pair<TipKljuca,TipVrijednosti> *novi=new std::pair<TipKljuca,TipVrijednosti> [kapacitet*2];
        for(int i=0;i<broj_el;i++){
            novi[i]=parovi[i];
        }
        novi[broj_el].first=kljuc;
        novi[broj_el].second=TipVrijednosti();
        broj_el++;
        kapacitet*=2;
        delete [] parovi;
        parovi=novi;
        return parovi[broj_el-1].second;
    }
    
    TipVrijednosti operator [](const TipKljuca &kljuc) const override{
        for(int i=0;i<broj_el;i++){
            if(parovi[i].first==kljuc){
                return parovi[i].second;
            }
        }
        return TipVrijednosti();
    }
    
    void obrisi() {
        broj_el=0;
    }
    
    void obrisi(const TipKljuca& kljuc){
        for(int i=0;i<broj_el;i++){
            if(parovi[i].first==kljuc){
                for(int j=i;j<broj_el-1;j++){
                    parovi[j]=parovi[j+1];
                }
                broj_el--;
                return;
            }
        }
        throw std::range_error("U mapi se ne nalazi element sa datim kljucem!");
    }
};


template<typename TipKljuca, typename TipVrijednosti>
class BSCvor{
public:    
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    BSCvor<TipKljuca,TipVrijednosti> *lijevo;
    BSCvor<TipKljuca,TipVrijednosti> *desno;
    BSCvor<TipKljuca,TipVrijednosti> *roditelj;
    
    BSCvor() {
        lijevo=desno=nullptr;
        roditelj=nullptr;
    }
    
    BSCvor(const TipKljuca &kljuc, const TipVrijednosti &vrijednost=TipVrijednosti(),BSCvor* lijevo=nullptr,BSCvor* desno=nullptr, BSCvor* roditelj=nullptr){
        this->kljuc=kljuc;
        this->vrijednost=vrijednost;
        this->lijevo=lijevo;
        this->desno=desno;
        this->roditelj=roditelj;
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa:public Mapa<TipKljuca,TipVrijednosti>{
    BSCvor<TipKljuca,TipVrijednosti>*korijen;
public:
    BinStabloMapa(){
        korijen=nullptr;
    }
    
    ~BinStabloMapa(){
        Brisi(korijen);
        korijen=nullptr;
    }
    
    
    BSCvor<TipKljuca,TipVrijednosti>* Trazi(BSCvor<TipKljuca,TipVrijednosti> *r, const TipKljuca& x)const{
        if(r==nullptr || r->kljuc==x)
            return r;
        else if(x<r->kljuc){
            return Trazi(r->lijevo,x);
        }else{
            return Trazi(r->desno,x);
        }
    }
    
    
    void Brisi(BSCvor<TipKljuca,TipVrijednosti> *r){
        if(r){
            Brisi(r->desno);
            Brisi(r->lijevo);
            delete r;
        }
    }
    
    void BrisiCvor(BSCvor<TipKljuca,TipVrijednosti>* &r, const TipKljuca &x){
        BSCvor<TipKljuca,TipVrijednosti> *p=r, *q=nullptr;
        BSCvor<TipKljuca,TipVrijednosti> *tmp=nullptr, *pp=nullptr, *rp=nullptr;
        while(p!=nullptr && x!=p->kljuc){
            q=p;
            if(x<p->kljuc){
                p=p->lijevo;
            }else{
                p=p->desno;
            }
        }
        if(p==nullptr){
            throw "Ne postoji trazeni element u mapi!";
        }
        if(p->lijevo==nullptr){
            rp=p->desno;
        }else if(p->desno==nullptr){
            rp=p->lijevo;
        }else{
            pp=p;
            rp=p->lijevo;
            tmp=rp->desno;
            while (tmp!=nullptr) {
                pp=rp;
                rp=tmp;
                tmp=rp->desno;
            }
            if(pp!=p){
                pp->desno=rp->lijevo;
                rp->lijevo=p->lijevo;
            }
            rp->desno=p->desno;
        }
        if(q==nullptr){
            korijen=rp;
        }else if(p==q->lijevo){
            q->lijevo=rp;
            if(rp!=nullptr){
                rp->roditelj=q;
            }
        }else{
            q->desno=rp;
            if(rp!=nullptr){
                rp->roditelj=q;
            }
        }
        delete p;
    }
    
    void Inorder(BSCvor<TipKljuca,TipVrijednosti> *r, int &brojac=0)const{
        if(r!=nullptr){
            Inorder(r->lijevo,brojac);
            brojac++;
            Inorder(r->desno,brojac);
        }
    }
    
    int brojElemenata() const{
        int brojac=0;
        Inorder(korijen,brojac);
        return brojac;
    }
    
    TipVrijednosti &operator [](const TipKljuca& kljuc){
        BSCvor<TipKljuca,TipVrijednosti>* trazeni=korijen;
        while(trazeni!=nullptr && kljuc!=trazeni->kljuc){
            if(kljuc<trazeni->kljuc){
                trazeni=trazeni->lijevo;
            }else{
                trazeni=trazeni->desno;
            }
        }
        if(trazeni!=nullptr){
            return trazeni->vrijednost;
        }
        
        trazeni=new BSCvor<TipKljuca,TipVrijednosti> {kljuc,TipVrijednosti()};
        BSCvor<TipKljuca,TipVrijednosti> *p=korijen, *q=nullptr;
        while(p!=nullptr){
            q=p;
            if(trazeni->kljuc<p->kljuc){
                p=p->lijevo;
            }else{
                p=p->desno;
            }
        }
        if(q==nullptr) korijen=trazeni;
        else{
            if(trazeni->kljuc<q->kljuc){
                q->lijevo=trazeni;
            }else{
                q->desno=trazeni;
            }
            trazeni->roditelj=q;
        }
        return trazeni->vrijednost;
    }
    
    TipVrijednosti operator[] (const TipKljuca& kljuc)const{
        BSCvor<TipKljuca,TipVrijednosti>* trazeni=Trazi(korijen,kljuc);
        if(trazeni!=nullptr){
            return trazeni->vrijednost;
        }
        return TipVrijednosti();
    }
    
    void obrisi(){
        Brisi(korijen);
        korijen=nullptr;
    }
    
    void obrisi(const TipKljuca& kljuc){
        BrisiCvor(korijen,kljuc);
    }
    
    void dajKopiju(BSCvor<TipKljuca,TipVrijednosti> *korijen){
        if(korijen){
            this->operator [](korijen->kljuc)=korijen->vrijednost;
            dajKopiju(korijen->desno);
            dajKopiju(korijen->lijevo);
        }
    }
    
    BinStabloMapa(const BinStabloMapa &mapa){
        korijen=nullptr;
        dajKopiju(mapa.korijen);
    }
    
    BinStabloMapa<TipKljuca, TipVrijednosti> &operator=(const BinStabloMapa<TipKljuca,TipVrijednosti> &mapa){
        if(&mapa==this){
            return *this;
        }
        obrisi();
        dajKopiju(mapa.korijen);
        return *this;
    }
    
};

template <typename TipKljuca, typename TipVrijednosti>

class HashMapa:public Mapa<TipKljuca,TipVrijednosti> {
    std::pair<TipKljuca,TipVrijednosti> **parovi;
    unsigned int max;
    int broj_el;
    unsigned int (*hashFunkcija)(TipKljuca,unsigned int);
    unsigned int h(TipKljuca kljuc, unsigned int i)const{
        return (hashFunkcija(kljuc,max)+i) % max;
    }
    std::pair<TipKljuca,TipVrijednosti> *del;
public:
    void definisiHashFunkciju(unsigned int (*func)(TipKljuca,unsigned int)){
        hashFunkcija=func;
    }
    
    HashMapa(){
        max=100;
        parovi=new std::pair<TipKljuca,TipVrijednosti> *[max] {};
        broj_el=0;
        del=new std::pair<TipKljuca,TipVrijednosti> {TipKljuca(),TipVrijednosti()};
        hashFunkcija=nullptr;
    }
    
    ~HashMapa(){
        obrisi();
        delete [] parovi;
        delete del;
    }
    
    HashMapa(const HashMapa &mapa){
        max=mapa.max;
        broj_el=mapa.broj_el;
        del=new std::pair<TipKljuca,TipVrijednosti> {TipKljuca(),TipVrijednosti()};
        hashFunkcija=mapa.hashFunkcija;
        parovi=new std::pair<TipKljuca,TipVrijednosti> *[max] {};
        for(int i=0;i<max/2;i++){
            if(mapa.parovi[i]!=nullptr && mapa.parovi[i]!=del){
                int j=0;
                do{
                    int index=h(mapa.parovi[i]->first,j);
                    if(parovi[index]==nullptr || parovi[index]==del){
                        parovi[index]=new std::pair<TipKljuca,TipVrijednosti> (*mapa.parovi[i]);
                        break;
                    }else{
                            j++;
                        }
                }while(j<max);
            }
        }
    }
    
    HashMapa &operator= (const HashMapa &mapa){
        if(this==&mapa){
            return *this;
        }
        obrisi();
        max=mapa.max;
        for(int i=0;i<max/2;i++){
            if(mapa.parovi[i]!=nullptr && mapa.parovi[i]!=del){
                int j=0;
                do{
                    int index=h(mapa.parovi[i]->first,j);
                    if(parovi[index]==nullptr || parovi[index]==del){
                        parovi[index]=new std::pair<TipKljuca,TipVrijednosti> (*mapa.parovi[i]);
                        break;
                    }else{
                            j++;
                        }
                }while(j<max);
            }
        }
        broj_el=mapa.broj_el;
        return *this;
    }
    
    int brojElemenata() const override{
        return broj_el;
    }
    
    void obrisi(const TipKljuca& kljuc){
        if(hashFunkcija==nullptr){
            throw "";
        }
        int index,i=0;
        do {
            index=h(kljuc,i);
            if(parovi[index]!=nullptr){
                if(parovi[index]->first==kljuc){
                    delete parovi[index];
                    parovi[index]=del;
                    broj_el--;
                    return;
                }
            }
            i++;
        }while(parovi[index]!=del && i!=max);
        throw std::range_error("Ne postoji element sa datim kljucem u mapi!");
    }
    
    void obrisi(){
        for(int i=0;i<max;i++){
            delete parovi[i];
            parovi[i]=nullptr;
        }
        broj_el=0;
    }
    
    TipVrijednosti &operator[] (const TipKljuca& kljuc){
        if(hashFunkcija==nullptr){
            throw "";
        }
        //Prvo da vidimo ima li trazenog kljuca
        unsigned int index;
        unsigned int i=0;
        do {
            index=h(kljuc,i);
            if(parovi[index]!=nullptr){
                // std::cout << parovi[index]->first << std::endl;
                if(parovi[index]->first==kljuc){
                    return parovi[index]->second;
                }else{
                    i++;
                }
            }
        }while(parovi[index]!=nullptr && i!=max);
        
        //Provjeravamo da li je kapacitet popunjen
        if(broj_el==max){
            std::pair<TipKljuca,TipVrijednosti> **novi=new std::pair<TipKljuca,TipVrijednosti>* [max*2]{};
            max*=2;
            for(int i=0;i<max/2;i++){
                if(parovi[i]!=nullptr && parovi[i]!=del){
                    int j=0;
                    do{
                        index=h(parovi[i]->first,j);
                        if(novi[index]==nullptr || novi[index]==del){
                            novi[index]=new std::pair<TipKljuca,TipVrijednosti> (*parovi[i]);
                            break;
                        }else{
                            j++;
                        }
                    }while(j<max);
                }
            }
            for(int i=0;i<max/2;i++){
                delete parovi[i];
                parovi[i]=nullptr;
            }
            delete [] parovi;
            parovi=novi;
        }
        
        //Dodajemo novi element
        i=0;
        do{
            index=h(kljuc,i);
            if(parovi[index]==nullptr || parovi[index]==del){
                parovi[index]=new std::pair<TipKljuca,TipVrijednosti> ();
                broj_el++;
                parovi[index]->first=kljuc;
                parovi[index]->second=TipVrijednosti();
                break;
            }else{
                i++;
            }
        }while(i<max);
        return parovi[index]->second;
    }
    
    TipVrijednosti operator[] (const TipKljuca& kljuc)const override{
        if(hashFunkcija==nullptr){
            throw "";
        }
        int index,i=0;
        do {
            index=h(kljuc,i);
            if(parovi[index]!=nullptr){
                if(parovi[index]->first==kljuc){
                    return parovi[index]->second;
                }else{
                    i++;
                }
            }
        }while(parovi[index]!=nullptr && i!=max);
        return TipVrijednosti();
    }
    
};




int main() {
    BinStabloMapa<int,int> stabloMapa;
    NizMapa<int,int> nizMapa;
    HashMapa<int,int> hashMapa;
    hashMapa.definisiHashFunkciju(hashfun);
    
    std::cout << "Dodavanje 5000 nasumično izabranih brojeva u BinStabloMapu." <<std::endl;
    
    clock_t vrijeme1=clock();
    srand(time(NULL));
    for(int i=0;i<5000;i++){
        stabloMapa[i]=std::rand();
    }
    
    clock_t vrijeme2=clock();
    
    double ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme dodavanja broja u BinStabloMapu ";
    std::cout << ukvrijeme/5000 <<"ms."<< std::endl<<std::endl;
    
    std::cout << "Dodavanje 5000 nasumično izabranih brojeva u NizMapu." <<std::endl;
    
    vrijeme1=clock();
    srand(time(NULL));
    for(int i=0;i<5000;i++){
        nizMapa[i]=std::rand();
    }
    
    vrijeme2=clock();
    
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme dodavanja broja u NizMapu ";
    std::cout << ukvrijeme/5000 <<"ms."<< std::endl<<std::endl;
    
    std::cout << "Dodavanje 5000 nasumično izabranih brojeva u HashMapu." <<std::endl;
    
    vrijeme1=clock();
    srand(time(NULL));
    for(int i=0;i<5000;i++){
        hashMapa[i]=std::rand();
    }
    
    vrijeme2=clock();
    
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme dodavanja broja u HashMapu ";
    std::cout << ukvrijeme/5000 <<"ms."<< std::endl<<std::endl;
    
    //Prilikom dodavanja elementa u BinStabloMapa, prvobitno je potrebno pretraziti stablo sto ima kompleksnost O(logn),
    //te zatim ako element ne postoji, treba pronaci mjesto gdje ga ubaciti, sto je takodjer O(logn), pa je ukupna kompleksnost
    // O(2logn), odnosno O(log(n^2))
    
    //Prilikom dodavanja elementa u NizMapa, imamo kompleksnost O(n), jer jednom vrsimo pretragu niza, te ako element ne postoji, dodajemo na kraj
    
    //Prilikom dodavanja elementa u HashMapa, imamo kompleksnost O(1), jer prvo pretrazimo da li element vec postoji
    //a pretraga je kompleksnosti O(1), te ako ne postoji dodamo element, sto je takodjer kompleksnost O(1)
    
    //NAPOMENA
    //U ovom zadatku je NizMapa modifikovan u odnosu na raniju implementaciju NizMapa
    //Ako je kapacitet popunjen, ne povecava se sa 100 vise, vec mnozimo trenutni kapacitet sa 2
    //Cilj je da imamo sto manje kopiranja
    
    std::cout << "Pristup svim elementima u BinStabloMapa." << std::endl;
    
    vrijeme1=clock();

    for(int i=0;i<5000;i++){
        stabloMapa[i];
    }
    
    vrijeme2=clock();
    
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme pristupa elementu u BinStabloMapa" << std::endl;
    std::cout << ukvrijeme/5000 <<"ms."<<std::endl<<std::endl;
    
    
    std::cout << "Pristup svim elementima u NizMapa." << std::endl;
    
    vrijeme1=clock();

    for(int i=0;i<5000;i++){
        nizMapa[i];
    }
    
    vrijeme2=clock();
    
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme pristupa elementu u NizMapa" << std::endl;
    std::cout << ukvrijeme/5000 <<"ms."<<std::endl<<std::endl;
    
    
    std::cout << "Pristup svim elementima u HashMapa." << std::endl;
    
    vrijeme1=clock();

    for(int i=0;i<5000;i++){
        hashMapa[i];
    }
    
    vrijeme2=clock();
    
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme pristupa elementu u HashMapa" << std::endl;
    std::cout << ukvrijeme/5000 <<"ms."<<std::endl<<std::endl;
    
    
    
    //Vrijeme pristupa elementu u BinStabloMapi ima kompleksnost O(logn), dok vrijeme pristupa elementu u NizMapi ima kompleksnost O(n)
    //Vrijeme pristupa elementu u HashMapi ima kompleksnost O(1), jer je dovoljno samo da dobije vrijednost hasha za dati element
    //te provjeri da li element ima u mapi, odnosno nije potrebno "redom" ici sve i porediti elemente
    
    std::cout << "Brisanje svih elemenata u BinStabloMapa." << std::endl;
    
    vrijeme1=clock();

    for(int i=0;i<5000;i++){
        stabloMapa.obrisi(i);
    }
    
    vrijeme2=clock();
    
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme brisanja elementa u BinStabloMapa" << std::endl;
    std::cout << ukvrijeme/5000 <<"ms."<<std::endl<<std::endl;
    
    
    std::cout << "Brisanje svih elemenata u NizMapa." << std::endl;
    
    vrijeme1=clock();

    for(int i=0;i<5000;i++){
        nizMapa.obrisi(i);
    }
    
    vrijeme2=clock();
    
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme brisanja elementa u NizMapa" << std::endl;
    std::cout << ukvrijeme/5000 <<"ms."<<std::endl<<std::endl;
    
    
    std::cout << "Brisanje svih elemenata u HashMapa." << std::endl;
    
    vrijeme1=clock();

    for(int i=0;i<5000;i++){
        hashMapa.obrisi(i);
    }
    
    vrijeme2=clock();
    
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme brisanja elementa u HashMapa" << std::endl;
    std::cout << ukvrijeme/5000 <<"ms."<<std::endl<<std::endl;
    
    //Sto se tice brisanja vidimo da je najvise vremena potrebno da se obrisu elementi iz NizMape, zato sto prilikom brisanja elementa
    //dolazi do premjestanja velikog broja elemenata
    
    
    //Konkretno u ovom primjeru, NizMapa daje bolje rezultate od BinStabloMape jer se radi o malim velicinama mapa; koristi se prosjecno vrijeme
    //Pri vecim velicinama mape, BinStabloMapa postaje efikasnija i dalje bolje rezultate od NizMape
    //U ovom primjeru najbolje rezultate daje HashMapa, a tome doprinosi jednostavno hashfunkcije
    
    
    return 0;
}
