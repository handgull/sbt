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
    @return puntatore a root dell'albero copia
    **/
    template<typename Q>
    node<T> *copy_tree(node<Q> *head) {
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
    template<typename Q, typename C>
    search_binary_tree(const search_binary_tree<Q, C> &other) : _size(other._size) {
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
        search_binary_tree st = new search_binary_tree;
        node<T> *head = find_node(d);
        if (head != 0) {
            st._root = copy_tree(head);
        }
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
/**
template <typename T, typename F>
std::ostream &operator<<(std::ostream &os, const search_binary_tree<T,F> &sbt) {
    os << sbt.get_size();
  return os;
}
**/

#endif
