#include <string.hpp>


class dictionary {
  private:
    int numElements;

    struct node {
        int key;
        string value;
        dictionary* child;
        node* next;
    };

    node* head;
    
    public:
    dictionary(/* args */);
    ~dictionary(/* args */);
    void add(int key, string value);
    void remove(int key);
    node* getByKey(int key);
    node* getByValue(string& value);
    dictionary* createChildDictionary(int key);
    void deleteAll();
};