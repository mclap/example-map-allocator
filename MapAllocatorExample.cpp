#include <map>
#include <compare>
#include <iostream>
#include <unordered_map>

#include <stdlib.h>
#include <string.h>

const char *shorter(const char *value)
{
    const char *p = strrchr(value, '/');
    return p != nullptr ? p + 1 : value;
}

#define P(v) do { std::cout << shorter(__FILE__) << ":" << __LINE__ << ": " << #v << "=" << v << std::endl; } while(0);

struct Key
{
    int key;

    Key() = default;
    constexpr Key(int v ) : key(v) {}

    auto operator<=>(const Key&) const = default;
};

struct Value
{
    int value;

    Value() = default;
    constexpr Value(int v) : value(v) {}

    auto operator<=>(const Value&) const = default;
};

void example1()
{
    P("example1");

    using Type = std::map<Key, Value>;

    Type v;
    v[1]=2;
}


template<typename T>
struct MyAllocator
{
    using value_type = T;
    using size_type = size_t;

    constexpr MyAllocator() noexcept {}

    template< class U >
    constexpr MyAllocator( const MyAllocator<U>& other ) noexcept {}

    void deallocate(T *ptr, size_type /*count*/)
    {
        P(ptr);
        free(ptr);
    }

    T* allocate(size_type count)
    {
        auto size = sizeof(T) * count;
        P(size);

        auto ptr = reinterpret_cast<T*>(malloc(size));
        P(ptr);
        return ptr;
    }
};

template<typename K, typename V>
using MyMap = std::map<K, V, std::less<K>, MyAllocator<std::pair<const K, V>>>;

void example2()
{
    using Type = MyMap<Key, Value>;

    P("example2");

    Type v;
    v[1]=2;

    P(sizeof(MyAllocator<std::pair<Key, Value>>::value_type));
    P(sizeof(std::pair<Key, Value>));
    P(sizeof(std::map<Key, Value>::node_type));
    P(sizeof(std::multimap<Key, Value>::node_type));
}

struct MyHash
{
    std::size_t operator()(const Key& v) const noexcept
    {
        return std::hash<int>{}(v.key);
    }
};

struct MyEqual
{
    bool operator()(const Key& lhs, const Key& rhs) const
    {
        return lhs.key == rhs.key;
    }
};

template<typename K, typename V>
using MyUnorderedMap = std::unordered_map<K, V, MyHash, MyEqual, MyAllocator<std::pair<const K, V>>>;

void example3()
{
    using Type = MyUnorderedMap<Key, Value>;

    P("example3");

    Type v;

    P(sizeof(v));
    v[1]=2;

    P(v.size());

    P(sizeof(decltype(v)::allocator_type::value_type));
    P(sizeof(MyAllocator<std::pair<Key, Value>>::value_type));
    P(sizeof(std::pair<Key, Value>));
    P(sizeof(decltype(v)::node_type));
}

int main()
{

    example1();
    example2();
    example3();

    return 0;
}
