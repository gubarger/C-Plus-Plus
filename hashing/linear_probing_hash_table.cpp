/**
 * @file
 * @author [achance6](https://github.com/achance6)
 * @author [Krishna Vedala](https://github.com/kvedala)
 * @brief Storage mechanism using [linear probing
 * hash](https://en.wikipedia.org/wiki/Linear_probing) keys.
 * @note The implementation can be optimized by using OOP style.
 */

#include <cassert>
#include <iostream>
#include <vector>

/**
 * @addtogroup open_addressing Open Addressing
 * @{
 * @namespace linear_probing
 * @brief An implementation of hash table using [linear
 * probing](https://en.wikipedia.org/wiki/Linear_probing) algorithm.
 */
namespace linear_probing {
/**
 * @class
 * @brief Linear Probing Hash Table Class
 */
class linear_robing_hash_table {
 public:
    /**
     * @public
     * @brief Hash table constructor
     * @param stCap initial table capacity
     */
    linear_robing_hash_table(int stCap);

    /**
     * @public
     * @brief Checks if a key exists in the table
     * @param key The key to check for
     * @return true if the key exists, false otherwise
     */
    bool contains(int key);

    /**
     * @public
     * @brief Inserts a key into the table
     * @param key the key to insert
     */
    void insert(int key);

    /**
     * @public
     * @brief Deletes a key from the table
     * @param key the key to delete
     * @return true if the key was deleted, false if not found
     */
    bool remove(int key);

    /**
     * @public
     * @brief Returns the current size of the table
     * @return the number of elements in the table
     */
    int getSize() const { return size; }

    /**
     * @public
     * @brief Checks if the table is empty
     * @return true if the table is empty
     */
    bool isEmpty() const { return size == 0; }

    /**
     * @public
     * @brief Outputs the table contents (for debugging)
     */
    void print();

    /**
     * @public
     * @brief Hash a key. Uses the STL library's `std::hash()` function.
     * @param key the key to hash
     * @return the hash value of the key
     */
    size_t hash(int key) const;

 private:
    /**
     * @struct
     * @private
     * @brief Node object that holds key
     */
    struct Entry {
        explicit Entry(int key = empty) : key(key) {}
        int key;  ///< key value
    };

    /**
     * @private
     * @brief Performs a linear scan to find the index
     * @param key the key to search for
     * @param search true search for an existing key, false - search for
     * a free cell
     * @return the index of the cell found, or -1 if not found
     */
    int findIndex(int key, bool search) const;

    /**
     * @private
     * @brief Finds the index of an existing key
     * @param key the key to find
     * @return index of the key, or -1 if not found
     */
    int findKey(int key) const { return findIndex(key, true); }

    /**
     * @private
     * @brief Finds a free slot for insertion
     * @param key the key to insert (used for hash calculation)
     * @return index of free slot, or -1 if table is full
     */
    int findFreeSlot(int key) const { return findIndex(key, false); }

    /**
     * @private
     * @brief Rehashes the table when the load factor is exceeded
     */
    void rehash();

 private:
    static const int empty = -1;  ///< constant for empty cells
    static const int tomb = -2;   ///< constant for deleted elements

    int cap;                   ///< total table capacity
    int size;                  ///< current number of elements
    bool isRehashing;          ///< rehashing process flag
    std::vector<Entry> table;  ///< array for storing elements
};

linear_robing_hash_table::linear_robing_hash_table(int size)
    : cap(size), size(0), isRehashing(false) {
    table.resize(cap, Entry(empty));
}

void linear_robing_hash_table::print() {
    std::cout << "Hash table (size: " << size << " | " << cap << ")\n";

    for (int i = 0; i < cap; i++) {
        std::cout << "[" << i << "]: ";

        if (table[i].key == empty) {
            std::cout << "EMPTY";
        } else if (table[i].key == tomb) {
            std::cout << "TOMB";
        } else {
            std::cout << table[i].key;
        }

        std::cout << "\n";
    }
}

size_t linear_robing_hash_table::hash(int key) const {
    std::hash<int> hash;
    return hash(key);
}

int linear_robing_hash_table::findIndex(int key, bool search) const {
    int curHash = static_cast<int>(hash(key) % cap);

    for (int i = 0; i < cap; i++) {
        int index = (curHash + i) % cap;

        if (search && table[index].key == empty) {
            return -1;
        }

        if (search) {
            if (table[index].key == empty) {
                return -1;
            }
            if (table[index].key == key) {
                return index;
            }
        } else {
            if (table[index].key == empty || table[index].key == tomb) {
                return index;
            }
            if (table[index].key == key) {
                return index;
            }
        }
    }

    return -1;
}

bool linear_robing_hash_table::contains(int key) { return findKey(key) != -1; }

void linear_robing_hash_table::insert(int key) {
    if (contains(key)) {
        return;  // The key already exists
    }

    int index = findFreeSlot(key);
    if (index == -1) {
        rehash();
        index = findFreeSlot(key);

        assert(index != -1 && "Table should have free space after rehash");
    }

    table[index].key = key;

    // Load factor greater than 0.5 causes resizing
    if (static_cast<double>(size++) / cap >= 0.5 && !isRehashing) {
        rehash();
    }
}

void linear_robing_hash_table::rehash() {
    // Necessary so wall of add info isn't printed all at once
    isRehashing = true;
    int oldSize = cap;
    std::vector<Entry> oldTable = std::move(table);

    // Really this should use the next prime number greater than cap *
    // 2
    cap *= 2;
    table = std::vector<Entry>(cap, Entry(empty));
    size = 0;

    for (int i = 0; i < oldSize; i++) {
        if (oldTable[i].key != empty && oldTable[i].key != tomb) {
            insert(oldTable[i].key);
        }
    }

    isRehashing = false;
}

bool linear_robing_hash_table::remove(int key) {
    int index = findKey(key);
    if (index != -1) {
        table[index].key = tomb;
        size--;
        return true;
    }
    return false;
}
}  // namespace linear_probing
/**
 * @}
 */

namespace test_linear_probing {
/**To test hash table functions
 * @returns None
 */
bool test() {
    std::cout << "===Start tests \"linear probing hash table\"===\n";

    {  // insert and find
        linear_probing::linear_robing_hash_table ht(10);

        ht.insert(5);
        ht.insert(15);

        assert(ht.contains(5) == true);
        assert(ht.contains(15) == true);
        assert(ht.contains(999) == false);

        std::cout << "TEST 1 | +insert and find | PASSED\n";
    }

    {  // contains
        linear_probing::linear_robing_hash_table ht(10);

        assert(ht.contains(5) == false);
        ht.insert(5);
        assert(ht.contains(5) == true);

        std::cout << "TEST 2 | +contains | PASSED\n";
    }

    {  // collision
        linear_probing::linear_robing_hash_table ht(5);

        ht.insert(0);
        ht.insert(5);
        ht.insert(10);
        ht.insert(15);

        assert(ht.contains(0) == true);
        assert(ht.contains(5) == true);
        assert(ht.contains(10) == true);
        assert(ht.contains(15) == true);

        std::cout << "TEST 3 | +collision | PASSED\n";
    }

    {  // remove
        linear_probing::linear_robing_hash_table ht(10);

        ht.insert(5);
        ht.insert(15);

        assert(ht.contains(5) == true);

        bool remove = ht.remove(5);

        assert(remove == true);
        assert(ht.contains(5) == false);
        assert(ht.contains(15) == true);

        remove = ht.remove(100000000);

        assert(remove == false);

        std::cout << "TEST 4 | +remove | PASSED\n";
    }

    {  // rehash
        linear_probing::linear_robing_hash_table ht(3);

        ht.insert(1);
        ht.insert(2);
        ht.insert(3);

        assert(ht.contains(1) == true);
        assert(ht.contains(2) == true);
        assert(ht.contains(3) == true);

        std::cout << "TEST 5 | +rehash | PASSED\n";
    }

    {  // hash
        linear_probing::linear_robing_hash_table ht(10);

        size_t hash1 = ht.hash(5);
        size_t hash2 = ht.hash(15);
        size_t hash3 = ht.hash(5);

        assert(hash1 == hash3);
        assert(hash1 != hash2);

        std::cout << "TEST 6 | +hash | PASSED\n";
    }

    {  // empty
        linear_probing::linear_robing_hash_table ht(5);

        assert(ht.isEmpty() == true);
        assert(ht.getSize() == 0);
        assert(ht.contains(100) == false);
        assert(ht.remove(42) == false);

        std::cout << "TEST 6 | +empty | PASSED\n";
    }

    {  // tomb
        linear_probing::linear_robing_hash_table ht(5);

        ht.insert(5);
        ht.insert(10);
        ht.remove(5);

        ht.insert(15);

        assert(ht.contains(15) == true);
        assert(ht.contains(10) == true);
        assert(ht.contains(5) == false);

        std::cout << "TEST 6 | +tomb | PASSED\n";
    }

    std::cout << "===ALL TESTS PASSED===\n";

    return 0;
}
}  // namespace test_linear_probing

/** Main function
 * @returns 0 on success
 */
int main() {
    test_linear_probing::test();
    return 0;
}
