#ifndef MAP_H
#define MAP_H
#include <ostream>
#include <cstddef>
#include <initializer_list>
#include <utility>

namespace CS246E {
	using std::pair;
	template<typename K, typename V> class map {
		const size_t startSize = 1;
		class Node {
			pair<K, V> p;
			Node * left;
			Node * right;
			Node * father;
			Node(pair<K, V> p, Node * father, Node * left = nullptr, Node * right = nullptr);
			Node(const Node & other, Node * father);
			friend class map;
		};
		
		class iterator {
			Node * p;
			iterator(Node * p): p{p} {}
			public:
				bool operator!=(const iterator & other) const { return p != other.p; }
				pair<K, V> & operator*() const { return p->p; }
				iterator & operator++() {
					if (p->right) {
						p = p->right;
						while (p->left) p = p->left; 
					} else if (p->father == nullptr) {
						p = nullptr;	
					} else if (p == p->father->left) {
						p = p->father;
					} else if (p == p->father->right) {
						while (p->father && p == p->father->right) {
							p = p->father;
						}
						if (p->father == nullptr) p = nullptr;
						else p = p->father;
					}
					return *this;
				}
				friend class map;
		};
		
		size_t n;
		Node * theMap;
		Node * bg;
				
		public:
			map();
			map(const map & other);
			map(map<K, V> && other);
			map(std::initializer_list<pair<K, V>> init);
			map<K, V> & operator=(const map<K, V> & other);
			map<K, V> & operator=(map<K, V> && other);
			V & operator[](const K & key);
			V & at(const K & key);
			iterator begin();
			iterator end();
			bool empty() const;
			size_t size() const;
			size_t erase(const K & key);
			void clear();
			size_t count(const K & key) const;
			~map();
		private:
			void swap(map<K, V> & other);
			Node * findBegin(Node * cur);
			V & traverse(Node * cur, const K & key, bool set, bool & success);
			size_t countRecursion(const Node * cur, const K & key) const;
			Node * eraseRecursion(Node * cur, const K & key, bool & flag);
			void destroyItems(Node * cur);
	};
	
	////////////// implementation for functions in Node ////////////////////
	
	template<typename K, typename V> map<K, V>::Node::Node(pair<K, V> p, Node * father, Node * left, Node * right): p{p}, left{left}, right{right}, father{father} {};
	
	template<typename K, typename V> map<K, V>::Node::Node(const Node & other, Node * father): p{other.p}, father{father} {
		left = other.left ? new Node(*(other.left), this) : nullptr;
		right = other.right ? new Node(*(other.right), this) : nullptr;  
	}
	
	////////////// implemantation for function in map //////////////////////
	
	template<typename K, typename V> map<K, V>::map(): n{0}, theMap{nullptr}, bg{nullptr} {}
	
	template<typename K, typename V> map<K, V>::map(const map<K, V> & other): n{other.n}, theMap{nullptr} {
		if (other.theMap) theMap = new Node(*(other.theMap), nullptr);
		else theMap = nullptr;
		bg = findBegin(theMap);
	}
	
	template<typename K, typename V> map<K, V>::map(map<K, V> && other): n{other.n}, theMap{other.theMap}, bg{other.bg} {
		other.theMap = nullptr;
		other.bg = nullptr;
	}
	
	template<typename K, typename V> map<K, V>::map(std::initializer_list<pair<K, V>> init): n{0}, theMap{nullptr} {
		for (auto & i : init) (*this)[i.first] = i.second;
		bg = findBegin(theMap);
	}
	
	template<typename K, typename V> map<K, V> & map<K, V>::operator=(const map<K, V> & other) {
		map<K, V> tmp = other;
		swap(tmp);
		return *this;
	}
	
	template<typename K, typename V> map<K, V> & map<K, V>::operator=(map<K, V> && other) {
		swap(other);
		return *this;
	}
	
	template<typename K, typename V> V & map<K, V>::operator[](const K & key) {
		if (theMap == nullptr) {
			theMap = new Node(pair<K, V>(key, V{}), nullptr);
			++n;
			bg = theMap;
			return theMap->p.second;
		} else {
			bool success = false;
			V & tmp = traverse(theMap, key, true, success);
			bg = findBegin(theMap);
			return tmp;
		}
	} 
	
	template<typename K, typename V> V & map<K, V>::at(const K & key) {
		if (theMap == nullptr) {
			throw 0;
		} else {
			bool success = false;
			V & tmp = traverse(theMap, key, false, success);
			if (success) return tmp;
			else throw 0;
		}
	}
		
	template<typename K, typename V> typename map<K, V>::iterator map<K, V>::begin() { return iterator{bg}; }
	
	template<typename K, typename V> typename map<K, V>::iterator map<K, V>::end() { return nullptr; }
	
	template<typename K, typename V> bool map<K, V>::empty() const { return !n; }
	
	template<typename K, typename V> size_t map<K, V>::size() const { return n; }
	
	template<typename K, typename V> size_t map<K, V>::erase(const K & key) {
		bool flag = false;
		theMap = eraseRecursion(theMap, key, flag);
		bg = findBegin(theMap);
		return flag; 
	}
	
	template<typename K, typename V> void map<K, V>::clear() {
		destroyItems(theMap);
		theMap = nullptr;
		bg = nullptr;
		n = 0;
	}
	
	template<typename K, typename V> size_t map<K, V>::count(const K & key) const {
		return countRecursion(theMap, key);
	}
	
	
	template<typename K, typename V> map<K, V>::~map() { this->clear(); }

	template<typename K, typename V> void map<K, V>::swap(map<K, V> & other) {
		using std::swap;
		swap(theMap, other.theMap);
		swap(n, other.n);
		swap(bg, other.bg);
	}
	
	template<typename K, typename V> typename map<K, V>::Node * map<K, V>::findBegin(Node * cur) {
		if (cur == nullptr) return cur;
		else if (cur->left) return findBegin(cur->left);
		else return cur; 
	}
	
	template<typename K, typename V> V & map<K, V>::traverse(Node * cur, const K & key, bool set, bool & success) {
		if (key == cur->p.first) {
			success = true;
			return cur->p.second;
		} else if (key > cur->p.first) {
			if (cur->right) return traverse(cur->right, key, set, success);
			else if (set)  {
				success = true;
				cur->right = new Node(pair<K, V>(key, V{}), cur);
				++n;
				return cur->right->p.second;
			} else {
				success = false;
				return cur->p.second;	
			}
		} else {
			if (cur->left) return traverse(cur->left, key, set, success);
			else if (set) {
				success = true;
				cur->left = new Node(pair<K, V>(key, V{}), cur);
				++n;
				return cur->left->p.second;
			} else {
				success = false;
				return cur->p.second;
			}
		}
	}
	
	template<typename K, typename V> size_t map<K, V>::countRecursion(const Node * cur, const K & key) const {
		if (cur == nullptr) return 0;
		else if (key == cur->p.first) return 1;
		else if (key > cur->p.first) return countRecursion(cur->right, key);
		else return countRecursion(cur->left, key);
	}	

	template<typename K, typename V> typename map<K, V>::Node * map<K, V>::eraseRecursion(Node * cur, const K & key, bool & flag)  {
		if (cur == nullptr) {
			flag = false;
			return cur;	
		} else if (key == cur->p.first) {
			flag = true;
			--n;
			if (cur->left == nullptr) {
				Node * tmp = cur->right;
				if (tmp) tmp->father = cur->father;
				delete cur;
				return tmp;
			} else if (cur->right == nullptr) {
				Node * tmp = cur->left;
				if (tmp) tmp->father = cur->father;
				delete cur;
				return tmp;
			} else {
				Node * joint = findBegin(cur->right);
				cur->p = joint->p;
				Node * tmp = joint->father;
				if (joint == tmp->left) tmp->left = nullptr;
				else if (joint == tmp->right) tmp->right = nullptr;
				delete joint;
				return cur;
			}
		} else if (key > cur->p.first) {
			cur->right =  eraseRecursion(cur->right, key, flag);
			return cur;
		} else {
			cur->left = eraseRecursion(cur->left, key, flag);
			return cur;
		}
	}
	
	template<typename K, typename V> void map<K, V>::destroyItems(Node * cur) {
		if (cur == nullptr) return;
		destroyItems(cur->left);
		destroyItems(cur->right);
		delete cur;
	}
}

#endif
