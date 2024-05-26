#include <iostream>

#include <fstream>

#include <vector>

#include <string>

#include <cstring>

#include <exception>

#include <set>

#include <algorithm>

#include <queue>

#include <utility>

#include <memory>

#include <stack>
using namespace std;
class caretaker_memento;
class memento_angajat;
class Data_Invalida : public exception
{
    string motiv;
public:
    Data_Invalida();
    Data_Invalida(string m) : motiv(m) {}
    const char* what() const throw()
    {
        return motiv.c_str();
    }
};
class ID_invalid : public exception
{
    int valoare_start;
public:
    ID_invalid(int val) : valoare_start(val) {}
    ID_invalid() {}
    int get_val()
    {
        return valoare_start;
    }
    const char* what() const throw()
    {
        return "ID-urile au fost realocate automat incepand de la valoarea indicata\n";
    }
};
class data
{
    int zi;
    int luna;
    int an;
public:
    data(int day, int month, int year): zi(day), luna(month), an(year) {}
    data(): zi(5), luna(11), an(2024) {}
    friend ostream & operator << (ostream & out,
                                  const data & d);
    friend istream & operator >> (istream & in, data & a);
    friend int comparare(data data_1, data data_2);
    friend void verificare_data(data* date);
    bool operator< (const data & d) const{
        if(comparare(*this, d) == 1)
            return 1;
        else
            return 0;
    }
    bool operator> (const data & d) const{
        if(comparare(*this, d) == -1)
            return 1;
        else
            return 0;
    }
};
class task{
    data deadline;
    string eticheta;
public:
    friend class iterator_task;
    task();
    task(data deadline, string eticheta) : deadline(deadline), eticheta(eticheta){}
    data get_deadline(){return deadline;}
    string get_eticheta(){return eticheta;}
};
typedef pair<data, string> taskddl;
class angajat
{
protected:
    string nume;
    string prenume;
    int ID;
    int ID_departament;
    static int contor_ID;
    data data_nasterii;
    void afisare_angajat();
public:
    angajat(string surname, string name, int dep, data data_n): nume(surname), prenume(name), ID_departament(dep), data_nasterii(data_n)
    {
        contor_ID ++;
        ID = contor_ID;
    }
    angajat(string surname, string name, data data_n): nume(surname), prenume(name), ID_departament(0), data_nasterii(data_n)
    {
        contor_ID ++;
        ID = contor_ID;
    }
    angajat(): nume("place_holder"), prenume("place_holder"), data_nasterii(data()) {}
    ~angajat()
    {
        ID = 0, ID_departament = 0;
    }

    int getdep()
    {
        return ID_departament;
    }
    string getname()
    {
        return nume + " " + prenume;
    }
    virtual string get_venit() {}
    int get_ID()
    {
        return ID;
    }
    virtual string caracterizare() {}
    data get_data()
    {
        return data_nasterii;
    }
    void set_departmentID(int depID)
    {
        ID_departament = depID;
    }
    void afisare_nume();
    void set_id(int nr)
    {
        ID = nr;
    }
    static void initializare_ID(int start)
    {
        contor_ID = start;
    }
    friend void ordonare_alfabetica(vector < angajat* > angajati);
    static void realocare_ID_uri(vector<angajat*> ang, int nr)
    {
        for(int i = 0; i < ang.size(); i++)
        {
            ang[i]->ID = nr + i + 1;
        }
        angajat::contor_ID = nr + ang.size();
    }
};
class angajat_permanent : public angajat
{
    int salariu;
    data data_angajarii;
    caretaker_memento* snapshoturi;
public:
        class memento_angajat{
private:
        const int salariu;
        const int dep_id;
        const int ID;
    public:
        friend class angajat_permanent;
        memento_angajat(int sal, int dep_ID, int ID) : salariu(sal), dep_id(dep_ID), ID(ID){}
    };
    angajat_permanent(string surname, string name, int dep, data data_n, int sal, data data_a): angajat(surname, name, dep, data_n), salariu(sal), data_angajarii(data_a) {}
    angajat_permanent() {}
    friend ostream & operator << (ostream & out,
                                  const angajat_permanent & a);
    friend istream & operator >> (istream & in, angajat_permanent & a);
    int getsalariu()
    {
        return salariu;
    }
    string get_venit()
    {
        return  to_string(salariu) + " lei";
    }
    void set_salariu(int sal)
    {
        salariu = sal;
    }
    string caracterizare()
    {
        return "Angajat Permanent";
    }
    memento_angajat* creeaza_snapshot(){return new memento_angajat(salariu,ID_departament, ID);}
    void restaureaza(memento_angajat* mem){
        salariu = mem->salariu;
        ID = mem->ID;
        ID_departament = mem->dep_id;
    }
    caretaker_memento* get_caretaker(){return snapshoturi;}
    void add_caretaker(caretaker_memento* crt){snapshoturi = crt;}
};
class contractor : public virtual angajat
{
protected:
    int comision;
    int durata_contract;
    shared_ptr<task> tsk;
public:
    contractor() {}
    contractor(string surname, string name, int dep, data data_n, int com, int durata) : angajat(surname, name, dep, data_n), comision(com), durata_contract(durata) {}
    friend ostream & operator << (ostream & out,
                                  const contractor & a);
    friend istream & operator >> (istream & in, contractor & a);
    string get_venit()
    {
        return "contractor pe durata de " + to_string(durata_contract) + " luni. comison: " + to_string(comision) + " lei";
    }
    string caracterizare()
    {
        return "Contractor";
    }
    void assign_task(shared_ptr<task> t){tsk = t;}
};
class manager : public virtual angajat
{
protected:
    int salariu;
    int bonus;
public:
    manager() {}
    manager(string surname, string name, int dep, data data_n, int sal, int bon) : angajat(surname, name, dep, data_n), salariu(sal), bonus(bon) {}
    friend ostream & operator << (ostream & out,
                                  const manager & a);
    friend istream & operator >> (istream & in, manager & a);
    int get_salariu()
    {
        return salariu;
    }
    string get_venit()
    {
        return to_string(salariu + bonus) + " lei";
    }
    string caracterizare()
    {
        return "Manager";
    }
};
class coordonator_proiect : public contractor, public manager
{
public:
    coordonator_proiect() {}
    coordonator_proiect(string surname, string name, int dep, data data_n, int com, int durata, int bon) : angajat(surname, name, dep, data_n), contractor(surname, name, dep, data_n, com, durata), manager(surname, name, dep, data_n, 0, bon) {}
    friend ostream & operator << (ostream & out,
                                  const coordonator_proiect & a);
    friend istream & operator >> (istream & in, coordonator_proiect & a);
    string get_venit()
    {
        return "contractor pe durata de " + to_string(durata_contract) + " luni. comison + bonus: " + to_string(comision + bonus) + " lei";
    }
    string caracterizare()
    {
        return "Coordonator_Proiect";
    }
};
class departament
{
protected:
    string nume;
    int ID;
    static int contor_ID;
    vector < angajat* > angajati;
    priority_queue<shared_ptr<task>, vector<shared_ptr<task>>, greater<shared_ptr<task>>>lista_taskuri;
    manager* manager_dep;
public:
    departament(string name): nume(name)
    {
        contor_ID ++;
        ID = contor_ID;
    }
    departament(): nume("place_holder"), ID(0) {}
    static void initializare_ID(int start)
    {
        contor_ID = start;
    }
    friend istream & operator >> (istream & in, departament & d);
    friend ostream & operator << (ostream & out,
                                  const departament & d);
    friend void afisare_departamente_salarii( vector < departament* > departamente);
    friend void afisare_angajati_departamente(vector < angajat* > angajati, vector < departament > departamente);
    friend void muta_angajat(angajat *ang, departament & dep);
    friend void sterge_angajat(angajat* ang, vector < departament* > div);
    friend void unire(vector < angajat* > & angajati, vector < departament > & departamente, int &nr_departamente, vector < departament* > & divizii);
    friend void crestere_departament(vector < angajat_permanent > & angajati, int nr_angajati, vector < departament > & dep, int nr_departamente);
    friend void scadere_departament(vector < angajat_permanent > & angajati, int nr_angajati, vector < departament > & dep, int nr_departamente);
    friend void update(angajat & a, departament & d);
    friend void afisare_taskuri(vector<departament> deps, int nrdep);
    friend class iterator_task;
    string getname()
    {
        return nume;
    }
    int get_ID()
    {
        return ID;
    }
    int genereaza_ID();
    void adauga_angajat(angajat* a)
    {
        angajati.push_back(a);
    }
    void afiseaza_angajatii();
    string caracterizare()
    {
        if(angajati.size() == 0)
        {
            return "0";
        }
        else
        {
            return angajati[0]->caracterizare();
        }
    }
    void set_manager(manager* m)
    {
        manager_dep = m;
    }
    manager* get_manager()
    {
        return manager_dep;
    }
    void set_id(int valoare){ID = valoare;}
    static void realocare_ID_uri(vector<departament*> dep, int nr)
    {
        for(int i = 0; i < dep.size(); i++)
        {
            dep[i]->ID = nr + i + 1;
        }
        departament::contor_ID = nr + dep.size();
    }
    void addtask(shared_ptr<task> tsk){lista_taskuri.push(tsk);}

};
class proiect_contractor : public departament
{
    data deadline;
    int fonduri_alocate;
public:
    proiect_contractor(string name, int zi,  int luna, int an, int fonduri) : departament(name), deadline(data(zi,luna,an)), fonduri_alocate(fonduri) {}
    proiect_contractor() {}
};
class iterator_task{
    departament* dep;
public:
    iterator_task(departament* d) : dep(d) {}
    void urmatorul(){dep->lista_taskuri.pop();}
    shared_ptr<task> element_curent(){return dep->lista_taskuri.top();}
    bool gol(){return dep->lista_taskuri.empty();}
    string eticheta_curenta(){return element_curent()->eticheta;}
    data deadline_curent(){return element_curent()->deadline;}
};
class caretaker_memento{
    stack<angajat_permanent::memento_angajat*> snapshots;
public:
    angajat_permanent::memento_angajat* recent(){return snapshots.top();}
    void sterge(){snapshots.pop();}
    void add(angajat_permanent::memento_angajat* mem){snapshots.push(mem);}
};
class firma{
    static firma* instanta;
    string nume;
    firma(string nume) : nume(nume) {}\
    firma(const firma &f) {nume = f.nume;}
public:
    static firma* instantiere(string nume){
        if (instanta == 0){
            instanta = new firma(nume);
        }
        return instanta;
    }
};
firma* firma::instanta = 0;
int angajat  :: contor_ID = 0;
int departament  :: contor_ID = 0;
int comparare(data data_1, data data_2)
{
    if (data_1.an < data_2.an)
    {
        return 1;
    }
    else
    {
        if (data_1.an == data_2.an)
        {
            if (data_1.luna < data_2.luna)
            {
                return 1;
            }
            else
            {
                if (data_1.luna == data_2.luna)
                {
                    if (data_1.zi <= data_2.zi)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return -1;

}
void verificare_data(data* date)
{
    int zi, luna, an;
    zi = date->zi;
    luna = date->luna;
    an = date->an;
    if(luna < 1 || luna > 12)
    {
        throw Data_Invalida("Luna introdusa este invalida\n");
    }
    if(zi < 1 || zi > 31 || (zi > 29 && luna == 2) || (zi > 30 && luna == 4) || (zi > 30 && luna == 6) || (zi > 30 && luna == 9) || (zi > 30 && luna == 11))
    {
        throw Data_Invalida("Ziua introdusa este invalida\n");
    }
    if((zi == 29 && luna == 2 && an % 4 != 0) || (zi == 29 && luna == 2 && an % 100 == 0 && an % 400 != 0))
    {
        throw Data_Invalida("Anul introdus nu este bisect\n");
    }
    unique_ptr<data> d1(new data(1,1,1905));
    unique_ptr<data> d2(new data(1,1,2031));
    if(comparare(*d1, *date) != 1)
    {
        throw Data_Invalida("Programul nu suporta date inainte de 1/1/1905\n");
    }
    if(*d2 < *date){
        throw Data_Invalida("Programul nu suporta date dupa 1/1/2031");
    }
}
ostream & operator << (ostream & out,
                       const data & d)
{
    out << d.zi << " " << d.luna << " " << d.an;
    return out;
}
ostream & operator << (ostream & out,
                       const angajat_permanent & a)
{
    out << a.nume << " " << a.prenume << " " << a.ID_departament << " " << a.salariu << " " << a.data_nasterii << " " << a.data_angajarii << endl;
    return out;
}
ostream & operator << (ostream & out,
                       const contractor & a)
{
    out << a.nume << " " << a.prenume << " " << a.ID_departament << " " << a.data_nasterii << " " << a.comision << " " << a.durata_contract << endl;
    return out;
}
ostream & operator << (ostream & out,
                       const manager & a)
{
    out << a.nume << " " << a.prenume << " " << a.ID_departament << " " << a.data_nasterii << " " << a.salariu << " " << a.bonus << endl;
    return out;
}
ostream & operator << (ostream & out,
                       const coordonator_proiect & a)
{
    out << a.nume << " " << a.prenume << " " << a.ID_departament << " " << a.data_nasterii << " " << a.comision << " " << a.durata_contract << " " << a.bonus << endl;
    return out;
}
ostream & operator << (ostream & out,
                       const departament & d)
{
    out << d.nume << endl;
    return out;
}
istream & operator >> (istream & in, data & a)
{
    while(true)
    {
        int zi, luna, an;
        in >> zi;
        in >> luna;
        in >> an;
        unique_ptr<data> d(new data(zi, luna, an));
        try
        {
            verificare_data(d.get());
            a = *d;
            break;
        }
        catch(Data_Invalida d)
        {
            cout << d.what() << "\n";
        }
    }
    return in;
}
istream & operator >> (istream & in, angajat_permanent & a)
{
    string nume, prenume;
    int id_departament, salariu;
    data dn, da;
    in >> nume;
    in >> prenume;
    in >> id_departament;
    in >> salariu;
    in >> dn;
    while(true)
    {
        in >> da;
        try
        {
            if(comparare(dn, da) != 1)
            {
                throw Data_Invalida("Angajatul nu s-a nascut inca!\n");
            }
            break;
        }
        catch(Data_Invalida d)
        {
            cout << d.what();
        }
    }
    a = angajat_permanent(nume, prenume, id_departament, dn, salariu, da);
    return in;
}
istream & operator >> (istream & in, contractor & a)
{
    string nume, prenume;
    int id_departament, comision, durata;
    data dn;
    in >> nume;
    in >> prenume;
    in >> id_departament;
    in >> dn;
    in >> comision;
    in >> durata;
    a = contractor(nume, prenume, id_departament, dn, comision, durata);
    return in;
}
istream & operator >> (istream & in, manager & a)
{
    string nume, prenume;
    int id_departament, salariu, bonus;
    data dn;
    in >> nume;
    in >> prenume;
    in >> id_departament;
    in >> dn;
    in >> salariu;
    in >> bonus;
    a = manager(nume, prenume, id_departament, dn, salariu, bonus);
    return in;
}
istream & operator >> (istream & in, coordonator_proiect & a)
{
    string nume, prenume;
    int id_departament, comision, durata, bonus;
    data dn;
    in >> nume;
    in >> prenume;
    in >> id_departament;
    in >> dn;
    in >> comision;
    in >> durata;
    in >> bonus;
    a = coordonator_proiect(nume, prenume, id_departament, dn, comision, durata, bonus);
    return in;
}
istream & operator >> (istream & in, departament & d)
{
    string nume;
    in >> nume;
    d = departament(nume);
    return in;
}
iterator_task* creeaza_iter(departament* dep) {
        return new iterator_task(dep);
    }
void angajat::afisare_nume()
{
    cout << nume << " " << prenume;

}
void angajat::afisare_angajat()
{
    cout << nume << " " << prenume << " " << ID << " " << data_nasterii << " " << caracterizare() << endl;
    //aici se afla o instanta de polimorfism la executie (dynamic dispatch) prin functia caracterizare
}
void departament::afiseaza_angajatii()
{
    cout << nume << ":\n";
    cout << "MANAGER: " << manager_dep->getname() << endl;
    for (int i = 0; i < angajati.size(); i++)
    {
        angajati[i]->afisare_nume();
        cout << endl;
    }
    cout << "-------------------------------------------------------" << endl;
}
void ordonare_alfabetica(vector < angajat* > angajati)
{
    vector < angajat* > ordonat;
    int nra = angajati.size();
    for (int i = 0; i < nra; i++)
    {
        ordonat.push_back(angajati[i]);
    }
    for (int i = 0; i < nra - 1; i++)
    {
        for (int j = i + 1; j < nra; j++)
        {
            if (ordonat[i]->getname() > ordonat[j]->getname())
            {
                swap(ordonat[i], ordonat[j]);
            }
        }
    }
    for (int i = 0; i < nra; i++)
    {
        ordonat[i]->afisare_angajat();
    }
}
void afisare_departamente(vector < departament > departamente, int nr_dep, vector< proiect_contractor > proiecte, int nr_p)
{
    cout << "-----DEPARTAMENTE-----\n";
    for (int i = 0; i < nr_dep; i++)
    {
        cout << departamente[i].getname() << " " << departamente[i].get_ID() << endl;
    }
    cout << "-----PROIECTE-----\n";
    for(int i = 0; i < nr_p; i++)
    {
        cout << proiecte[i].getname() << " " << proiecte[i].get_ID() << endl;
    }
}
void afisare_angajati_departamente(vector < angajat* > angajati, vector < departament* > divizii)
{
    for (int i = 0; i < angajati.size(); i++)
    {
        cout << angajati[i]->getname() << " " << angajati[i]->get_ID() << " ";
        for (int j = 0; j < divizii.size(); j++)
        {
            if (divizii[j]->get_ID() == angajati[i]->getdep())
            {
                cout << divizii[j]->getname() << endl;
                break;
            }
        }
    }
}
void afisare_departamente_angajati(vector < departament* > departamente)
{
    for (int i = 0; i < departamente.size(); i++)
    {
        departamente[i]->afiseaza_angajatii();
    }
}
void afisare_salarii(vector < angajat* > angajati)
{
    for (int i = 0; i < angajati.size(); i++)
    {
        cout << angajati[i]->getname() << " " << angajati[i]->get_venit() << endl;
        //aici se afla o instanta de polimorfism la executie (dynamic dispatch) prin functia get_venit
    }
}
void afisare_departamente_salarii(vector < departament* > departamente)
{
    for (int i = 0; i < departamente.size(); i++)
    {
        for (int j = 0; j < departamente[i]->angajati.size(); j++)
        {
            cout << departamente[i]->angajati[j]->getname() << " " << departamente[i]->getname() << " " << departamente[i]->angajati[j]->get_venit() << endl;
        }
        cout << endl;
    }
}
void afisare_angajati_data(vector < angajat* > angajati)
{
    for (int i = 0; i < angajati.size() - 1; i++)
    {
        for (int j = i + 1; j < angajati.size(); j++)
        {
            if (angajati[i]->get_data() > angajati[j]->get_data())
            {
                swap(angajati[i], angajati[j]);
            }
        }
    }
    for (int i = 0; i < angajati.size(); i++)
    {
        cout << angajati[i]->getname() << " " << angajati[i]->get_data() << endl;
    }
}
void afisare_taskuri(vector<departament> deps, int nrdep){
    for(int i = 0; i < nrdep; i++){
        cout << deps[i].getname() << ":\n";
        iterator_task* it = creeaza_iter(&deps[i]);
        for(it->element_curent(); !it->gol();it->urmatorul()){
            cout << it->eticheta_curenta() << " " << it->deadline_curent() << endl;
        }
    }
}
void adauga_angajat(int & numarp,int & numarc, vector < angajat*> & angajati, vector <angajat_permanent> &ang_perm, vector <contractor> &con)
{
    cout << "Introduceti numele si prenumele angajatului separate printr-un singur spatiu\n";
    string nume, prenume;
    int salariu, zia, lunaa, ana, comision, durata;
    data dn, da;
    char selector;
    cin >> nume >> prenume;
    cout << "Introduceti data de nastere a angajatului(in format DD MM YYYY)\n";
    cin >> dn;
    cout << "Doriti ca angajatul sa fie permanent sau un contractor? [P/C]\n";
    cin >> selector;
    if(selector == 'P')
    {
        cout << "Introduceti data la care a fost angajat(in format DD MM YYYY)\n";
        while(true)
        {
            cin >> da;
            try
            {
                if(comparare(dn, da) != 1)
                {
                    throw Data_Invalida("Angajatul nu s-a nascut inca!\n");
                }
                break;
            }
            catch(Data_Invalida d)
            {
                cout << d.what();
            }
        }
        cout << "Introduceti valoarea salariului angajatului\n";
        cin >> salariu;
        numarp++;
        ang_perm[numarp - 1] = angajat_permanent(nume, prenume, 0, dn, salariu, da);
        angajati.push_back(&ang_perm[numarp - 1]);
    }
    else
    {
        cout << "Introduceti valoarea comisionului perceput de contractor\n";
        cin >> comision;
        cout << "Introduceti durata pe care este incheiat contractul\n";
        cin >> durata;
        numarc++;
        con[numarc - 1] = contractor(nume, prenume, 0, dn, comision, durata);
        angajati.push_back(&con[numarc - 1]);
    }
}
void adauga_departament(int & numard, vector < departament > & departamente, int & numarp, vector <proiect_contractor> &proiecte, vector<departament*> &divizii)
{
    cout << "Introduceti numele departamentului\n";
    string nume;
    cin >> nume;
    char selector;
    cout << "Doriti sa introduceti un departament sau un proiect de contractori? [D/P]\n";
    cin >> selector;
    if(selector == 'D')
    {
        numard++;
        departamente[numard - 1] = departament(nume);
        divizii.push_back(&departamente[numard - 1]);
    }
    else
    {
        int zi, luna, an, fonduri;
        cout << "Introduceti deadline-ul pana la care trebuie terminat proiectul (in format DD MM YYYY)\n";
        cin >> zi >> luna >> an;
        cout << "Introduceti valoarea fondurilor alocate proiectului\n";
        cin >> fonduri;
        numarp++;
        proiecte[numarp - 1] = proiect_contractor(nume, zi, luna, an, fonduri);
        divizii.push_back(&proiecte[numarp - 1]);
    }
}
void muta_angajat(angajat* ang, departament & dep)
{
    ang->set_departmentID(dep.get_ID());
    dep.angajati.push_back(ang);
}
void sterge_angajat(angajat *ang, vector < departament* > div)
{
    for (int i = 0; i < div.size(); i++)
    {
        if (div[i]->get_ID() == ang->getdep())
        {
            for (int j = 0; j < div[i]->angajati.size(); j++)
            {
                if (div[i]->angajati[j]->get_ID() == ang->get_ID())
                {
                    for (int k = j; k < div[i]->angajati.size() - 1; k++)
                    {
                        div[i]->angajati[k] = div[i]->angajati[k + 1];
                    }
                    div[i]->angajati.resize(div[i]->angajati.size() - 1);
                    break;
                }
            }
            break;
        }
    }
}
void unire(vector < angajat* > & angajati, vector < departament > & departamente, int &nr_departamente, vector < departament* > & divizii)
{
    cout << "Scrieti numele celor doua departamente separate printr-un singur spatiu\n";
    string dep1, dep2;
    cin >> dep1 >> dep2;
    for (int i = 0; i < nr_departamente; i++)
    {
        if (departamente[i].getname() == dep1 || departamente[i].getname() == dep2)
        {
            for (int j = i + 1; j < nr_departamente; j++)
            {
                if (departamente[j].getname() == dep1 || departamente[j].getname() == dep2)
                {
                    int size_1 = departamente[i].angajati.size();
                    int size_2 = departamente[j].angajati.size();
                    departament* p2 = &departamente[j];
                    departamente[i].angajati.resize(size_1 + size_2);
                    copy(departamente[j].angajati.begin(), departamente[j].angajati.end(), departamente[i].angajati.begin() + size_1);
                    for (int k = 0; k < angajati.size(); k++)
                    {
                        if (angajati[k]->getdep() == departamente[j].get_ID())
                        {
                            angajati[k]->set_departmentID(departamente[i].get_ID());
                        }
                    }
                    for (int k = j; k < nr_departamente - 1; k++)
                    {
                        departamente[k] = departamente[k + 1];
                    }
                    nr_departamente --;
                    break;
                }
            }
            break;
        }
    }

}
void crestere_angajat(vector < angajat_permanent > & angajati, int nr_angajati, vector < departament > & dep)
{
    cout << "Introduceti id-ul angajatului\n";
    int id_ang;
    cin >> id_ang;
    cout << "Introduceti cu cat vreti sa mariti salariul(adaugati % daca doriti ca schimbarea sa fie procentuala sau * daca vreti sa fie o suma fixa)\n";
    int numar;
    char modificator;
    cin >> numar >> modificator;
    if (modificator == '%')
    {
        float procent = float(numar) / 100 + 1;
        for (int i = 0; i < nr_angajati; i++)
        {
            if (angajati[i].get_ID() == id_ang)
            {
                cout << "salariu vechi: " << angajati[i].getsalariu() << " salariu nou: " << angajati[i].getsalariu() * procent << endl;
                angajati[i].set_salariu(angajati[i].getsalariu() * procent);
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < nr_angajati; i++)
        {
            if (angajati[i].get_ID() == id_ang)
            {
                cout << "salariu vechi: " << angajati[i].getsalariu() << " salariu nou: " << angajati[i].getsalariu() + numar << endl;
                angajati[i].set_salariu(angajati[i].getsalariu() + numar);
                break;
            }
        }
    }
}
void crestere_departament(vector < angajat_permanent > & angajati,int nr_angajati, vector < departament > & dep, int nr_departamente)
{
    cout << "Introduceti numele departamentului\n";
    string nume;
    cin >> nume;
    int ID;
    for (int i = 0; i < nr_departamente; i++)
    {
        if (nume == dep[i].getname())
        {
            ID = dep[i].ID;
        }
    }
    cout << "Introduceti cu cat vreti sa mariti salariul(adaugati % daca doriti ca schimbarea sa fie procentuala sau * daca vreti sa fie o suma fixa)\n";
    int numar;
    char modificator;
    cin >> numar >> modificator;
    if (modificator == '%')
    {
        float procent = float(numar) / 100 + 1;
        for (int i = 0; i < nr_angajati; i++)
        {
            if (angajati[i].getdep() == ID)
            {
                cout << "salariu vechi: " << angajati[i].getsalariu() << " salariu nou: " << angajati[i].getsalariu() * procent << endl;
                angajati[i].set_salariu(angajati[i].getsalariu() * procent);
            }
        }
    }
    else
    {
        for (int i = 0; i < nr_angajati; i++)
        {
            if (angajati[i].getdep() == ID)
            {
                cout << "salariu vechi: " << angajati[i].getsalariu() << " salariu nou: " << angajati[i].getsalariu() + numar << endl;
                angajati[i].set_salariu(angajati[i].getsalariu() + numar);
            }
        }
    }
}
void scadere_angajat(vector < angajat_permanent > & angajati,int nr_angajati, vector < departament > & dep)
{
    cout << "Introduceti ID-ul angajatului\n";
    int id_ang;
    cin >> id_ang;
    cout << "Introduceti cu cat vreti sa micsorati salariul(adaugati % daca doriti ca schimbarea sa fie procentuala sau * daca vreti sa fie o suma fixa)\n";
    int numar;
    char modificator;
    cin >> numar >> modificator;
    if (modificator == '%')
    {
        float procent = 1 - float(numar) / 100;
        for (int i = 0; i < nr_angajati; i++)
        {
            if (angajati[i].get_ID() == id_ang)
            {
                cout << "salariu vechi: " << angajati[i].getsalariu() << " salariu nou: " << angajati[i].getsalariu() * procent << endl;
                angajati[i].set_salariu(angajati[i].getsalariu() * procent);
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < nr_angajati; i++)
        {
            if (angajati[i].get_ID() == id_ang)
            {
                cout << "salariu vechi: " << angajati[i].getsalariu() << " salariu nou: " << angajati[i].getsalariu() - numar << endl;
                angajati[i].set_salariu(angajati[i].getsalariu() - numar);
                break;
            }
        }
    }
}
void scadere_departament(vector < angajat_permanent > & angajati,int nr_angajati, vector < departament > & dep, int nr_departamente)
{
    cout << "Introduceti numele departamentului\n";
    string nume;
    cin >> nume;
    int ID;
    for (int i = 0; i < nr_departamente; i++)
    {
        if (nume == dep[i].getname())
        {
            ID = dep[i].ID;
        }
    }
    cout << "Introduceti cu cat vreti sa micsorati salariul(adaugati % daca doriti ca schimbarea sa fie procentuala sau * daca vreti sa fie o suma fixa)\n";
    int numar;
    char modificator;
    cin >> numar >> modificator;
    if (modificator == '%')
    {
        float procent = 1 - float(numar) / 100;
        for (int i = 0; i < nr_angajati; i++)
        {
            if (angajati[i].getdep() == ID)
            {
                cout << "salariu vechi: " << angajati[i].getsalariu() << " salariu nou: " << angajati[i].getsalariu() * procent << endl;
                angajati[i].set_salariu(angajati[i].getsalariu() * procent);
            }
        }
    }
    else
    {
        for (int i = 0; i < nr_angajati; i++)
        {
            if (angajati[i].getdep() == ID)
            {
                cout << "salariu vechi: " << angajati[i].getsalariu() << " salariu nou: " << angajati[i].getsalariu() - numar << endl;
                angajati[i].set_salariu(angajati[i].getsalariu() - numar);
            }
        }
    }
}
void rescrie_fisier(int nra, vector < angajat_permanent > &pang, int nrc, vector < contractor> &con, int nrd, vector < departament > &dep, int nrp, vector <proiect_contractor> &pro, vector<manager>& man, vector<coordonator_proiect> coo, int nr_taskuri)
{
    ofstream fout("output.txt");
    fout << nrd << endl;
    for (int i = 0; i < nrd; i++)
    {
        fout << dep[i];
    }
    fout << nrp << endl;
    for (int i = 0; i < nrp; i++)
    {
        fout << pro[i];
    }
    fout << endl << nra << endl;
    for (int i = 0; i < nra; i++)
    {
        fout << pang[i];
    }
    fout << nrc << endl;
    for(int i = 0; i < nrc; i++)
    {
        fout << con[i];
    }
    fout << endl;
    for(int i = 0; i < nrd; i++)
    {
        fout << man[i];
    }
    fout << endl;
    for(int i = 0; i < nrp; i++)
    {
        fout << coo[i];
    }
    fout << endl << nr_taskuri << endl << endl;
    for(int i = 0; i < nrd; i++){
        iterator_task* it = creeaza_iter(&dep[i]);
        for(it->element_curent();!it->gol();it->urmatorul()){
            fout << it->eticheta_curenta() << " " << it->deadline_curent() << " " << dep[i].get_ID() << endl;
        }
    }
}
template<class type>
void id_checker(vector<type*> &lista)
{
    set<int> ids;
    for(int i = 0; i < lista.size(); i++)
    {
        try
        {
            if(ids.find(lista[i]->get_ID()) != ids.end())
            {
                cout << "ID-urile nu mai sunt unice! Introduceti valoarea de la care va incepe realocarea acestora:\n";
                int val;
                cin >> val;
                throw ID_invalid(val);
            }
        }
        catch(ID_invalid i)
        {
            int val = i.get_val();
            type::realocare_ID_uri(lista, val);
            cout << i.what();
            break;
        }
        ids.insert(lista[i]->get_ID());
    }
}
template <class type>
void schimba_id(vector<type*> &lista){
    int id;
    type* target;
    cout << "Introduceti ID-ul pe care doriti sa il schimbati:\n";
    cin >> id;
    int gasit = 0;
    for(int i = 0; i < lista.size(); i++)
    {
        if(lista[i]->get_ID() == id)
        {
            gasit = 1;
            cout << "Introduceti noua valoare a ID-ului pentru " << lista[i]->getname() << ":\n";
            cin >> id;
            lista[i]->set_id(id);
            target = lista[i];
            break;
        }
    }
    if(gasit == 1)
        cout << "ID-ul pentru " << target->getname() << " a fost schimbat\n";
    else
        cout << "ID-ul cautat nu a fost gasit!\n";
}
int main()
{
    firma* f1 = firma::instantiere("ferrari");
    ifstream fin("date_companie.txt");
    ofstream fout("output.txt");
    vector < angajat* > lista_angajati;
    vector < angajat_permanent > l_ang_perm;
    vector < contractor > contractori;
    vector < departament > departamente;
    vector < proiect_contractor > proiecte;
    vector < departament* > divizii;
    vector < manager > manageri;
    vector < coordonator_proiect> coordonatori;
    int nr_angajati_p, nr_contractori, nr_departamente, nr_proiecte, nr_taskuri;
    angajat a = angajat();
    departament d = departament();
    a.initializare_ID(1000);
    d.initializare_ID(0);
    fin >> nr_departamente;
    departamente.resize(nr_departamente * 2);
    for (int i = 0; i < nr_departamente; i++)
    {
        fin >> departamente[i];
        divizii.push_back(&departamente[i]);
    }
    fin >> nr_proiecte;
    proiecte.resize(nr_proiecte * 2);
    for (int i = 0; i < nr_proiecte; i++)
    {
        fin >> proiecte[i];
        divizii.push_back(&proiecte[i]);
        //aici are loc o instanta de upcasting, unde pointerii de tip departament* (clasa de baza) retin adresa unui obiect de tip proiect_contractor (clasa derivata)
    }
    fin >> nr_angajati_p;
    l_ang_perm.resize(nr_angajati_p * 2);
    for (int i = 0; i < nr_angajati_p; i++)
    {
        fin >> l_ang_perm[i];
        caretaker_memento* crt = new caretaker_memento();
        l_ang_perm[i].add_caretaker(crt);
        lista_angajati.push_back(&l_ang_perm[i]);
    }
    fin >> nr_contractori;
    contractori.resize(nr_contractori * 2);
    for(int i = 0; i < nr_contractori; i++)
    {
        fin >> contractori[i];
        lista_angajati.push_back(&contractori[i]);
        //aici are loc o instanta de upcasting, unde pointerii de tip angajat* (clasa de baza abstracta) din vectorul lista_angajati retin adresele obiectelor de tip contractor (clasa derivata)
    }
    for (int i = 0; i < lista_angajati.size(); i++)
    {
        if(lista_angajati[i]->getdep() <= nr_departamente)
        {
            departamente[lista_angajati[i]->getdep() - 1].adauga_angajat(lista_angajati[i]);
        }
        else
        {
            proiecte[lista_angajati[i]->getdep() - nr_departamente - 1].adauga_angajat(lista_angajati[i]);
        }
    }
    manageri.resize(nr_departamente);
    for(int i = 0; i < nr_departamente; i++)
    {
        fin >> manageri[i];
        lista_angajati.push_back(&manageri[i]);
        departamente[manageri[i].getdep() - 1].set_manager(&manageri[i]);
    }
    coordonatori.resize(nr_proiecte);
    for(int i = 0; i < nr_proiecte; i++)
    {
        fin >> coordonatori[i];
        lista_angajati.push_back(&coordonatori[i]);
        proiecte[coordonatori[i].getdep() - nr_departamente - 1].set_manager(&coordonatori[i]);
    }
    fin >> nr_taskuri;
    for(int i = 0; i < nr_taskuri; i++){
        string eticheta;
        int iddep;
        data deadline;
        fin >> eticheta >> deadline >> iddep;
        departamente[iddep - 1].addtask(make_shared<task>(deadline, eticheta));
    }
    cout << "Scrieti aici keywordul pentru comenzile pe care vreti sa le efectuati. Scrieti STOP daca vreti ca programul sa se opreasca sau COMMIT daca vreti ca schimbarile efectuate sa se aplice in fisier\n";
    while (true)
    {
        cout << "\n\nIntroduceti un nou keyword:\n\n";
        string keyword;
        cin >> keyword;
        if (keyword == "STOP")
            break;
        else
        {
            if (keyword == "LIST")
            {
                while(true)
                {
                    int invalid = 0;
                    cout << "Scrie numarul corespunzator uneia dintre urmatoarele optiuni:\nDEPARTAMENTE 1 / ANGAJATI 2 / ANGAJATI + ID + DEPARTAMENT 3 / DEPARTAMENT + ANGAJATI 4 / ANGAJATI + VENIT 5 / ANGAJATI + DEPARTAMENT + VENIT 6 / ANGAJATI + DATA NASTERII 7 / DEPARTAMENTE + TASKURI 8\n";
                    int cod;
                    cin >> cod;
                    if (cod == 1)
                        afisare_departamente(departamente, nr_departamente, proiecte, nr_proiecte);
                    else
                    {
                        if (cod == 2)
                            ordonare_alfabetica(lista_angajati);
                        else
                        {
                            if (cod == 3)
                                afisare_angajati_departamente(lista_angajati, divizii);
                            else
                            {
                                if (cod == 4)
                                    afisare_departamente_angajati(divizii);
                                else
                                {
                                    if (cod == 5)
                                        afisare_salarii(lista_angajati);
                                    else
                                    {
                                        if (cod == 6)
                                            afisare_departamente_salarii(divizii);
                                        else
                                        {
                                            if (cod == 7)
                                            {
                                                afisare_angajati_data(lista_angajati);
                                            }
                                            else
                                            {
                                                if(cod == 8){
                                                    afisare_taskuri(departamente, nr_departamente);
                                                }
                                                else{
                                                cout << "Optiune invalida introdusa, incercati din nou" << endl;
                                                invalid = 1;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(invalid == 0)
                        break;
                }

            }
            else
            {
                if (keyword == "ADD")
                {
                    cout << "Optiuni: ANGAJAT 1 / DEPARTAMENT 2 \n";
                    int cod;
                    cin >> cod;
                    if (cod == 1)
                    {
                        string nume_departament;
                        adauga_angajat(nr_angajati_p,nr_contractori, lista_angajati, l_ang_perm, contractori);
                        cout << "Introduceti departamentul angajatului din optiunile:\n";
                        afisare_departamente(departamente, nr_departamente, proiecte, nr_proiecte);
                        cin >> nume_departament;
                        for (int i = 0; i < nr_departamente + nr_proiecte; i++)
                        {
                            if (divizii[i]->getname() == nume_departament)
                            {
                                muta_angajat(lista_angajati[lista_angajati.size() - 1], *divizii[i]);
                                break;
                            }
                        }
                        cout << "Angajatul a fost adaugat.\n";
                    }
                    else
                    {
                        adauga_departament(nr_departamente, departamente, nr_proiecte, proiecte, divizii);
                        cout << "Departamentul a fost adaugat\n";
                    }
                    id_checker(lista_angajati);
                    id_checker(divizii);
                }
                else
                {
                    if (keyword == "MOVE")
                    {
                        cout << "Introduceti id-ul angajatului\n";
                        string nume_d, nume_a;
                        int ang_id;
                        cin >> ang_id;
                        cout << "Introduceti numele departamentului\n";
                        cin >> nume_d;
                        for (int i = 0; i < nr_angajati_p + nr_contractori; i++)
                        {
                            if (lista_angajati[i]->get_ID() == ang_id)
                            {
                                nume_a = lista_angajati[i]->getname();
                                sterge_angajat(lista_angajati[i], divizii);
                                for (int j = 0; j < nr_departamente + nr_proiecte; j++)
                                {
                                    if (divizii[j]->getname() == nume_d)
                                    {
                                        muta_angajat(lista_angajati[i], *divizii[j]);
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                        cout << "Angajatul " << nume_a << " a fost mutat la departametnul " << nume_d << "\n";
                    }
                    else
                    {
                        if (keyword == "MERGE")
                        {
                            unire(lista_angajati, departamente, nr_departamente, divizii);
                            cout << "Departamentele au fost unite.\n";
                            divizii.resize(0);
                            for(int i = 0; i < nr_departamente; i++)
                            {
                                divizii.push_back(&departamente[i]);
                            }
                            for(int i = 0; i < nr_proiecte; i++)
                            {
                                divizii.push_back(&proiecte[i]);
                            }
                        }
                        else
                        {
                            if (keyword == "PAYRISE")
                            {
                                cout << "Doriti sa modificati salariul pentru un angajat (tastati 1) sau pentru un intreg departament (tastati 2)?\n";
                                int cod;
                                cin >> cod;
                                if (cod == 1)
                                {
                                    crestere_angajat(l_ang_perm, nr_angajati_p, departamente);
                                }
                                else
                                {
                                    crestere_departament(l_ang_perm, nr_angajati_p, departamente, nr_departamente);
                                }
                            }
                            else
                            {
                                if (keyword == "PAYCUT")
                                {
                                    cout << "Doriti sa modificati salariul pentru un angajat (tastati 1) sau pentru un intreg departament (tastati 2)?\n";
                                    int cod;
                                    cin >> cod;
                                    if (cod == 1)
                                    {
                                        scadere_angajat(l_ang_perm, nr_angajati_p, departamente);
                                    }
                                    else
                                    {
                                        scadere_departament(l_ang_perm, nr_angajati_p, departamente, nr_departamente);
                                    }
                                }
                                else
                                {
                                    if (keyword == "COMMIT")
                                    {
                                        id_checker(lista_angajati);
                                        id_checker(divizii);
                                        rescrie_fisier(nr_angajati_p, l_ang_perm,nr_contractori, contractori, nr_departamente, departamente, nr_proiecte, proiecte, manageri, coordonatori, nr_taskuri);
                                    }
                                    else
                                    {
                                        if(keyword == "CHANGEID")
                                        {
                                            cout << "Doriti sa schimbati ID-ul pentru un angajat(1) sau pentru un departament(2)?";
                                            int cod;
                                            cin >> cod;
                                            if(cod == 1){
                                                schimba_id(lista_angajati);
                                            }
                                            else
                                            {
                                                schimba_id(divizii);
                                            }
                                            id_checker(lista_angajati);
                                            id_checker(divizii);
                                        }
                                        else
                                        {
                                            if(keyword == "SNAPSHOT"){
                                                for(int i = 0; i < nr_angajati_p; i++){
                                                    l_ang_perm[i].get_caretaker()->add(l_ang_perm[i].creeaza_snapshot());
                                                }
                                            }
                                            else
                                            {
                                                if(keyword == "UNDOALL"){
                                                    for(int i = 0 ; i < nr_angajati_p; i++){
                                                        l_ang_perm[i].restaureaza(l_ang_perm[i].get_caretaker()->recent());
                                                        l_ang_perm[i].get_caretaker()->sterge();
                                                    }
                                                    cout << "SCHIMBARILE AU FOST ANULATE\n";
                                                }
                                                else
                                                {
                                                    if(keyword == "UNDO"){
                                                        cout << "Introuceti ID-ul angajatului pe care vreti sa il restaurati\n";
                                                        int id;
                                                        cin >> id;
                                                        for(int i = 0; i < nr_angajati_p; i++){
                                                            if(l_ang_perm[i].get_ID() == id){
                                                                l_ang_perm[i].restaureaza(l_ang_perm[i].get_caretaker()->recent());
                                                                l_ang_perm[i].get_caretaker()->sterge();
                                                                break;
                                                            }
                                                        }
                                                        cout << "SCHIMBARILE AU FOST ANULATE\n";

                                                    }
                                                    else
                                                    {
                                                        cout << "KEYWORD INVALID INTRODUS\n";
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    }
    return 0;
}
