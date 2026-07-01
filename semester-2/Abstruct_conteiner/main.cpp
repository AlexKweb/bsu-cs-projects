#include "Container.h"

 class Array: public AbstractContainer
 { 
    friend class IteratorArray;
 private:
    int* data;
    int size;
    int capacity;
public:
    Array(int _size = 16);
    Array(Array &a);
    ~Array();
    bool IsEmpty() const;
    bool IsFull() const ;    
    Array& operator=(const Array& a);
    int operator[](int index) const;
 };

 Array::Array(int _size){
    size = _size;
    data = new int[size];
 }
 Array::Array(Array &a){
    size = a.size;
    data = new int[size];
    for(int i = 0; i < size; i++){
        data[i] = a.data[i];
    }
 }
 Array::~Array(){
    delete[] data;
 }
 bool Array::IsEmpty() const{
    return size == 0;
 }
 bool Array::IsFull() const{
    return size == capacity;
 }
 Array& Array::operator=(const Array& a){
    if(this == &a) return *this;
    delete[] data;
    size = a.size;
    data = new int[size];
    for(int i = 0; i < size; i++){
        data[i] = a.data[i];
    }
    return *this;
 }
 int Array::operator[](int index) const{
    return data[index];
 }

 class IteratorArray: public AbstractIterator
 {
private:
    Array &a;
    int pos;
public:
    IteratorArray(Array& _a);
    bool InRange() const;
    void Reset();
    int& operator *() const;
    void operator ++();
    IteratorArray&  operator=(const IteratorArray& it);
    IteratorArray&  operator=(const int& n);

 };
IteratorArray::IteratorArray(Array& _a): a(_a), pos(0) {}
bool IteratorArray::InRange() const{
    return pos < a.size;
}
void IteratorArray::Reset(){
    pos = 0;
}
int& IteratorArray::operator *() const{
    return a.data[pos];
}
void IteratorArray::operator ++(){
    pos++;
}
IteratorArray& IteratorArray::operator=(const IteratorArray& it){
    if(this == &it) return *this;
    a = it.a;
    pos = it.pos;
    return *this;
}
IteratorArray& IteratorArray::operator=(const int& n){
    if(InRange()){
        a.data[pos] = n;
    }
    return *this;
}


/////////////////////////
ArrayStack::ArrayStack(int _size): size(_size), top(0){
    p = new int[size];
}
ArrayStack::ArrayStack(ArrayStack &s): size(s.size), top(s.top){
    p = new int[size];
    for(int i = 0; i < size; i++){
        p[i] = s.p[i];
    }
}
ArrayStack::~ArrayStack(){
    delete[] p;
}
void ArrayStack::push(const int& n){
    if(!IsFull()){
        p[top++] = n;
    }
}
void ArrayStack::pop(int& n){
    if(!IsEmpty()){
        n = p[--top];
    }
}
bool ArrayStack::IsEmpty() const{       
    return top == 0;
}
bool ArrayStack::IsFull() const{
    return top == size;
}
void ArrayStack::print() const{
    for(int i = 0; i < top; i++){
        std::cout << p[i] << " ";
    }
    std::cout << std::endl;
}



// class ArrayStackIterator: public AbstractIterator
// {
//     friend class ArrayStack;
// private:
//     ArrayStack  &a;      // ссылка на стек
//     int    pos;      // текущая позиция итератора
// public: 
//     ArrayStackIterator(ArrayStack& _a);
//     bool InRange() const;      // индекс в допустимых пределах
//     void Reset();        // сбросить индекс в начало
//     int& operator *() const;    // разыменование (чтение элемента)
//     void operator ++();      // сдвиг на элемент
// };

ArrayStackIterator::ArrayStackIterator(const ArrayStack& _a)
    : a(const_cast<ArrayStack&>(_a)), pos(0) {}
bool ArrayStackIterator::InRange() const{
    return pos < a.top;
}
void ArrayStackIterator::Reset(){
    pos = 0;
}
int& ArrayStackIterator::operator *() const{
    return a.p[pos];
}
void ArrayStackIterator::operator ++(){
    pos++;
}

int main(){

    AbstractStack* pStack = new ArrayStack(10);

        for(int i = 0; i < 10; i++){
            pStack->push(i);
            static_cast<ArrayStack*>(pStack)->print();
        }

    std::cout << std::endl;
        
    AbstractIterator* it = new ArrayStackIterator(*static_cast<ArrayStack*>(pStack));
    while(it->InRange()){
        std::cout << *(*it) << " ";
        ++(*it);
    }
    std::cout << std::endl;
    std::cout << std::endl;

    delete pStack;
    return 0;
}
