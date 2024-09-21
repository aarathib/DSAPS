#include <iostream>
#include <random>

using namespace std;

template <class T>
class deque
{
private:
    T *arr;
    int front;
    int rear;
    int size;
    int capacity;

    T defaultValue{};

public:
    deque()
    {
        arr = nullptr;
        front = -1;
        rear = -1;
        size = 0;
        capacity = 0;
    }

    deque(int n)
    {
        arr = new T[n];
        front = -1;
        rear = -1;
        size = n;
        capacity = n;
        for (int i = 0; i < n; i++)
        {
            arr[i] = T();
        }
    }

    deque(int n, const T &x)
    {
        arr = new T[n];
        front = -1;
        rear = -1;
        size = n;
        capacity = n;
        for (int i = 0; i < n; i++)
        {
            arr[i] = x;
        }
    }

    ~deque()
    {
        delete[] arr;
    }

    bool push_back(const T &element)
    {
        if (size == capacity)
        {
            int new_capacity = (capacity == 0) ? 1 : 2 * capacity;
            reserve(new_capacity);
        }
        if (front == -1 && rear == -1)
        {
            front = 0;
            rear = 0;
        }
        else
        {
            rear = (rear + 1) % capacity;
        }
        arr[rear] = element;
        size++;
        return true;
    }

    bool pop_back()
    {
        if (front != -1 && rear != -1)
        {
            if (front == rear)
            {
                front = -1;
                rear = -1;
            }
            else
            {
                rear = ((rear - 1) % capacity + capacity) % capacity;
            }
            size--;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool push_front(const T &element)
    {
        if (size == capacity)
        {
            int new_capacity = (capacity == 0) ? 1 : 2 * capacity;
            reserve(new_capacity);
        }
        if (front == -1 && rear == -1)
        {
            front = 0;
            rear = 0;
        }
        else
        {
            front = ((front - 1) % capacity + capacity) % capacity;
        }
        arr[front] = element;
        size++;
        return true;
    }

    bool pop_front()
    {
        if (front != -1 && rear != -1)
        {
            if (front == rear)
            {
                front = -1;
                rear = -1;
            }
            else
            {
                front = (front + 1) % capacity;
            }
            size--;
            return true;
        }
        else
        {
            return false;
        }
    }

    T getFront()
    {
        if (front != -1)
            return arr[front];
        else
            return T();
    }

    T back()
    {
        if (empty())
            return T();
        return arr[rear];
    }

    T &operator[](int index)
    {
        if (index >= size)
        {
            return defaultValue;
        }
        if (index >= 0)
            return arr[(front + index) % capacity];
        if (abs(index) > size)
            return defaultValue;

        return arr[((rear + index + 1) % capacity + capacity) % capacity];
    }

    bool empty()
    {
        return size == 0;
    }

    int getSize()
    {
        return size;
    }

    void resize(int n)
    {
        while (n < size)
        {
            pop_back();
        }
        if (n > size)
        {
            if (n > capacity)
                reserve(n);
            while (n > size)
            {
                push_back(T());
            }
        }
    }

    void resize(int n, const T &value)
    {
        while (n < size)
        {
            pop_back();
        }
        if (n > size)
        {
            if (n > capacity)
                reserve(n);
            while (n > size)
            {
                push_back(value);
            }
        }
    }

    void reserve(int n)
    {
        if (n > capacity)
        {
            T *realloc_arr = new T[n];
            if (front <= rear)
            {
                for (int i = 0; i < size; i++)
                {
                    realloc_arr[i] = arr[(front + i) % capacity];
                }
            }
            else
            {
                int index = 0;
                for (int i = front; i < capacity; i++)
                {
                    realloc_arr[index++] = arr[i];
                }
                for (int i = 0; i <= rear; i++)
                {
                    realloc_arr[index++] = arr[i];
                }
            }
            delete[] arr;
            arr = realloc_arr;
            capacity = n;
            front = 0;
            rear = size - 1;
        }
    }

    void shrink_to_fit()
    {
        if (empty())
        {
            delete[] arr;
            arr = nullptr;
            front = -1;
            rear = -1;
            capacity = 0;
            return;
        }

        T *realloc_arr = new T[size];
        if (front <= rear)
        {
            for (int i = 0; i < size; i++)
            {
                realloc_arr[i] = arr[(front + i) % capacity];
            }
        }
        else
        {
            int index = 0;
            for (int i = front; i < capacity; i++)
            {
                realloc_arr[index++] = arr[i];
            }
            for (int i = 0; i <= rear; i++)
            {
                realloc_arr[index++] = arr[i];
            }
        }

        delete[] arr;
        arr = realloc_arr;
        capacity = size;
        front = 0;
        rear = size - 1;
    }

    void clear()
    {
        for (int i = 0; i < size; i++)
        {
            arr[(front + i) % capacity] = T();
        }
        size = 0;
        front = -1;
        rear = -1;
    }

    int getCapacity()
    {
        return capacity;
    }
};

template <class T>
class RandomizedQueue
{
private:
    deque<T> dq;

public:
    int generateRandomNumber()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distr(0, dq.getSize() - 1);
        return distr(gen);
    }

    void enqueue(const T &element)
    {
        dq.push_back(element);
    }

    T dequeue()
    {
        if (dq.empty())
        {
            return T();
        }
        int randomNumber = generateRandomNumber();
        T randomElement = dq[randomNumber];
        if (randomNumber != dq.getSize() - 1)
            dq[randomNumber] = dq.back();
        dq.pop_back();
        return randomElement;
    }

    T sample()
    {
        if (dq.empty())
        {
            return T();
        }
        int randomNumber = generateRandomNumber();
        T randomElement = dq[randomNumber];
        return randomElement;
    }
};

int main()
{
    RandomizedQueue<T> x;
    int option;
    do
    {
        cin >> option;
        switch (option)
        {
        case 0:
        {
            exit(0);
        }
        case 1:
        {
            int val;
            cin >> val;
            x.enqueue(val);
            break;
        }
        case 2:
        {
            cout << x.dequeue() << "\n";
            break;
        }
        case 3:
        {
            cout << x.sample() << "\n";
            break;
        }
        default:
            cout << "Invalid" << "\n";
            break;
        }
    } while (option != 0);

    return 0;
}