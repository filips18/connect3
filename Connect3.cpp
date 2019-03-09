#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
enum Boja {Nista,Crvena,Plava};
class Node {
	Boja _matrix[5][5] = {
		{ Nista,Nista,Nista,Nista,Nista},
		{ Nista,Nista,Nista,Nista,Nista },
		{ Nista,Nista,Nista,Nista,Nista },
		{ Nista,Nista,Nista,Nista,Nista },
		{ Nista,Nista,Nista,Nista,Nista },
	};
	Node* _a[5] = { nullptr,nullptr,nullptr,nullptr,nullptr };
	int _pop = 0, _win, _igrao;
	bool _odlucenpob = false;
	Node* _otac;

	void koporprem(const Node& N) {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; i < 5; i++) {
				_matrix[i][j] = N._matrix[i][j];
			}
		}
		_pop = N._pop;
		_otac = N._otac;
		for (int i = 0; i < 5; _a[i] = N._a[i++]);
	}


	void obrisi() {
		_otac = nullptr;
		_pop = 0;
	}

public:
	Node(Node* otac) : _otac(otac) {}                                                                //Podrazumevajuci konstruktor

	Node(const Node& N) {                                                                                                                                   //Kopirajuci
		koporprem(N);
	}

	Node(Node&& N) {                                                                                                                                                //Premestajuci
		koporprem(N);
	}

	~Node() {                                                                                                                                                               //Destruktor
		obrisi();
	}

	Node& operator= (const Node& N) {                                                                                                       //Kopirajuca dodela vrednosti
		if (this != &N) { obrisi(); koporprem(N); }
		return *this;
	}

	Node& operator= (Node&& N) {                                                                                                    //Premestajuca
		if (this != &N) { obrisi(); koporprem(N); }
		return *this;
	}

	int dohvatipop() const { return _pop; }

	bool& dohvatiodlucen() { return _odlucenpob; }

	int& dohvatiigrao() { return _igrao; }

	Boja& dohvatielem(int i, int j) {
		return _matrix[i][j];
	}

	int& dohvatiwin() { return _win; }

	Node* dohvatioca() const { return _otac; }

	void incpop() { _pop++; }

	Node* odredibrata() {
		int i = 0;
		while (this != (*_otac)[i]) i++;
		i++;
		while ((*_otac)[i] == nullptr && i < 5) i++;
		if (i == 5) return nullptr;
		return (*_otac)[i];
	}

	Node*& operator[] (int i) {
		return _a[i];
	}

	void postavimatricu(const Node& N) {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				_matrix[i][j] = N._matrix[i][j];
			}
		}
	}

	int koigra() {
		int p1 = 0, p2 = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (dohvatielem(i, j) == 1) p1++;
				else if (dohvatielem(i, j) == 2) p2++;
			}
		}
		if (p1 > p2 ) return 2;
		else if (p2 > p1) return 1;
		else if (_otac == nullptr) return random();
		else if (_otac->dohvatiigrao() == 1) return 2;
		else if (_otac->dohvatiigrao() == 2) return 1;
	}
	
	int random() {
		srand(time(NULL));
		int r = rand() % 2;
		if (r == 0) return 2;
		else return 1;
	}

	Node* prvisin() {
		int i = 0;
		while ((*this)[i] == nullptr && i < 5) i++;
		return (*this)[i];
	}

	int odrediwin() const {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 3; j++) {
				if (i < 3) {																		
					if ((_matrix[i][j] == 1) && (_matrix[i][j + 1] == 1) && (_matrix[i][j + 2] == 1)) return 1;
					else if ((_matrix[i][j] == 2) && (_matrix[i][j + 1] == 2) && (_matrix[i][j + 2] == 2)) return -1;
					else if ((_matrix[i][j] == 1) && (_matrix[i + 1][j + 1] == 1) && (_matrix[i + 2][j + 2] == 1)) return 1;
					else if ((_matrix[i][j] == 2) && (_matrix[i + 1][j + 1] == 2) && (_matrix[i + 2][j + 2] == 2)) return -1;
					else if ((_matrix[i][j] == 1) && (_matrix[i + 1][j] == 1) && (_matrix[i + 2][j] == 1)) return 1;
					else if ((_matrix[i][j] == 2) && (_matrix[i + 1][j] == 2) && (_matrix[i + 2][j] == 2)) return -1;
				}

				if (i >= 2) {
					if ((_matrix[i][j] == 1) && (_matrix[i - 1][j + 1] == 1) && (_matrix[i - 2][j + 2] == 1)) return 1;
					else if ((_matrix[i][j] == 2) && (_matrix[i - 1][j + 1] == 2) && (_matrix[i - 2][j + 2] == 2)) return -1;
				}

			}
		}
		return 0;
	}

};

class Stablo {
	Node* _root;

	void brisi();

public:
	Stablo() {
		_root = new Node(nullptr);
		int b;
		cout << "Unesite pocetno stanje matrice. " << endl;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				cin >> b;
				_root->dohvatielem(i, j) = (Boja)b;
			}
		}
	}

	Stablo(const Stablo&) = delete;

	Stablo(Stablo&&) = delete;

	~Stablo() {
		brisi();
	}
	void generisi();

	friend ostream& operator<< (ostream& it, const Stablo& S);

	int brcvorova() const;

	void azuriraj();
};

ostream& operator<<(ostream& it, const Stablo& S) {
	Node* pom, *q, *brt;
	int a = 25;
	Node** stek = new Node*[a];
	int i = 0 , br = 0;
	stek[i] = S._root;
	while (stek[0] != nullptr) {
		pom = stek[i];
		stek[i--] = nullptr;
		while (pom != nullptr) {
			if (pom->dohvatipop() != 0) q = pom->prvisin();
			else q = nullptr;
			br++;
			if (pom->dohvatioca() == nullptr) brt = nullptr;
			else {
				brt = pom->odredibrata();
			}
			if (brt != nullptr) {
				stek[++i] = brt;
			}
			it << "	Matrica cvora broj " << br << " :" << endl;
			for (int x = 0; x < 5; x++) {
				it << "		(" << (x + 1) << ") ";
				for (int y = 0; y < 5; y++) { 
					it << " " << (int)pom->dohvatielem(x, y);
				}
				it << endl;

			}
			it << "U prednosti je: ";
			if (pom->dohvatiwin() == 1) {
				it << "crveni igrac." << endl;
			}
			else if (pom->dohvatiwin() == -1) {
				it << "plavi igrac." << endl;
			}
			else {
				it << "niko." << endl;
			}
			it << pom->dohvatiigrao();
			pom = q;
			it << endl;
		}
	}
	it << endl;
	delete[] stek;
	return it;
}

int Stablo::brcvorova() const {
	int br = 0;
	Node* pom, *q, *brt;
	int a = 25;
	Node** stek = new Node*[a];
	for (int i = 0; i < a; stek[i++] = nullptr);
	int i = 0;
	stek[i] = _root;
	while (stek[0] != nullptr) {
		pom = stek[i];
		stek[i--] = nullptr;
		while (pom != nullptr) {
			if (pom->dohvatipop() != 0) q = pom->prvisin();
			else q = nullptr;
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
	delete[] stek;
	return br;
}
void Stablo::generisi() {						//Generisanje stabla igre
	Node* otac ,*brt;
	Node* q;
	Node* p;
	Node* stek[25];
	bool flg = false;
	for (int i = 0; i < 25; stek[i++] = nullptr);
	int igrac, x, index = 0, pomwin;
	bool v0 = false, v1 = false, v2 = false, v3 = false, v4 = false;
	stek[index] = _root;
	while (stek[0] != nullptr) {
		otac = stek[index];
		stek[index--] = nullptr;
		while (otac != nullptr) {
			if (flg == true) {
				stek[++index] = otac->odredibrata();
				flg = false;
			}
			pomwin = otac->odrediwin();
			if (pomwin == 0) {
				for (int j = 0; j < 5; j++) {
					igrac = otac->koigra();
					if (otac->dohvatielem(4, j) == 0) {
						v4 = true;
					}
					else if (otac->dohvatielem(3, j) == 0) {
						v3 = true;
					}
					else if (otac->dohvatielem(2, j) == 0) {
						v2 = true;
					}
					else if (otac->dohvatielem(1, j) == 0) {
						v1 = true;
					}
					else if (otac->dohvatielem(0, j) == 0) {
						v0 = true;
					}
					if (v4 || v3 || v2 || v1 || v0) {
						p = new Node(otac);
						p->postavimatricu(*otac);
						if (v4) p->dohvatielem(4, j) = (Boja)igrac;
						else if (v3) p->dohvatielem(3, j) = (Boja)igrac;
						else if (v2) p->dohvatielem(2, j) = (Boja)igrac;
						else if (v1) p->dohvatielem(1, j) = (Boja)igrac;
						else if (v0) p->dohvatielem(0, j) = (Boja)igrac;
						p->dohvatiigrao() = igrac;
						(*otac)[j] = p;
						otac->incpop();
					}
					v0 = false;
					v1 = false;
					v2 = false;
					v3 = false;
					v4 = false;
				}
				x = 0;
				while ((*otac)[x] == nullptr && x < 5) x++;
				if (x == 5) {
					q = nullptr;
				}
				else  q = (*otac)[x];
				if (q != nullptr) brt = q->odredibrata();
				else brt = nullptr;
				if (brt != nullptr) stek[++index] = brt;
				otac = q;
			}
			else {

				igrac = otac->koigra();
				otac->dohvatiwin() = pomwin;
				otac->dohvatiigrao() = igrac;
				otac->dohvatiodlucen() = true;
				flg = true;
				otac = nullptr;
			}
		}
	}
}
	

void Stablo::brisi() { 
	Node* pom;
	Node** stek = new Node*[brcvorova()];
	int index = 0;
	stek[index] = _root;
	while (stek[0] != nullptr) {
		pom = stek[index];
		stek[index--] = nullptr;
		if (pom->dohvatipop() != 0) {
			for (int i = 0; i < 5; i++) {
				if ((*pom)[i] != nullptr) {
					stek[++index] = (*pom)[i];
				}
			}
		}
		delete pom;
	}
	_root = nullptr;
	delete[] stek;
	stek = nullptr;
}

void Stablo::azuriraj() {
	Node** stek = new Node*[brcvorova()];
	Node *pom, *brt;
	int i = 0;
	stek[i] = _root;
	while (stek[0] != nullptr) {
		pom = stek[i];
		stek[i--] = nullptr;
		for (int x = 4; x >= 0; x--) {
			if ((*pom)[x] != nullptr) {
				if (!((*pom)[x])->dohvatiodlucen()) {
					stek[++i] = (*pom)[x];
				}
				else {
					if (!(pom->dohvatiodlucen())) {
						pom->dohvatiwin() = ((*pom)[x])->dohvatiwin();
						pom->dohvatiodlucen() = true;
					}
					else if (pom->dohvatiigrao() == 1 && ((*pom)[x])->dohvatiwin() > pom->dohvatiwin()) {
						pom->dohvatiwin() = ((*pom)[x])->dohvatiwin();
					}
					else if (pom->dohvatiigrao() == 2 && ((*pom)[x])->dohvatiwin() < pom->dohvatiwin()) {
						pom->dohvatiwin() = ((*pom)[x])->dohvatiwin();
					}
				}
			}
		}
	}
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
		cout << "B) Generisanje stabla" << endl;
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
			K->generisi();
			K->azuriraj();
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