#pragma once

#ifndef SEARCH_BINARY_TREE_H
#define SEARCH_BINARY_TREE_H
#include <iostream>

/**
@file SparseMatrix.h
@brief Dichiarazione della classe SparseMatrix e delle strutture usate dalla classe
**/

typedef unsigned int uint;

template<typename T, typename F>
class search_binary_tree {
private:
    template<typename Q>
    struct node {
        Q value;
        node *left, *right;

        node(const Q value) : value(value), left(0), right(0) {}

        node(const Q value, node *l , node *r) : value(value), left(l), right(r) {}

        ~node() {
            delete left;
            delete right;
            left = 0;
            right = 0;
        }
    };

    node<T> *_root;
    F _funct;
    uint _size;
    
    /**
    @brief clear

    Metodo privato che si occupa di ripulire l'albero
    **/
    void clear() {
        delete _root;
        _size = 0;
    }
    
    /**
    @brief copy_tree

    Metodo privato che copia la struttura di un albero data la sua radice
    NOTA: il tipo template è messo per rendere la funzione più "intelligente" ma in questa implementazione non è necessario essendo che lavoro sempre su SBT con tipo T
    @return puntatore a root dell'albero copia
    **/
    template<typename Q>
    node<T> *copy_tree(node<Q> *head) const {
        return head == 0 ? 0 : new node<T>(static_cast<T>(head->value), copy_tree(head->left), copy_tree(head->right));
    }
    
    /**
    @brief find_node
    
    Trova l'elemento nell'albero, usando il funtore per determinarne la posizione e ritorna il nodo
    @param v il valore del nodo
    @return l'indirizzo del nodo trovato, 0 se il nodo non esiste
    **/
    node<T> *find_node(const T &v) const {
        node<T> *head = _root;
        while(head != 0) {
            if (head->value == v) {
                return head;
            }
            head = _funct(v, head->value) == 1 ? head->right : head->left;
        }
        return 0;
    }
    
    /**
    @brief count
    
    Calcola ricorsivamente la dimensione dell'albero
    @param n nodo di partenza
    @return la dimensione dell'albero
    **/
    uint count(const node<T> *n) {
        if (n == 0) {
            return 0;
        }
        return count(n->left) + count(n->right) + 1;
    }

public:
    /**
    @brief Costruttore di default (METODO FONDAMENTALE)

    Costruttore di default per istanziare un search_binary_tree vuoto.
    **/
    search_binary_tree() : _root(0), _size(0) {}

    /**
    @brief Root constructor

    Permette di istanziare un search_binary_tree con una root valorizzata
    @param v (valore della radice dell'albero)
    **/
    explicit search_binary_tree(const T &v) : _root(new node<T>(v)), _size(1) {
    }

    /**
    @brief Copy constructor (METODO FONDAMENTALE)

    Costruttore copia. Permette di istanziare un search_binary_tree con i nodi
    copiati da un altro search_binary_tree.
    @param other (un altro search_binary_tree, da usare per creare la copia)
    **/
    search_binary_tree(const search_binary_tree &other) : _size(other._size) {
        try {
            _root = copy_tree(other._root);
        } catch(...) {
            clear();
            throw;
        }
    }

    /**
    @brief Distruttore (METODO FONDAMENTALE)
    
    Distruttore. Rimuove la memoria allocata dall'oggetto.
    **/
    ~search_binary_tree() {
        clear();
    }
    
    /**
    @brief get_size
    @return Il numero di elementi totale dell'albero
    **/
    uint get_size() const {
        return _size;
    }
    
    /**
    @brief insert
    
    Inserisce l'elemento nell'albero, usando il funtore per determinarne la posizione
    @param v il valore del nodo
    **/
    void insert(const T &v) {
        if (_root == 0) {
            _root = new node<T>(v);
        }
        else {
            node<T> *father = _root;
            for (node<T> *head = father; head != 0; father = head ? head : father) {
                if (_funct(v, head->value) == 0) {
                    throw std::runtime_error("Error! The SBT doesn't allow duplicates\n");
                    return;
                }
                head = _funct(v, head->value) == 1 ? head->right : head->left;
            }
            if (_funct(v, father->value) == 1) {
                father->right = new node<T>(v);
            }
            else {
                father->left = new node<T>(v);
            }
        }
        _size++;
    }
    
    /**
    @brief search
    
    Trova l'elemento nell'albero, usando il funtore per determinarne la posizione
    @param v il valore del nodo
    @return true se il valore è presente nel SBT, false altrimenti
    **/
    bool search(const T &v) const {
        return find_node(v) != 0;
    }
    
    /**
    @brief subtree
    
    Trova l'elemento nell'albero, usando il funtore per determinarne la posizione
    @param d il valore del nodo
    @return il subtree, se il valore d non era presente ritorno un albero vuoto
    **/
    search_binary_tree subtree(const T &d) const {
        search_binary_tree<T, F> st;
        node<T> *head = find_node(d);
        if (head != 0) {
            st._root = copy_tree(head);
        }
        st._size = st.count(st._root);
        return st;
    }

    /**
    @brief operator=

    Operatore di assegnamento che permette la copia tra SBT.
    @param other (l'oggetto da cui copiare)
    @return puntatore all'oggetto che lancia il metodo.
    **/
    search_binary_tree &operator=(const search_binary_tree &other) {
        if (this != &other) {
            this->clear();
            this->_root = copy_tree(other._root);
            this->_size = other._size;
        }
        return *this;
    }


    class const_iterator {
    private:
        const node<T> **_array;
        uint _len;

        // La classe container deve essere messa friend dell'iteratore per poter
        // usare il costruttore di inizializzazione.
        friend class search_binary_tree;

        // Costruttore privato di inizializzazione usato dalla classe container
        // tipicamente nei metodi begin e end
        explicit const_iterator(const node<T> *h) : _len(1) {
            _array[0] = h;
        }

    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef node<T> value_type;
        typedef ptrdiff_t difference_type;
        typedef const node<T> *pointer;
        typedef const node<T> &reference;

        const_iterator() : _array(0), _len(0) {}

        const_iterator(const const_iterator &other) : _array(other._array), _len(other._len) {}

        const_iterator &operator=(const const_iterator &other) {
            if (this != &other) {
                _array = other._array;
                _len = other._len;
            }
            return *this;
        }

        ~const_iterator() {}

        // Ritorna il dato riferito dall'iteratore (dereferenziamento)
        reference operator*() const {
            return *_array[_len - 1];
        }

        // Ritorna il puntatore al dato riferito dall'iteratore
        pointer operator->() const {
            return _array[_len - 1];
        }

        // Operatore di iterazione post-incremento
        const_iterator operator++(int) {
            const_iterator tmp(*this);
            if (_len != 0) {
                node<T> *l = _array[_len - 1]->left;
                node<T> *r = _array[_len - 1]->right;
                _len--;
                if (l != 0) {
                    _array[_len] = l;
                    _len++;
                }
                if (r != 0) {
                    _array[_len] = r;
                    _len++;
                }
            }
            return tmp;
        }

        // Operatore di iterazione pre-incremento
        const_iterator &operator++() {
            if (_len != 0) {
                node<T> *l = _array[_len - 1]->left;
                node<T> *r = _array[_len - 1]->right;
                _len--;
                if (l != 0) {
                    _array[_len] = l;
                    _len++;
                }
                if (r != 0) {
                    _array[_len] = r;
                    _len++;
                }
            }
            return *this;
        }

        // Uguaglianza
        bool operator==(const const_iterator &other) const {
            return this->_array[_len - 1] == other._array[other._len - 1];
        }

        // Diversita'
        bool operator!=(const const_iterator &other) const {
            return this->_array[_len - 1] != other._array[other._len - 1];
        }
    };

    // Ritorna l'iteratore all'inizio della sequenza dati
    const_iterator begin() const {
        return const_iterator(_root);
    }

    // Ritorna l'iteratore alla fine della sequenza dati
    const_iterator end() const {
        return const_iterator();
    }

};

/**
@brief opeartor<<

Operatore di stream, permette di spedire su uno stream
di output il contenuto dell'albero.
@param os (lo stream target)
@param sbt (search_binary_tree da utilizzare)
@return Il riferimento allo stream di output
**/
template <typename T, typename F>
std::ostream &operator<<(std::ostream &os, const search_binary_tree<T,F> &sbt) {
    typename search_binary_tree<T, F>::const_iterator it, ite;
    it = sbt.begin();
    ite = sbt.end();
    while (it != ite) {
        os << it.operator*().value;
        it++;
    }
    return os;
}

#endif
