#pragma once
#include<iostream>
#include<vector>
#include<set>
using namespace std;

template<class K>
struct HashKey
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

template<>
struct HashKey<string>
{
	int operator()(const string& s)
	{
		int val = 0;
		for (int i = 0; i < s.size(); i++)
		{
			val = val * 131 + s[i];
		}
		return val;
	}
};

namespace Link_Hash
{

	template<class T>
	struct Hash_Date
	{
		T date;
		Hash_Date<T>* next = nullptr;
	};

	template<class K, class T, class KeyOft, class Hashfunc>
	class Hash_Table;

	template<class K, class T, class Ref, class Ptr,class KeyOft,class HashFunc>
	struct HS_Iterator
	{
		Hash_Table<K, T, KeyOft, HashFunc>* _p;
		typedef Hash_Date<T> Node;
		typedef HS_Iterator<K, T, Ref, Ptr, KeyOft, HashFunc> Seft;

		Node* _node;

		HS_Iterator(Node*node, Hash_Table<K, T, KeyOft, HashFunc>*p)
		{
			_node = node;
			_p = p;
		}
		Ptr operator*()
		{
			return _node->date;
		}

		Ref operator->()
		{
			return &_node->date;
		}

		Seft operator++()
		{
			if (_node->next)
			{
				_node = _node->next;
			}
			else
			{
				KeyOft kt;
				HashFunc hs;
				size_t index = hs(kt(_node->date))%_p->_tables.size();
				index++;
				while (index<_p->_tables.size())
				{
					if (_p->_tables[index] != nullptr)
					{
						break;
					}
					else
					{
						index++;
					}
				}
				if (index == _p->_tables.size())
				{
					_node = nullptr;
				}
				else
				{
					_node = _p->_tables[index];
				}
			}
			return *this;
		}
		bool operator!=(const Seft&b)
		{
			return _node != b._node;
		}
		bool operator==(const Seft& b)
		{
			return _node != b._node;
		}
	};


	template<class K, class T, class KeyOft,class Hashfunc>
	class Hash_Table
	{
		typedef Hash_Date<T> Node;
		template<class K, class T, class Ref, class Ptr, class KeyOft, class HashFunc>
		friend struct HS_Iterator;
	public:
		typedef  HS_Iterator<K, T, T*, T&, KeyOft, Hashfunc> iterator;
		Hashfunc hf;
		KeyOft kt;

		iterator begin()
		{
			for (int i = 0; i < _tables.size(); i++)
			{
				if (_tables[i] != nullptr)
				{
					return iterator(_tables[i], this);
				}
			}
			return end();
		}
		iterator end()
		{
			return iterator(nullptr, this);
		}
		Hash_Table()
		{
			_tables.resize(10, nullptr);
		}
		~Hash_Table()
		{
			for (int i = 0; i < _tables.size(); i++)
			{
				Node* cur = _tables[i];
				while (cur)
				{
					Node* next = cur->next;
					delete cur;
					cur = next;
				}
				_tables[i] = nullptr;
			}
		}
		iterator Find(const K& key)
		{
			size_t start = hf(key) % _tables.size();
			Node* cur = _tables[start];
			while (cur)
			{
				if (kt(cur->date) == key)
				{
					return iterator(cur,this);
				}
				cur = cur->next;
			}
			return end();
		}
		pair<iterator,bool> insert(const T&date)
		{
			iterator it = Find(kt(date));
			if (it ==end())
				return make_pair(it,false);

			if (_n == _tables.size())
			{
				size_t newsize = _tables.size() * 2;
				vector<Node*>newnode;
				newnode.resize(newsize);

				for (int i = 0; i < _tables.size(); i++)
				{
					if (_tables[i])
					{
						Node* cur = _tables[i];
						while (cur)
						{
							size_t val = hf(kt(cur->date)) % newsize;
							cur->next = newnode[val];
							newnode[val] = cur;
							cur = cur->next;
						}
						_tables[i] = nullptr;
					}
				}
				_tables.swap(newnode);
			}

			size_t start = hf(kt(date)) % _tables.size();
			Node* newnode = new Node;
			newnode->date = date;
			newnode->next = nullptr;

			Node* cur = _tables[start];
			if (!cur)
			{
				_tables[start] = newnode;
			}
			else
			{
				_tables[start] = newnode;
				newnode->next = cur;
			}
			_n++;
			return make_pair(iterator(newnode,this), true);
		}
		bool Erase(const K& key)
		{
			Node* ret = Find(key);
			if (ret == nullptr)
			{
				return false;
			}
			else
			{
				_n--;
				size_t val = hf(key) % _tables.size();
				Node* cur = _tables[val];
				if (kt(cur->date) == key)
				{
					_tables[val] = cur->next;
					return true;
				}
				while (cur && cur->next)
				{
					if (kt(cur->next->date) == key)
					{
						if (cur->next->next)
						{
							cur->next = cur->next->next;
						}
						else
						{
							cur->next = nullptr;
						}
						break;
					}
					cur = cur->next;
				}
				return true;
			}
		}
	private:
		vector<Node*>_tables;
		size_t _n = 0;
	};
}