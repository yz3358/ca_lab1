using namespace std;

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

void
Stack::Push(int value) {
    assert();
}

int main() {
    Stack s1(17);
    Stack* s2 = new Stack(23);
    s1.Push(11);
    s2->Push(12);
}
