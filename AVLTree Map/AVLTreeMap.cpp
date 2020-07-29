#include <iostream>
#include <iomanip>

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



template<typename TipKljuca, typename TipVrijednosti>
class AVLCvor{
public:    
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    AVLCvor<TipKljuca,TipVrijednosti> *lijevo;
    AVLCvor<TipKljuca,TipVrijednosti> *desno;
    AVLCvor<TipKljuca,TipVrijednosti> *roditelj;
    
    int balans;
    
    AVLCvor() {
        lijevo=desno=nullptr;
        roditelj=nullptr;
    }
    
    AVLCvor(const TipKljuca &kljuc, const TipVrijednosti &vrijednost=TipVrijednosti(),AVLCvor* lijevo=nullptr,AVLCvor* desno=nullptr, AVLCvor* roditelj=nullptr){
        this->kljuc=kljuc;
        this->vrijednost=vrijednost;
        this->lijevo=lijevo;
        this->desno=desno;
        this->roditelj=roditelj;
        balans=0;
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
class AVLStabloMapa:public Mapa<TipKljuca,TipVrijednosti>{
    AVLCvor<TipKljuca,TipVrijednosti>*korijen;
public:
    AVLStabloMapa(){
        korijen=nullptr;
    }
    
    ~AVLStabloMapa(){
        Brisi(korijen);
        korijen=nullptr;
    }
    
    
    AVLCvor<TipKljuca,TipVrijednosti>* Trazi(AVLCvor<TipKljuca,TipVrijednosti> *r, const TipKljuca& x)const{
        if(r==nullptr || r->kljuc==x)
            return r;
        else if(x<r->kljuc){
            return Trazi(r->lijevo,x);
        }else{
            return Trazi(r->desno,x);
        }
    }
    
    
    void Brisi(AVLCvor<TipKljuca,TipVrijednosti> *r){
        if(r){
            Brisi(r->desno);
            Brisi(r->lijevo);
            delete r;
        }
    }
    
    void BrisiCvor(AVLCvor<TipKljuca,TipVrijednosti>* &r, const TipKljuca &x){
        AVLCvor<TipKljuca,TipVrijednosti> *p=r, *q=nullptr;
        AVLCvor<TipKljuca,TipVrijednosti> *tmp=nullptr, *pp=nullptr, *rp=nullptr;
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
                //Ovdje ispravljeno povezivanje s parent cvorom
                pp->desno=rp->lijevo;
                if(pp->desno!=nullptr){
                    pp->desno->roditelj=pp;
                }
                rp->lijevo=p->lijevo;
                if(rp->lijevo!=nullptr){
                    rp->lijevo->roditelj=rp;
                }
            }
            rp->desno=p->desno;
            if(rp->desno!=nullptr){
                rp->desno->roditelj=rp;
            }
        }
        
        if(q==nullptr){
            korijen=rp;
            //Ovdje postavljeno da rp nema roditelja, jer je to korijen
            if(rp!=nullptr){
                rp->roditelj=nullptr;
            }
        }else if(p==q->lijevo){
            q->lijevo=rp;
            if(rp!=nullptr){
                rp->roditelj=q;
            }
            q->balans--;
        }else{
            q->desno=rp;
            if(rp!=nullptr){
                rp->roditelj=q;
            }
           q->balans++; 
           
        }
        delete p;
    }
    
    void Inorder(AVLCvor<TipKljuca,TipVrijednosti> *r, int &brojac=0)const{
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
    
    void desnaRotacija(AVLCvor<TipKljuca,TipVrijednosti> *cvor){
        //U slucaju dvostruke rotacije
        if(cvor->balans*cvor->lijevo->balans<0){
            lijevaRotacija(cvor->lijevo);
        }
        
        AVLCvor<TipKljuca,TipVrijednosti> *pomocni=cvor->lijevo;
        pomocni->roditelj=cvor->roditelj;
        if(cvor->roditelj!=nullptr){
            if(cvor->roditelj->lijevo==cvor){
                cvor->roditelj->lijevo=pomocni;
            }else{
                cvor->roditelj->desno=pomocni;
            }
        }else{
            korijen=pomocni;
        }
        
        cvor->lijevo=pomocni->desno;
        cvor->roditelj=pomocni;
        if(cvor->lijevo!=nullptr){
            cvor->lijevo->roditelj=cvor;
        }
        pomocni->desno=cvor;
        cvor->roditelj=pomocni;
        
        if(pomocni->balans<0){
            cvor->balans--;
        }else{
            cvor->balans=cvor->balans-pomocni->balans-1;
        }
        
        if(cvor->balans>0){
            pomocni->balans--;
        }else{
            pomocni->balans=pomocni->balans+cvor->balans-1;
        }
    }
    
    void lijevaRotacija(AVLCvor<TipKljuca,TipVrijednosti> *cvor){
        //U slucaju dvostruke rotacije
        if(cvor->balans*cvor->desno->balans<0){
            desnaRotacija(cvor->desno);
        }
        
        AVLCvor<TipKljuca,TipVrijednosti> *pomocni=cvor->desno;
        pomocni->roditelj=cvor->roditelj;
        if(cvor->roditelj!=nullptr){
            if(cvor->roditelj->lijevo==cvor){
                cvor->roditelj->lijevo=pomocni;
            }else{
                cvor->roditelj->desno=pomocni;
            }
        }else{
            korijen=pomocni;
        }
        
        cvor->desno=pomocni->lijevo;
        cvor->roditelj=pomocni;
        if(pomocni->lijevo!=nullptr){
            pomocni->lijevo->roditelj=cvor;
        }
        pomocni->lijevo=cvor;
        cvor->roditelj=pomocni;
        
        if(cvor->balans>0){
            cvor->balans++;
        }else{
            cvor->balans=cvor->balans-pomocni->balans+1;
        }
        
        if(cvor->balans>0){
            pomocni->balans=pomocni->balans+cvor->balans+1;
        }else{
            pomocni->balans++;
        }
    }
    
    void azurirajBalans(AVLCvor<TipKljuca,TipVrijednosti> *cvor){
        if(cvor->roditelj==nullptr) return;
        if(cvor==cvor->roditelj->lijevo){
            cvor->roditelj->balans++;
        }else{
            cvor->roditelj->balans--;
        }
        
        
        if(cvor->roditelj->balans<-1){
            lijevaRotacija(cvor->roditelj);
        }else if(cvor->roditelj->balans>1){
            desnaRotacija(cvor->roditelj);
        }else{
            azurirajBalans(cvor->roditelj);
        }
    }
    
    
    TipVrijednosti &operator [](const TipKljuca& kljuc){        
        //Ovaj dio izbacen, jer je moguce u jednoj petlji ustanoviti da li ima trazeni element
        
        // AVLCvor<TipKljuca,TipVrijednosti>* novi=korijen;
        // while(novi!=nullptr && kljuc!=novi->kljuc){
        //     if(kljuc<novi->kljuc){
        //         novi=novi->lijevo;
        //     }else{
        //         novi=novi->desno;
        //     }
        // }
        // if(novi!=nullptr){
        //     return novi->vrijednost;
        // }
        
        
        AVLCvor<TipKljuca,TipVrijednosti> *p=korijen, *q=nullptr;
        while(p!=nullptr){
            q=p;
            if(p->kljuc==kljuc) return p->vrijednost;
            if(kljuc<p->kljuc){
                p=p->lijevo;
            }else{
                p=p->desno;
            }
        }
        
        
        AVLCvor<TipKljuca,TipVrijednosti> *novi=new AVLCvor<TipKljuca,TipVrijednosti> {kljuc,TipVrijednosti()};
        novi->roditelj=q;
        
        //Ovdje dolazi do izmjene u odnosu na BinStabloMapu, jer je potrebno uvijek voditi racuna o balansu prilikom dodavanje
        if(q==nullptr) korijen=novi;
        else if(q->lijevo==nullptr && q->desno==nullptr){  
        
            if(novi->kljuc<q->kljuc){
                q->lijevo=novi;
            }else{
                q->desno=novi;
            }
            azurirajBalans(novi);
        }else{
            if(q->desno==nullptr){
                q->desno=novi;
            }else{
                q->lijevo=novi;
            }
            q->balans=0;
        }
        return novi->vrijednost;
    }
    
    TipVrijednosti operator[] (const TipKljuca& kljuc)const{
        AVLCvor<TipKljuca,TipVrijednosti>* trazeni=Trazi(korijen,kljuc);
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
    
    void dajKopiju(AVLCvor<TipKljuca,TipVrijednosti> *korijen){
        if(korijen){
            this->operator [](korijen->kljuc)=korijen->vrijednost;
            dajKopiju(korijen->desno);
            dajKopiju(korijen->lijevo);
        }
    }
    
    AVLStabloMapa(const AVLStabloMapa &mapa){
        korijen=nullptr;
        dajKopiju(mapa.korijen);
    }
    
    AVLStabloMapa<TipKljuca, TipVrijednosti> &operator=(const AVLStabloMapa<TipKljuca,TipVrijednosti> &mapa){
        if(&mapa==this){
            return *this;
        }
        obrisi();
        dajKopiju(mapa.korijen);
        return *this;
    }
    
};

int main() {
    BinStabloMapa<int,int> stabloMapa;
    AVLStabloMapa<int,int> avlMapa;
    
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
    
    std::cout << "Dodavanje 5000 nasumično izabranih brojeva u AVLMapu." <<std::endl;
    
    vrijeme1=clock();
    srand(time(NULL));
    for(int i=0;i<5000;i++){
        avlMapa[i]=std::rand();
    }
    
    vrijeme2=clock();
    
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme dodavanja broja u AVLMapu ";
    std::cout << ukvrijeme/5000 <<"ms."<< std::endl<<std::endl;
    
    
    std::cout << "Pristup svim elementima u BinStabloMapa." << std::endl;
    
    vrijeme1=clock();

    for(int i=0;i<5000;i++){
        stabloMapa[i];
    }
    
    vrijeme2=clock();
    
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme pristupa elementu u BinStabloMapa" << std::endl;
    std::cout << ukvrijeme/5000 <<"ms."<<std::endl<<std::endl;
    
    
    std::cout << "Pristup svim elementima u AVLMapa." << std::endl;
    
    vrijeme1=clock();

    for(int i=0;i<5000;i++){
        avlMapa[i];
    }
    
    vrijeme2=clock();
    
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme pristupa elementu u AVLMapa" << std::endl;
    std::cout << ukvrijeme/5000 <<"ms."<<std::endl<<std::endl;
    
    
    std::cout << "Brisanje svih elemenata u BinStabloMapa." << std::endl;
    
    vrijeme1=clock();

    for(int i=0;i<5000;i++){
        stabloMapa.obrisi(i);
    }
    
    vrijeme2=clock();
    
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme brisanja elementa u BinStabloMapa" << std::endl;
    std::cout << ukvrijeme/5000 <<"ms."<<std::endl<<std::endl;
    
    
    std::cout << "Brisanje svih elemenata u AVLMapa." << std::endl;
    
    vrijeme1=clock();

    for(int i=0;i<5000;i++){
        avlMapa.obrisi(i);
    }
    
    vrijeme2=clock();
    
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    
    std::cout << "Prosječno vrijeme brisanja elementa u AVLMapa" << std::endl;
    std::cout << ukvrijeme/5000 <<"ms."<<std::endl<<std::endl;
    return 0;
}
