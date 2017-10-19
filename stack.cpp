//
// Created by ZhangYutong on 10/16/17.
//

class Stack {
public:
    Stack(int sz);
    ~Stack();
    void Push(int value);
    bool Full();
private:
    int size;
    int top;
    int* stack;
};

Stack::Stack(int sz) {
    size = sz;
    top = 0;
    stack = new int[size];
}

Stack::~Stack() {
    delete [] stack;
}

//int main() {
//    Stack s1(17);
//    Stack* s2 = new Stack(23);
//    s1.Push(11);
//    s2->Push(12);
//}
