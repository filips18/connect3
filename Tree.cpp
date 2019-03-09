#include <iostream>
using namespace std;

class Node {
        int _info;
        Node** _a;
        int _kap, _pop = 0;
        Node* _otac;

        void kopiraj(const Node& N) {
                _info = N._info;
                _kap = N._kap;
                _pop = N._pop;
                _otac = N._otac;
                _a = new Node*[_kap];
                for (int i = 0; i < _pop; _a[i] = N._a[i++]);
        }


        void premesti(Node& N) {
                _info = N._info;
                _kap = N._kap;
                _pop = N._pop;
                _otac = N._otac;
                _a = N._a;
                N._a = nullptr;
        }

        void obrisi() {
                _otac = nullptr;
                _info = _kap = _pop = 0;
                delete[] _a;
                _a = nullptr;
        }

public:
        Node(int m,int info, Node* otac): _info(info) {                                                                 //Podrazumevajuci konstruktor
                _kap = m;
                _otac = otac;
                _a = new Node* [m];
                for (int i = 0; i < m; _a[i++] = nullptr);
        }

        Node(const Node& N) {                                                                                                                                   //Kopirajuci
                kopiraj(N);
        }

        Node(Node&& N) {                                                                                                                                                //Premestajuci
                premesti(N);
        }

        ~Node() {                                                                                                                                                               //Destruktor
                obrisi();
        }

        Node& operator= (const Node& N) {                                                                                                       //Kopirajuca dodela vrednosti
                if (this != &N) { obrisi(); kopiraj(N); }
                return *this;
        }

        Node& operator= (Node&& N) {                                                                                                    //Premestajuca
                if (this != &N) { obrisi(); premesti(N); }
                return *this;
        }

        Node** dohvatiA() const { return _a; }

        int dohvatikap() const { return _kap; }

        int dohvatipop() const { return _pop; }

        int dohvatiinfo() const { return _info;  }

        Node* dohvatioca() const { return _otac; }

        void incpop() { _pop++; }

        Node* odredibrata() {
                int i = 0;
                while (this != (*_otac)[i]) i++;
                if ((i + 1) == _kap) return nullptr;
                return (*_otac)[i + 1];
        }

        Node*& operator[] (int i) {
                return _a[i];
        }

};

class Stablo {
        Node* _root;
        int _m;

        void brisi();

public:
        Stablo () {
                int info, m;
                cout << "Unesite podatak korena i stepen stabla: " << endl;
                cin >> info >> m;
                _m = m;
                _root = new Node(m,info, nullptr);
        }

        Stablo(const Stablo&) = delete;

        Stablo& operator=(const Stablo&) = delete;

        ~Stablo() {
                brisi();
        }

        Node& odredimesto(Node& N) const {                                                                                                              //Odredjuje prvi cvor koji ima slobodno mesto na koje se moze dodati novi cvor
                Node* mesto = &N;                                                                                                                                       // a da pritom stablo ostane kompletno/skoro kompletno
                Node* pom = mesto, *brt;
                while (mesto->dohvatipop() == mesto->dohvatikap()) {
                        if (mesto->dohvatioca() == nullptr) brt = nullptr;
                        else {
                                brt = mesto->odredibrata();
                        }
                        if (brt != nullptr) mesto = brt;
                        else {
                                pom = (*pom)[0];
                                mesto = pom;
                        }
                }
                return *mesto;
        }


        void dodaj();

        friend ostream& operator<< (ostream& it, const Stablo& S);

        int brcvorova() const;

        int odredivisinu() const {
                int br = 0;
                Node* pom = (*_root)[0];
                while (pom != nullptr) {
                        br++;
                        pom = (*pom)[0];
                }
                return br;
        }
};

ostream& operator<<(ostream& it, const Stablo& S) {
         Node* pom, *q, *brt;
         int a = S.odredivisinu() + 1;
         Node** stek = new Node*[a];
         for (int i = 0; i < a; stek[i++] = nullptr);
         int i = 0;
         stek[i] = S._root;
         while (stek[0] != nullptr) {
                 pom = stek[i];
                 stek[i--] = nullptr;
                 while (pom != nullptr) {
                         q = (*pom)[0];
                         if (pom->dohvatioca() == nullptr) brt = nullptr;
                         else {
                                 brt = pom->odredibrata();
                         }
                         if (brt != nullptr) {
                                 stek[++i] = brt;
                         }
                         it << pom->dohvatiinfo();
                         it << " ";
                         pom = q;
                 }
         }
         it << endl;
         delete[] stek;
         return it;
}

int Stablo::brcvorova() const {
        int br = 0;
        Node* pom, *q, *brt;
        int a = odredivisinu() + 1;
        Node** stek = new Node*[a];
        for (int i = 0; i < a; stek[i++] = nullptr);
        int i = 0;
        stek[i] = _root;
        while (stek[0] != nullptr) {
                pom = stek[i];
                stek[i--] = nullptr;
                while (pom != nullptr) {
                        q = (*pom)[0];
                        br++;
                        if (pom->dohvatioca() == nullptr) brt = nullptr;
                        else {
                                brt = pom->odredibrata();
                        }
                        if (brt != nullptr) {
                                stek[++i] = brt;
                        }
                        pom = q;
                }
        }
        return br;
}
void Stablo::dodaj() {                                                                                                                                                          //Dodavanje cvora u stablo
        int info;
        cout << "Upisite vrednost koju zelite da dodate u stablo: " << endl;
        cin >> info;
        Node* otac = &odredimesto(*_root);
        Node* p = new Node(_m, info, otac);
        if (otac->dohvatipop() == 0) {
                (*otac)[0] = p;
        }
        else {
                (*otac)[otac->dohvatipop()] = p;
        }
        otac->incpop();
}

void Stablo::brisi() { 
        Node* pom;
        bool flg = false;
        Node** stek = new Node*[brcvorova()];
        for (int i = 0; i < brcvorova(); stek[i++] = nullptr);
        int i = 0;
        int b;
        stek[i] = _root;
        pom = stek[i--];
        while (pom != nullptr) {			 //puni stek svim cvorovima pocevsi od korena iduci od najdesnijeg sina ka najlevljem
                if ((*pom)[0] == nullptr) {
                        flg = true;
                        b = 0;
                        if (pom->dohvatioca() != nullptr) {
                                while (pom != (*(pom->dohvatioca()))[b]) b++;
                                (*pom)[b] = nullptr;
                        }
                        delete pom;
                }
                if (!flg) {
                        if ((*(*pom)[0])[0] == nullptr) {
                                if (pom->dohvatioca() != nullptr) {
                                        b = 0;
                                        while (pom != (*(pom->dohvatioca()))[b]) b++;
                                        (*(pom->dohvatioca()))[b] = nullptr;
                                }
                                for (int j = 0; j < pom->dohvatipop(); j++) {
                                        delete ((*pom)[j]);
                                        (*pom)[j] = nullptr;
                                }
                                delete pom;
                        }
                        else {
                                for (int j = pom->dohvatipop() - 1; j >= 0; j--) stek[++i] = (*pom)[j];
                        }
                }
                if (i < 0) break;
                pom = stek[i--];
                flg = false;
        }
        _root = nullptr;
        delete[] stek;
        stek = nullptr;
}

int main() {
        char a;
        Stablo* K = nullptr;
        bool flg = false;
        while (!flg) {
                cout << "Izaberite zeljenu radnju:" << endl;
                cout << "A) Kreiranje stabla" << endl;
                cout << "B) Dodavanje elementa" << endl;
                cout << "C) Ispis" << endl;
                cout << "D) Prebrojavanje cvorova" << endl;
                cout << "E) Izlaz" << endl;
                cin >> a;
                switch (a) {
                        case 'A': case 'a': {
                                if (K != nullptr) {
                                        cout << "Stablo je vec napravljeno! " << endl;
                                        break;
                                }
                                K = new Stablo;
                                break;
                        }
                        case 'B': case 'b': { 
                                if (K == nullptr) {
                                        cout << "Morate prvo kreirati stablo! " << endl;
                                        break;
                                }
                                K->dodaj();
                                break;
                        }
                        case 'C': case 'c': {
                                if (K == nullptr) {
                                        cout << "Morate prvo kreirati stablo! " << endl;
                                        break;
                                }
                                cout << *K;
                                break;
                        }
                        case 'D': case 'd': {
                                if (K == nullptr) {
                                        cout << "Morate prvo kreirati stablo! " << endl;
                                        break;
                                }
                                cout << K->brcvorova();
                                cout << endl;
                                break;
                        }
                        case 'E': case 'e': {
                                flg = true;
                                break;
                        }
                        default: {
                                cout << "Izabrali ste nepostojecu opciju, pokusajte ponovo. " << endl;
                                break;
                        }
                }
        }
        delete K;
        K = nullptr;
        return 0;


}
