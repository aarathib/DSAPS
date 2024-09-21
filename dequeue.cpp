#include <iostream>
#include <fstream>

using namespace std;

class Complex
{
private:
    double x = 0.0;
    double y = 0.0;

public:
    friend ostream &operator<<(ostream &output, const Complex &D)
    {
        output << D.x << "i + " << D.y << "j";
        return output;
    }
    friend istream &operator>>(istream &input, Complex &D)
    {
        input >> D.x >> D.y;
        return input;
    }
};

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
        if (arr == nullptr)
            return false;
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
        if (arr == nullptr)
            return false;
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

int main()
{
    deque<T> x;
    int n;
    T val;
    ifstream file("input00.txt");
    if (!file.is_open())
    {
        cout << "Error while opening file" << "\n";
        return 1;
    }

    int option;
    do
    {
        file >> option;
        if (option == 0)
            exit(0);
        switch (option)
        {
        case 1:
        {
            x = deque<T>();
            break;
        }
        case 2:
        {
            file >> n;
            x = deque<T>(n);
            break;
        }
        case 3:
        {
            file >> n;
            file >> val;
            x = deque<T>(n, val);
            break;
        }
        case 4:
        {
            file >> val;
            cout << boolalpha << x.push_back(val) << "\n";
            break;
        }
        case 5:
        {
            cout << boolalpha << x.pop_back() << "\n";
            break;
        }
        case 6:
        {
            file >> val;
            cout << boolalpha << x.push_front(val) << "\n";
            break;
        }
        case 7:
        {
            cout << boolalpha << x.pop_front() << "\n";
            break;
        }
        case 8:
        {
            cout << x.getFront() << "\n";
            break;
        }
        case 9:
        {
            cout << x.back() << "\n";
            break;
        }
        case 10:
        {
            file >> n;
            cout << x[n] << "\n";
            break;
        }
        case 11:
        {
            cout << boolalpha << x.empty() << "\n";
            break;
        }
        case 12:
        {
            cout << x.getSize() << "\n";
            break;
        }
        case 13:
        {
            file >> n;
            x.resize(n);
            break;
        }
        case 14:
        {
            file >> n;
            file >> val;
            x.resize(n, val);
            break;
        }
        case 15:
        {
            file >> n;
            x.reserve(n);
            break;
        }
        case 16:
        {
            x.shrink_to_fit();
            break;
        }
        case 17:
        {
            x.clear();
            break;
        }
        case 18:
        {
            cout << x.getCapacity() << "\n";
            break;
        }
        default:
            cout << "Invalid" << "\n";
            break;
        }
    } while (option != 0);

    return 0;
}