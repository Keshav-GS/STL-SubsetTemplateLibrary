#include "TL.cpp"

using std::cout, std::endl, std::string;

int main()
{
    //string does not work properly. Once realloc is called after filling in few elements, it breaks down

    Vector<int> * v0;
    v0 = new Vector<int>();
    v0->push_back(42);
    cout<<"First element, size of v0: "<<(*v0)[0]<< ", "<<(*v0).size()<<endl;
    delete v0;

    Stack<int> s1;
    s1.push(1);
    s1.push(2);
    s1.push(3);
    s1.push(4);
    s1.push(5);
    s1.push(6);
    cout<<"Top of stack s1 contains: "<<s1.top()<<endl;
    cout<<"Stack s1 size, capacity = "<<s1.size()<<","<<s1.capacity()<<endl;

    Stack<int> s2;
    s2 = s1;
    s1.clear();
    cout<<"Top of stack s2 contains: "<<s2.top()<<endl;
    cout<<"Stack s2 size, capacity = "<<s2.size()<<","<<s2.capacity()<<endl;

    Stack<int> s3(s2);
    cout<<"Top of stack s3 contains: "<<s3.top()<<endl;
    cout<<"Stack s3 size, capacity = "<<s3.size()<<","<<s3.capacity()<<endl;

    Vector<int> v1(3);
    v1.push_back(3);
    v1.push_back(4);
    v1.push_back(5);
    v1.push_back(6);
    cout<<"v1[0]: "<<v1[0]<<endl;

    Vector<Vector<int>> v2;
    v2.push_back(v1);
    v1.clear();
    cout<<"v2[0][1]: "<<v2[0][1]<<endl;
    cout<<"v1 size, capacity = "<<v1.size()<<","<<v1.capacity()<<endl;
    cout<<"v2 size, capacity = "<<v2.size()<<","<<v2.capacity()<<endl;

    Vector<Vector<int>> v3(v2);
    cout<<"v3[0][2]: "<<v3[0][2]<<endl;
    cout<<"v3 size, capacity = "<<v3.size()<<","<<v3.capacity()<<endl;

    Vector<Vector<int>> v4;
    v4 = v3;
    cout<<"v3[0][3]: "<<v3[0][3]<<endl;
    cout<<"v4[0][3]: "<<v4[0][3]<<endl;
    v3.clear();
    cout<<"After clearing v3, v3.size() and v4[0][3]: "<<v3.size()<<", "<<v4[0][3]<<endl;

    Vector<Stack<int>> v5;
    v5.push_back(s2);
    cout<<"v5[0].top(): "<<v5[0].top()<<endl;

    Dequeue<int> q1(5);
    q1.push_front(7);
    q1.push_front(8);
    cout<<"Front and rear of q1 contains: "<<q1.front()<<", "<<q1.rear()<<endl;
    cout<<"q1 size, capacity = "<<q1.size()<<","<<q1.capacity()<<endl;

    Queue<float> q2(3);
    q2.push(3.4);
    q2.push(100.71);
    q2.push(-6);
    q2.pop();
    q2.pop();
    q2.push(1);
    q2.push(2);
    q2.push(3);
    cout<<"Front of q2 contains: "<<q2.front()<<endl;
    cout<<"q2 size, capacity = "<<q2.size()<<","<<q2.capacity()<<endl;

    Queue<float> q3(q2);
    q3.pop();
    cout<<"Front of q3 contains: "<<q3.front()<<endl;
    cout<<"q3 size, capacity = "<<q3.size()<<","<<q3.capacity()<<endl;

    Queue<Dequeue<int>> q4(20);
    q4.push(q1);
    cout<<"Front of the first queue in q4 contains: "<<q4.front().front()<<endl;
    cout<<"q4 size, capacity = "<<q4.size()<<", "<<q4.capacity()<<endl;

    Dequeue<int> q8(1); 
    q8.push_front(7);
    q8.push_front(8);
    q8.push_front(9);
    q8.push_front(10);
    q8.push_back(1);
    q8.push_back(2);
    q8.push_front(11);
    q8.pop_front();
    q8.pop_back();
    cout<<"Checking dequeue indexing, First element of dequeue q8 contains: " << q8[0] << endl; //expected output: 10
    cout<<"Checking dequeue indexing, Last element of dequeue q8 contains: " << q8[q8.size() - 1] << endl; //expected output: 1
    Dequeue<Dequeue<int>> q9(2);
    q9.push_back(q8);
    cout<<"Front of first dequeue in q9 contains: "<<q9.front().front()<<endl;

    Dequeue<int> q6(5);
    q6.push_back(11);
    q6.push_back(12);
    q6.push_front(13);
    q6.push_front(14);
    Dequeue<int> q5(4);
    q5 = q6;
    q6.clear();
    cout<<"At front of q5: "<<q5.front()<<endl;
    cout<<"At rear of q5: "<<q5.rear()<<endl;


    //VECTOR
    // Vector<int>* v0 = new Vector<int>(3);
    // Vector<int> x = *v0;
    // v0->push_back(1);
    // v0->push_back(2);
    // v0->push_back(3);
    // v0->pop_back();
    // cout<<"v0 size, capacity = "<<v0->size()<<","<<v0->capacity()<<endl;
    // cout<<"At back of v0: "<<v0->back()<<endl;
    // x.push_back(10);
    // x.push_back(11);
    // x.push_back(12);
    // cout<<"x size, capacity = "<<x.size()<<","<<x.capacity()<<endl;
    // cout<<"At back of x: "<<x.back()<<endl;
    // delete v0;
    // cout<<"After deleting v0: v0 size, capacity = "<<v0->size()<<","<<v0->capacity()<<endl;
}