#ifndef POOL_H
#define POOL_H
//////////////////
// POOL - a collection of template with accessor functions
/////////////////
class IPool
{
    public:
        virtual ~IPool(){}
};

template <typename T> 
class Pool : public IPool {
    
    private:
        std::vector<T> Collection;
    
    public:
        Pool(int size = 100)
        {
            Collection.resize(size);
        }
        virtual ~Pool(){};

        bool IsEmpty()
        {
            return Collection.empty();
        }

        int GetSize() const
        {
            return static_cast<int>(Collection.size());
        }

        void Resize(int n)
        {
            Collection.resize(n);
        }

        void Clear()
        {
            Collection.clear();
        }

        void Add(T obj)
        {
            Collection.push_back(obj);
        }

        void Set(int index, T obj)
        {
            Collection[index] = obj;
        }

        T& Get(int index)
        {
            return static_cast<T&>(Collection[index]);
        }

        T& operator [](unsigned int index)
        {
            return Collection[index];
        }

};
#endif