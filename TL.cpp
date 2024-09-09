#include<iostream>

using std::endl;

template <typename ADsType> class ADs    //ADs=>'Array based Datastructure'
{
    protected:
        size_t count;  //number of elements currently present
        size_t Capacity;   //Maximum number of elements that can be accomodated currently
        ADsType* items;
    
        ADs(size_t Capacity)
        {
            count = 0;
            this->Capacity = Capacity;
            items = new ADsType[Capacity];
        }

        ADs(const ADs& ds) //copy constructor
        {
            count = ds.count;
            Capacity = ds.Capacity;
            items = new ADsType[ds.Capacity];
            std::copy(ds.items, ds.items + ds.Capacity, this->items);
        }

        virtual ~ ADs() //whenever a class might be extended, always a good idea to make the destructor virtual
        {
            /*unlike free, delete calls destructors of each obj in item array before deleting items from heap*/
            if(items) delete []items; 

            items = nullptr;
            Capacity = 0;
            count = 0;
        }

        void reserve(size_t Capacity) 
        {
            ADsType* temp = this->items;

            Capacity = std::max(Capacity, static_cast<size_t>(1));
            this->items = new ADsType[Capacity];
            for(size_t i = 0; i < std::min(Capacity, this->Capacity); i++) this->items[i] = temp[i];
            this->Capacity = Capacity;

            if(temp) delete []temp;
        }

    public:
        size_t size() const /*About const here: https://stackoverflow.com/questions/15999123/const-before-parameter-vs-const-after-function-name-in-c*/
        {
            return count;
        }

        size_t capacity() const
        {
            return Capacity;
        }

        bool empty() const
        {
            return count == 0;
        }

        virtual ADs& operator= (const ADs& ds) //needed because default assignment operator is shallow copy
        {
            if(items) delete[] items;
            count = ds.count;
            Capacity = ds.Capacity;
            items = new ADsType[ds.Capacity];
            for(size_t i = 0; i < Capacity; i++) items[i] = ds.items[i];

            return *this;

            /*Look at using the assignment operator inside copy constructor or vice versa. To avoid kinda duplication of same effort.*/
        }

        virtual void clear()
        {
            int temp_capacity = Capacity;
            this->~ADs();
            Capacity = temp_capacity;
            items = new ADsType[Capacity];
        }
};

template <typename StackType> class Stack : public ADs <StackType>
{
    protected:
        int TOP;
    
    public:
        Stack() : ADs<StackType>::ADs(5) //constructor chaining
        {
            TOP = -1;
        }

        Stack(size_t Capacity) : ADs<StackType>::ADs(Capacity)   //constructor chaining
        {
            TOP = -1;
        }

        Stack(const Stack& s) : ADs<StackType>::ADs(s) 
        {
            TOP = s.TOP;
        }

        Stack& operator= (const Stack&s)
        {
            ADs<StackType>::operator=(s);
            TOP = s.TOP;
            
            return *this;
        }

        StackType& top() const
        {   
            try
            {
                return this->items[TOP];
            }
            catch(const std::exception& e)
            {
                std::cerr<<e.what()<<endl;
                exit(0);
            }
        }

        void push(const StackType& x)
        {
            if(this->count + 1 >= this->Capacity) this->reserve(2 * this->Capacity);
            TOP += 1;
            this->items[TOP] = x;
            this->count += 1;
        }

        void pop()
        {
            if(TOP == -1) return; //stack is empty
            TOP -= 1;

            this->count -= 1;
        }

        void clear()
        {
            ADs<StackType> :: clear();
            TOP = -1;
        }

        virtual ~Stack()
        {
            TOP = -1;
        }
};

template<typename VectorType> class Vector : protected Stack <VectorType> /*Protected/private inheritance vs composition*/
{
    public:
        Vector() : Stack<VectorType>::Stack(5) {} //constructor chaining

        Vector(size_t Capacity) : Stack<VectorType>::Stack(Capacity) {}   //constructor chaining

        Vector(const Vector& v) : Stack<VectorType>::Stack(v) {} //copy constructor

        VectorType& back() const
        {
            return this->top();
        }

        void push_back(const VectorType& x)
        {
            this->push(x);
        }

        void pop_back()
        {
            this->pop();
        }

        VectorType& operator[](int index) const
        {
            if (index >= 0 && index < this->count) return this->items[index];
            else throw std::out_of_range("Index out of range");
        }

        using Stack<VectorType>::size;
        using Stack<VectorType>::capacity;
        using Stack<VectorType>::empty;
        using Stack<VectorType>::operator=; 
        using Stack<VectorType>::clear;

        /*Note: Why have we not written 'using Stack<VectorType>::~ADs' (writing this gives an error infact) ?
        It is because when stack is generated the compiler automatically implements a destructor for stack class
        as we have not declared one. The compiler then generates code that calls the base class destructors. In other
        words, the base class destructors are not inherited but get called from after the dervided class destructor.
        */
};

/*Dequeue is implemented in STL as a dynamic array of dynamic arrays:
https://stackoverflow.com/a/24483402/22839993*/

template <typename QueueType> class Queue : public ADs <QueueType>
{
    protected:
        long long f;
        long long r;

        void resize_queue(size_t C)
        {
            this->reserve(C);

            /*As we have increased Capacity, rearrange queue contents in linear fashion and reset values of f and r accordingly*/ 
            long long i = f - 1;
            size_t k = 0;
            QueueType temp[this->count];
            do
            {
                i = (i + 1)%(this->Capacity/2);   
                temp[k++] = this->items[i];
            }while(i != r);
            
            this->f = 0;
            r = this->count - 1;

            for(size_t i = 0; i < this->count; i++) this->items[i] = temp[i];
        }

    public:
        Queue() : ADs<QueueType>::ADs(5) //constructor chaining
        {
            f = -1;
            r = -1;
        }

        Queue(size_t Capacity) : ADs<QueueType>::ADs(Capacity)  //constructor chaining
        {
            f = -1;
            r = -1;
        }

        Queue(const Queue& q) : ADs<QueueType>::ADs(q) //copy constructor
        {
            f = q.f;
            r = q.r;
        }

        QueueType& front() const
        {
            if (f >= 0 && f < this->Capacity) return this->items[f];
            else throw std::out_of_range("Index out of range");
        }

        QueueType& rear() const
        {
            if (r >= 0 && r < this->Capacity) return this->items[r];
            else throw std::out_of_range("Index out of range");        
        }

        Queue& operator =(const Queue& q)
        {
            ADs<QueueType>::operator=(q);
            f = q.f;
            r = q.r;

            return *this;
        }

        void push(const QueueType& x)
        {
            if(this->count == this->Capacity)
            {
                this->resize_queue(this->Capacity * 2);
                r = this->count;
                f = 0;
            } 
            else
            {
                r = (r + 1)%this->Capacity; 
                if(f == -1) f++;
            }
            this->items[r] = x; 
            
            this->count += 1;        
        }

        void pop()
        {
            if(this->count == 0) return; //queue is empty
            else if(f == r) //last element present
            {
                f = -1;
                r = -1;
            }
            else
            {
                f = (f + 1)%this->Capacity; 
            }

            this->count -= 1;
        }   

        void clear()
        {
            ADs<QueueType> :: clear();
            f = -1;
            r = -1;
        }

        virtual ~Queue()
        {
            f = -1;
            r = -1;
        }
};

template <typename DequeueType> class Dequeue : protected Queue <DequeueType> /*Circular buffer used*/
{
    public:
        Dequeue() : Queue<DequeueType>::Queue(5) {}

        Dequeue(size_t Capacity) : Queue<DequeueType>::Queue(Capacity) {}

        Dequeue(const Dequeue& dq) : Queue<DequeueType>::Queue(dq) {} //copy constructor

        DequeueType& operator[](int index) const
        {
            if (index >= 0 && index < this->count) return this->items[(index + this->f) % this->Capacity];
            try {
                throw std::out_of_range("Index out of range");
            } 
            catch(const std::exception& e)
            {
                std::cerr<<e.what()<<endl;
                exit(0);
            }
        }

        void push_back(const DequeueType& x)
        {
            this->push(x);
        }

        void pop_front()
        {
            this->pop();
        }   

        void push_front(const DequeueType& x) 
        {
            if(this->count == this->Capacity)
            {
                this->resize_queue(this->Capacity * 2);
                this->r = this->count - 1;
                this->f = this->Capacity - 1;
            } 
            else if(this->f == 0)
            {
                this->f = this->Capacity - 1;
            }
            else if(this->f == -1) 
            {
                this->f = 0;
                this->r = 0;
            }
            else
            {
                this->f = (this->f - 1)%this->Capacity;

            }  
            this->items[this->f] = x;

            this->count += 1;
        }

        void pop_back()
        {
            if(this->count == 0) return; //queue is empty
            else if(this->f == this->r) //last element present
            {
                this->f = -1;
                this->r = -1;
            }
            else if(this->r == 0) 
            {
                this->r = this->Capacity - 1;
            }
            else 
            {
                this->r = (this->r - 1)%this->Capacity;
            }

            this->count -= 1;
        }

        using Queue<DequeueType>::front;
        using Queue<DequeueType>::rear;
        using Queue<DequeueType>::size;
        using Queue<DequeueType>::capacity;
        using Queue<DequeueType>::empty;
        using Queue<DequeueType>::operator=; 
        using Queue<DequeueType>::clear;
};

