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
            std::copy(ds.items, ds.items + ds.count, this->items);
        }

        ~ ADs()
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

template <typename VectorType> class Vector : public ADs <VectorType>
{
    protected:
        int TOP;
    
    public:
        Vector() : ADs<VectorType>::ADs(5) //constructor chaining
        {
            TOP = -1;
        }

        Vector(size_t Capacity) : ADs<VectorType>::ADs(Capacity)   //constructor chaining
        {
            TOP = -1;
        }

        Vector(const Vector& v) : ADs<VectorType>::ADs(v) 
        {
            TOP = v.TOP;
        }

        Vector& operator= (const Vector&v)
        {
            ADs<VectorType>::operator=(v);
            TOP = v.TOP;
            
            return *this;
        }

        VectorType& back() const
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

        void push_back(const VectorType& x)
        {
            if(this->count + 1 >= this->Capacity) this->reserve(2 * this->Capacity);
            TOP += 1;
            this->items[TOP] = x;
            this->count += 1;
        }

        void pop_back()
        {
            if(TOP == -1) return; //vector is empty
            TOP -= 1;

            this->count -= 1;
        }

        VectorType& operator[](int index) const
        {
            if (index >= 0 && index < this->count) return this->items[index];
            else throw std::out_of_range("Index out of range");
        }

        void clear()
        {
            ADs<VectorType> :: clear();
            TOP = -1;
        }

        ~Vector()
        {
            TOP = -1;
        }
};

template<typename StackType> class Stack : protected Vector <StackType> /*Protected/private inheritance vs composition*/
{
    public:
        Stack() : Vector<StackType>::Vector(5) {} //constructor chaining

        Stack(size_t Capacity) : Vector<StackType>::Vector(Capacity) {}   //constructor chaining

        Stack(const Stack& s) : Vector<StackType>::Vector(s) {} //copy constructor

        StackType& top() const
        {
            return this->back();
        }

        void push(const StackType& x)
        {
            this->push_back(x);
        }

        void pop()
        {
            this->pop_back();
        }

        using Vector<StackType>::size;
        using Vector<StackType>::capacity;
        using Vector<StackType>::empty;
        using Vector<StackType>::operator=; 
        using Vector<StackType>::clear;

        /*Note: Why have we not written 'using Vector<StackType>::~ADs' (writing this gives an error infact) ?
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

        ~Queue()
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

