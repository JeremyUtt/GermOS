#include "dictionary.hpp"

#include <memory.hpp>
#include <utils.hpp>


dictionary::dictionary(/* args */) {
    this->head = nullptr;
    this->numElements = 0;
}

dictionary::~dictionary() {
    this->deleteAll();
}

void dictionary::add(int key, string value) {
    node tempNode = node();
    node* newNode = reinterpret_cast<node*>(malloc(sizeof(node)));
    memcpy(&tempNode, newNode, sizeof(node));

    newNode->key = key;
    newNode->value = value;
    newNode->child = nullptr;
    newNode->next = nullptr;

    node* current = this->head;

    if (this->head == nullptr) {
        this->head = newNode;
        this->numElements++;
        return;
    }

    while (current->next != nullptr) {
        if (current->key == key) {
            // Key already exists, update the value
            current->value = value;
            return;
        }
        current = current->next;
    }
    current->next = newNode;
    this->numElements++;
}

void dictionary::remove(int key) {
    node* current = this->head;
    node* previous = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (previous == nullptr) {
                // Removing the head node
                this->head = current->next;
            } else {
                previous->next = current->next;
            }
            
            if (current->child != nullptr) {
                current->child->deleteAll();
                free(current->child);
            }
            
            free(current);
            this->numElements--;
            return;
        }
        previous = current;
        current = current->next;
    }
}


dictionary::node* dictionary::getByKey(int key) {
    node* current = this->head;
    while (current != nullptr) {
        if (current->key == key) {
            return current;
        }
        current = current->next;
    }
    return nullptr; // Key not found
}

dictionary::node* dictionary::getByValue(string& value) {
    node* current = this->head;
    while (current != nullptr) {
        if (current->value == value) {
            return current;
        }
        current = current->next;
    }
    return nullptr; // Value not found
}

dictionary* dictionary::createChildDictionary(int key){
    node* parentNode = this->getByKey(key);
    if (parentNode == nullptr) {
        return nullptr; // Key not found
    }

    if (parentNode->child == nullptr) {
        dictionary tempDict = dictionary();
        parentNode->child = reinterpret_cast<dictionary*>(malloc(sizeof(dictionary)));
        memcpy(&tempDict, parentNode->child, sizeof(dictionary));
        
        return parentNode->child;
    }
    return parentNode->child;
}


void dictionary::deleteAll(){
    node* current = this->head;
    while (current != nullptr) {
        node* nextNode = current->next;
        if (current->child != nullptr) {
            current->child->deleteAll();
            free(current->child);
        }
        free(current);
        current = nextNode;
    }
    this->head = nullptr;
    this->numElements = 0;
}
