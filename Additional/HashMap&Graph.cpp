#include <iostream>
#include <vector>
#include <utility>
#include <list>
#include <queue>
#include <cmath>

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

//NAPOMENA 
//Prilikom pokretanja testova nakon implementacije HashMapaLan, izbacilo je gresku tipa "Not logged in".
//Ovo pisem u slucaju da ako tokom pregledanja zadace odjednom "izbaci" informaciju, da je veca kolicina koda otkucana odjednom.

template <typename TipKljuca, typename TipVrijednosti>

class HashMapaLan:public Mapa<TipKljuca,TipVrijednosti>{
    std::vector<std::list<std::pair<TipKljuca,TipVrijednosti>>> mapa;
    int broj_el=0;
    unsigned int max=1000;
    unsigned int (*hashFunkcija)(TipKljuca,unsigned int);
public:
    void definisiHashFunkciju(unsigned int (*func)(TipKljuca,unsigned int)){
        hashFunkcija=func;
    }
    
    
    HashMapaLan(){
        hashFunkcija=nullptr;
        mapa.resize(max);
    }
    
    int brojElemenata() const override{
        return broj_el;
    }
    
    TipVrijednosti &operator [](const TipKljuca &kljuc){
        if(hashFunkcija==nullptr) throw std::logic_error("Nije definisana hash funkcija!");
        unsigned int index=hashFunkcija(kljuc,mapa.size());
        auto it=mapa[index].begin();
        while(it!=mapa[index].end() && kljuc>it->first){
            it++;
        }
        if(it==mapa[index].end() || it->first!=kljuc){
            std::pair<TipKljuca,TipVrijednosti> novi {kljuc,TipVrijednosti()};
            it=mapa[index].insert(it,novi);
            broj_el++;
            if(broj_el==max) max*=2;
        }
        return it->second;
    }
    
    TipVrijednosti operator [](const TipKljuca &kljuc) const override{
        if(hashFunkcija==nullptr) throw std::logic_error("Nije definisana hash funkcija!");
        unsigned int index=hashFunkcija(kljuc,mapa.size());
        auto it=mapa[index].begin();
        while(it!=mapa[index].end() && kljuc>it->first){
            it++;
        }
        if(it==mapa[index].end() || it->first!=kljuc) return TipVrijednosti();
        return it->second;
    }
    
    void obrisi(){
        for(int i=0;i<mapa.size();i++){
            mapa[i].clear();
        }
        broj_el=0;
    }
    void obrisi(const TipKljuca& kljuc){
        if(hashFunkcija==nullptr) throw std::logic_error("Nije definisana hash funkcija!");
        unsigned int index=hashFunkcija(kljuc,mapa.size());
        auto it=mapa[index].begin();
        while(it!=mapa[index].end() && kljuc>it->first){
            it++;
        }
        if(it==mapa[index].end() || kljuc!=it->first) throw "Greska";
        
        auto pom=it++;
        mapa[index].erase(pom,it);
        broj_el--;
    }
};


//DRUGI ZADATAK

template <typename TipOznake>
class Cvor;

template <typename TipOznake>
class Grana;

template <typename TipOznake>
class UsmjereniGraf;

template <typename TipOznake>
class GranaIterator {
    UsmjereniGraf<TipOznake> *graf;
    int polazniCvor,dolazniCvor;            
public:
    GranaIterator(UsmjereniGraf<TipOznake> *graf, int i, int j){
        this->graf=graf;
        this->polazniCvor=i;
        this->dolazniCvor=j;
    }
    
    Grana<TipOznake> operator*() {
        return graf->dajGranu(polazniCvor,dolazniCvor);
    }
    
    bool operator==(const GranaIterator &iter) const{
        return (graf==iter.graf && polazniCvor==iter.polazniCvor && dolazniCvor==iter.dolazniCvor);
    }
    bool operator!=(const GranaIterator &iter) const{
        return !(*this==iter);
    }
    
    GranaIterator& operator++(){
        do{
            if(dolazniCvor+1>=graf->dajBrojCvorova()){  //Ako je doslo do kraja matrice        
                dolazniCvor=0;                          //Prva kolona    
                polazniCvor++;                          //Idi u novi red
            }else{
                dolazniCvor++;                          
            }
        }while(polazniCvor<graf->dajBrojCvorova() && !graf->postojiGrana(polazniCvor,dolazniCvor));
        return *this;
    }
    
    GranaIterator operator ++(int){
        GranaIterator<TipOznake> pomocni=*this;
        ++(*this);
        return pomocni;
    }
};  

template <typename TipOznake>
class Grana{
    UsmjereniGraf<TipOznake> *graf;
    int polazniCvor, dolazniCvor;
public:
    Grana(UsmjereniGraf<TipOznake> *graf, int polazni, int dolazni) {
        this->graf=graf;
        polazniCvor=polazni;
        dolazniCvor=dolazni;
    }
    float dajTezinu() const {
        return graf->dajTezinuGrane(polazniCvor,dolazniCvor);
    }
    void postaviTezinu(float tezina){
        graf->postaviTezinuGrane(polazniCvor,dolazniCvor,tezina);
    }
    TipOznake dajOznaku(){
        return graf->dajOznakuGrane(polazniCvor,dolazniCvor);
    }
    void postaviOznaku(TipOznake oznaka){
        graf->postaviOznakuGrane(polazniCvor,dolazniCvor,oznaka);
    }
    Cvor<TipOznake> dajPolazniCvor(){
        return graf->dajCvor(polazniCvor);
    }
    Cvor<TipOznake> dajDolazniCvor(){
        return graf->dajCvor(dolazniCvor);
    }
};

template <typename TipOznake>
class Cvor{
    int redniBroj;
    UsmjereniGraf<TipOznake> *graf;
public:
    Cvor(UsmjereniGraf<TipOznake> *graf, int redniBroj){
        this->redniBroj=redniBroj;
        this->graf=graf;
    }
    int dajRedniBroj(){
        return redniBroj;
    }
    TipOznake dajOznaku(){
        return graf->dajOznakuCvora(redniBroj);
    }
    void postaviOznaku(TipOznake oznaka){
        graf->postaviOznakuCvora(redniBroj,oznaka);
    }
};



template <typename TipOznake>
class UsmjereniGraf{
public:
    UsmjereniGraf() {};
    UsmjereniGraf(int) {};
    virtual ~UsmjereniGraf() {};
    virtual int dajBrojCvorova() const=0;
    virtual void postaviBrojCvorova(int)=0;
    virtual void dodajGranu(int polazniCvor, int dolazniCvor, float tezina=0)=0;
    virtual void obrisiGranu(int polazniCvor, int dolazniCvor)=0;
    virtual void postaviTezinuGrane(int polazniCvor, int dolazniCvor, float tezina=0)=0;
    virtual float dajTezinuGrane(int polazniCvor, int dolazniCvor) const=0;
    virtual bool postojiGrana(int polazniCvor, int dolazniCvor) const=0;
    virtual void postaviOznakuCvora(int cvor, TipOznake oznaka)=0;
    virtual TipOznake dajOznakuCvora(int cvor) const=0;
    virtual void postaviOznakuGrane(int polazniCvor, int dolazniCvor, TipOznake oznaka)=0;
    virtual TipOznake dajOznakuGrane(int polazniCvor, int dolazniCvor) const=0;
    virtual Grana<TipOznake> dajGranu(int polazniCvor, int dolazniCvor)=0;
    virtual Cvor<TipOznake> dajCvor(int cvor)=0;
    GranaIterator<TipOznake> dajGranePocetak(){
        return ++GranaIterator<TipOznake>(this,0,-1);
    }
    GranaIterator<TipOznake> dajGraneKraj(){
        return GranaIterator<TipOznake> (this, dajBrojCvorova(),0);
    }
};

template <typename TipOznake>
class MatricaGraf:public UsmjereniGraf<TipOznake>{
    std::vector<std::vector<float>> m;                      //Matrica susjedstva
    std::vector<TipOznake> oznakeCvorova;                   //Niz oznaka za cvorove
    std::vector<std::vector<TipOznake>> oznakeGrana;        //Niz oznaka za grane
public:
    MatricaGraf(int n){
        if(n<0){
            throw std::logic_error("Broj cvorova ne moze biti negativan!");
        }
        //Postavljamo negativnu vrijednost koja nam govori da grana ne postoji
        m=std::vector<std::vector<float>>(n, std::vector<float>(n,-1));         
        oznakeCvorova=std::vector<TipOznake>(n);
        oznakeGrana=std::vector<std::vector<TipOznake>>(n,std::vector<TipOznake>(n));
    }
    
    int dajBrojCvorova() const override{
        return m.size();
    }
    
    void postaviBrojCvorova(int n){
        if(n<m.size()){
            throw std::range_error("Neispravan broj cvorova!");
        }
        m.resize(n);
        oznakeGrana.resize(n);
        for(int i=0;i<n;i++){
            m.at(i).resize(n,-1);
            oznakeGrana.resize(n);
        }
        oznakeCvorova.resize(n);
    }
    
    void dodajGranu(int polazniCvor, int dolazniCvor, float tezina=0){
        if(polazniCvor<0 || polazniCvor>=m.size() || dolazniCvor<0 || dolazniCvor>=m.size()){
            throw std::range_error("Neispravni parametri!");
        }
        m[polazniCvor][dolazniCvor]=tezina;
    }
    
    void obrisiGranu(int polazniCvor, int dolazniCvor){
        if(!postojiGrana(polazniCvor,dolazniCvor)){
            throw std::logic_error("Grana ne postoji!");
        }
        m[polazniCvor][dolazniCvor]=-1;
    }
    
    void postaviTezinuGrane(int polazniCvor, int dolazniCvor, float tezina=0){
        if(!postojiGrana(polazniCvor,dolazniCvor)){
            throw std::logic_error("Grana ne postoji!");
        }
        m[polazniCvor][dolazniCvor]=tezina;
    }
    
    float dajTezinuGrane(int polazniCvor, int dolazniCvor) const override{
        if(!postojiGrana(polazniCvor,dolazniCvor)){
            throw std::logic_error("Grana ne postoji!");
        }
        return m[polazniCvor][dolazniCvor];
    }
    
    bool postojiGrana(int polazniCvor, int dolazniCvor) const override{
        if(polazniCvor<0 || polazniCvor>=m.size() || dolazniCvor<0 || dolazniCvor>=m.size()){
            throw std::logic_error("Neispravan cvor!");
        }
        return std::fabs(m[polazniCvor][dolazniCvor]+1)>0.0000001;                             
    }
    
    void postaviOznakuCvora(int cvor, TipOznake oznaka){
        if(cvor<0 || cvor>=m.size()){
            throw std::logic_error("Neispravan cvor!");
        }
        oznakeCvorova[cvor]=oznaka;
    }
    
    TipOznake dajOznakuCvora(int cvor) const override{
        if(cvor<0 || cvor>=m.size()){
            throw std::logic_error("Neispravan cvor!");
        }
        return oznakeCvorova[cvor];
    }
    
    void postaviOznakuGrane(int polazniCvor, int dolazniCvor, TipOznake oznaka){
        if(!postojiGrana(polazniCvor,dolazniCvor)){
            throw std::logic_error("Grana ne postoji!");
        }
        oznakeGrana[polazniCvor][dolazniCvor]=oznaka;
    }
    
    TipOznake dajOznakuGrane(int polazniCvor, int dolazniCvor) const override{
        if(!postojiGrana(polazniCvor,dolazniCvor)){
            throw std::logic_error("Grana ne postoji!");
        }
        return oznakeGrana[polazniCvor][dolazniCvor];
    }
    
    Grana<TipOznake> dajGranu(int polazniCvor, int dolazniCvor){
        if(!postojiGrana(polazniCvor,dolazniCvor)){
            throw std::logic_error("Grana ne postoji!");
        }
        return Grana<TipOznake>(this,polazniCvor,dolazniCvor);
    }
    
    Cvor<TipOznake> dajCvor(int cvor){
        if(cvor<0 || cvor>=m.size()){
            throw std::logic_error("Neispravan cvor!");
        }
        return Cvor<TipOznake>(this,cvor);
    }
};




template <typename TipOznake>

struct ListaCvor{
    int redniBroj;
    TipOznake oznaka;
    float tezina;
};

template<typename TipOznake>

class ListaGraf:public UsmjereniGraf<TipOznake>{
    std::vector<std::list<ListaCvor<TipOznake>>> l;
    std::vector<TipOznake> oznakeCvorova; 
public:

    ListaGraf(int n){
        if(n<0){
            throw std::logic_error("Broj cvorova ne moze biti negativan!");
        }
        l=std::vector<std::list<ListaCvor<TipOznake>>>(n,std::list<ListaCvor<TipOznake>>());
        oznakeCvorova=std::vector<TipOznake> (n);
    }
    
    int dajBrojCvorova() const override{
        return l.size();
    }
    
    void postaviBrojCvorova(int n){
        if(n<l.size()){
            throw std::logic_error("Broj cvorova ne moze biti negativan!");
        }
        l.resize(n);
        oznakeCvorova.resize(n);
    }
    
    void dodajGranu(int polazniCvor, int dolazniCvor, float tezina=0){
        if(polazniCvor<0 || polazniCvor>=l.size() || dolazniCvor<0 || dolazniCvor>=l.size()){
            throw std::range_error("Neispravni parametri!");
        }
        auto it=l[polazniCvor].begin();
        while(it!=l[polazniCvor].end() && it->redniBroj<dolazniCvor){
            it++;
        }
        ListaCvor<TipOznake> novi;
        novi.redniBroj=dolazniCvor;
        novi.tezina=tezina;
        novi.oznaka=TipOznake();
        l[polazniCvor].insert(it,novi);
    }
    
    void obrisiGranu(int polazniCvor, int dolazniCvor){
        if(!postojiGrana(polazniCvor,dolazniCvor)){
            throw std::logic_error("Grana ne postoji!");
        }
        
        auto it=l[polazniCvor].begin();
        while(it!=l[polazniCvor].end() && dolazniCvor!=it->redniBroj){
            it++;
        }
        
        l[polazniCvor].erase(it);
    }
    
    bool postojiGrana(int polazniCvor, int dolazniCvor) const override{
        if(polazniCvor<0 || polazniCvor>=l.size() || dolazniCvor<0 || dolazniCvor>=l.size()){
            throw std::logic_error("Neispravan cvor!");
        }
        
        auto it=l[polazniCvor].begin();
        while(it!=l[polazniCvor].end()){
            if(dolazniCvor==it->redniBroj) return true;
            else if(dolazniCvor<it->redniBroj) return false;
            it++;
        }
        return false;
    }
    
    void postaviTezinuGrane(int polazniCvor, int dolazniCvor, float tezina=0){
        if(!postojiGrana(polazniCvor,dolazniCvor)){
            throw std::logic_error("Grana ne postoji!");
        }
        auto it=l[polazniCvor].begin();
        while(it!=l[polazniCvor].end() && it->redniBroj!=dolazniCvor){
            it++;
        }
        it->tezina=tezina;
    }
    
    float dajTezinuGrane(int polazniCvor, int dolazniCvor) const override{
        if(!postojiGrana(polazniCvor,dolazniCvor)){
            throw std::logic_error("Grana ne postoji!");
        }
        auto it=l[polazniCvor].begin();
        while(it!=l[polazniCvor].end() && it->redniBroj!=dolazniCvor){
            it++;
        }
        return it->tezina;
    }
    
    void postaviOznakuCvora(int cvor, TipOznake oznaka){
        if(cvor<0 || cvor>=l.size()){
            throw std::logic_error("Neispravan cvor!");
        }
        oznakeCvorova[cvor]=oznaka;
    }
    
    TipOznake dajOznakuCvora(int cvor) const override{
        if(cvor<0 || cvor>=l.size()){
            throw std::logic_error("Neispravan cvor!");
        }
        return oznakeCvorova[cvor];
    }
    
    TipOznake dajOznakuGrane(int polazniCvor,int dolazniCvor) const override{
        if(!postojiGrana(polazniCvor,dolazniCvor)){
            throw std::logic_error("Grana ne postoji!");
        }
        auto it=l[polazniCvor].begin();
        while(it!=l[polazniCvor].end() && it->redniBroj!=dolazniCvor){
            it++;
        }
        return it->oznaka;
    }
    
    void postaviOznakuGrane(int polazniCvor, int dolazniCvor, TipOznake oznaka){
        if(!postojiGrana(polazniCvor,dolazniCvor)){
            throw std::logic_error("Grana ne postoji!");
        }
        
        auto it=l[polazniCvor].begin();
        while(it!=l[polazniCvor].end() && it->redniBroj!=dolazniCvor){
            it++;
        }
        it->oznaka=oznaka;
    }
    
    Grana<TipOznake> dajGranu(int polazniCvor, int dolazniCvor){
        if(!postojiGrana(polazniCvor,dolazniCvor)){
            throw std::logic_error("Grana ne postoji!");
        }
        return Grana<TipOznake>(this,polazniCvor,dolazniCvor);
    }
    
    Cvor<TipOznake> dajCvor(int cvor){
        if(cvor<0 || cvor>=l.size()){
            throw std::logic_error("Neispravan cvor!");
        }
        return Cvor<TipOznake>(this,cvor);
    }
    
    
    
    
    
};



//Breadth-First-Search-BFS

template <typename TipOznake>

void bfs(UsmjereniGraf<TipOznake> *graf,std::vector<Cvor<TipOznake>> &bfs_obilazak, Cvor<TipOznake> cvor){
    std::queue<Cvor<TipOznake>> red;
    red.push(cvor);
    bfs_obilazak.push_back(cvor);
    cvor.postaviOznaku(1);          //1 ako je posjecen
    while(!red.empty()){
        Cvor<TipOznake> c=red.front();
        red.pop();
        for(GranaIterator<TipOznake> it=graf->dajGranePocetak();it!=graf->dajGraneKraj();it++){
            if((*it).dajDolazniCvor().dajOznaku()!=1 && (*it).dajPolazniCvor().dajRedniBroj()==c.dajRedniBroj()){
                (*it).dajDolazniCvor().postaviOznaku(1);
                red.push((*it).dajDolazniCvor());
                bfs_obilazak.push_back((*it).dajDolazniCvor());
            }
        }
    }
}

//Depth-First-Search-DFS
template <typename TipOznake>

void dfs(UsmjereniGraf<TipOznake> *graf,std::vector<Cvor<TipOznake>> &dfs_obilazak, Cvor<TipOznake> cvor){
    cvor.postaviOznaku(1);
    dfs_obilazak.push_back(cvor);
    for(GranaIterator<TipOznake> it=graf->dajGranePocetak();it!=graf->dajGraneKraj();it++){
        if((*it).dajDolazniCvor().dajOznaku()!=1 && (*it).dajPolazniCvor().dajRedniBroj()==cvor.dajRedniBroj()){
            dfs(graf,dfs_obilazak,(*it).dajDolazniCvor());
        }
    }
}





int main() {
    UsmjereniGraf<bool> *g = new ListaGraf<bool>(6);
  g->dodajGranu(0, 1, 2.5);
  g->dodajGranu(1, 2, 1.2);
  g->dodajGranu(1, 3, 0.1);
  g->dodajGranu(3, 3, -4.0);
  g->dodajGranu(2, 4, 3.14);
  g->dodajGranu(2, 3, 2.73);
  g->dodajGranu(3, 5, 1);
  g->dodajGranu(5, 2, -5.0);
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 6; j++)
      std::cout << g->postojiGrana(i, j);
  delete g;
    return 0;
}
