#include <string.hpp>

class dictionary {
  public:
    struct node {
        int key;
        string value;
        dictionary* child;
        node* next;
    };

  private:
    int numElements;
    node* head;

  public:
    dictionary(/* args */);
    ~dictionary(/* args */);
    void add(int key, string value);
    void remove(int key);
    node* getByKey(int key);
    node* getByValue(string& value);
    dictionary* createChildDictionary(int key);
};