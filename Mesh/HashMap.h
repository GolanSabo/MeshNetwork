#pragma once

#define TABLE_SIZE 211

// Hash node class template
template <typename K, typename V>
class HashNode {
public:
	HashNode(const K &key, const V &value) :
		key(key), value(value), next(NULL) {
	}

	K getKey() const {
		return key;
	}

	V getValue() const {
		return value;
	}

	void setValue(V value) {
		HashNode::value = value;
	}

	HashNode *getNext() const {
		return next;
	}

	void setNext(HashNode *next) {
		HashNode::next = next;
	}

private:
	// key-value pair
	K key;
	V value;
	// next bucket with the same key
	HashNode *next;
};

template <typename K>
struct KeyHash {
	unsigned long operator()(const K& key) const
	{
		return (long)key;
	}
};

// Hash map class template
template <typename K, typename V, typename F = KeyHash<K>>
class HashMap {
public:
	HashMap() {
		// construct zero initialized hash table of size
		table = new HashNode<K, V> *[TABLE_SIZE]();
	}

	~HashMap() {
		// destroy all buckets one by one
		for (int i = 0; i < TABLE_SIZE; ++i) {
			HashNode<K, V> *entry = table[i];
			while (entry != NULL) {
				HashNode<K, V> *prev = entry;
				entry = entry->getNext();
				delete prev;
			}
			table[i] = NULL;
		}
		// destroy the hash table
		delete[] table;
	}

	bool get(const K &key, V &value) {
		unsigned long hashValue = hashFunc(key);
		HashNode<K, V> *entry = table[hashValue];

		while (entry != NULL) {
			if (entry->getKey() == key) {
				value = entry->getValue();
				return true;
			}
			entry = entry->getNext();
		}
		return false;
	}

	void put(const K &key, const V &value) {
		unsigned long hashValue = hashFunc(key);
		HashNode<K, V> *prev = NULL;
		HashNode<K, V> *entry = table[hashValue];

		while (entry != NULL && entry->getKey() != key) {
			prev = entry;
			entry = entry->getNext();
		}

		if (entry == NULL) {
			entry = new HashNode<K, V>(key, value);
			if (prev == NULL) {
				// insert as first bucket
				table[hashValue] = entry;
			}
			else {
				prev->setNext(entry);
			}
		}
		else {
			// just update the value
			entry->setValue(value);
		}
	}

	void remove(const K &key) {
		unsigned long hashValue = hashFunc(key);
		HashNode<K, V> *prev = NULL;
		HashNode<K, V> *entry = table[hashValue];

		while (entry != NULL && entry->getKey() != key) {
			prev = entry;
			entry = entry->getNext();
		}

		if (entry == NULL) {
			// key not found
			return;
		}
		else {
			if (prev == NULL) {
				// remove first bucket of the list
				table[hashValue] = entry->getNext();
			}
			else {
				prev->setNext(entry->getNext());
			}
			delete entry;
		}
	}

private:
	// hash table
	HashNode<K, V> **table;
	F hashFunc;
};





























///* Handle association */
//template<typename hash, typename map>
//class HashType {
//public:
//	HashType() { reset(); }
//
//	HashType(hash code, map value) :hashCode(code), mappedValue(value) {}
//
//	void reset() { hashCode = 0; mappedValue = 0; }
//	hash getHash() { return hashCode; }
//	void setHash(hash code) { hashCode = code; }
//	map getValue() { return mappedValue; }
//	void setValue(map value) { mappedValue = value; }
//
//	HashType& operator()(hash code, map value) {
//		setHash(code);
//		setValue(value);
//	}
//private:
//	hash hashCode;
//	map mappedValue;
//};
//
///*
//Handle indexing and searches
//TODO - extend API
//*/
//template<typename hash, typename map>
//class HashMap {
//public:
//	HashMap(HashType<hash, map>* newMap, byte newSize) {
//		counter = 0;
//		hashMap = newMap;
//		size = newSize;
//		for (byte i = 0; i < size; i++) {
//			hashMap[i].reset();
//		}
//	}
//
//	HashType<hash, map>& operator[](int x) {
//		++counter;
//		return hashMap[x];
//	}
//
//	int getIndexOf(hash key) {
//		for (byte i = 0; i < size; i++) {
//			if (hashMap[i].getHash() == key) {
//				return i;
//			}
//			else {
//				return -1;
//			}
//		}
//		return -1;
//	}
//	map getValueOf(hash key) {
//		for (byte i = 0; i<size; i++) {
//			if (hashMap[i].getHash() == key) {
//				return hashMap[i].getValue();
//			}
//		}
//	}
//
//	//void debug() {
//	//	for (byte i = 0; i<size; i++) {
//	//		Serial.print(hashMap[i].getHash());
//	//		Serial.print(" - ");
//	//		Serial.println(hashMap[i].getValue());
//	//	}
//	//}
//	int getSize(){
//		return size;
//	}
//	int getCounter() {
//		return counter;
//	}
//
//private:
//	HashType<hash, map>* hashMap;
//	byte size;
//	byte counter;
//};
//
//


