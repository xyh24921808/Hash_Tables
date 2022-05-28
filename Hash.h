#pragma once
#include<iostream>
#include<vector>
#include<set>
using namespace std;

namespace Coles_Hash
{
	enum Status
	{
		EMPTY,
		EXITS,
		DELETE,
	};

	template<class K, class V>
	struct Hash_Date
	{
		pair<K, V>_kv;
		Status _st = EMPTY;
	};

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
		int operator()(string& s)
		{
			int val = 0;
			for (int i = 0; i < s.size(); i++)
			{
				val = val * 131 + s[i];
			}
			return val;
		}
	};



	template<class K, class V, class Hashfunc = HashKey<K>>
	class Hash_Table
	{

	public:
		Hashfunc hf;
		Hash_Table()
		{
			_tables.resize(10);
		}
		Hash_Date<K, V>* Find(const K& key)
		{
			size_t start = hf(key) % _tables.size();
			size_t i = 0;
			size_t index = start;
			while (_tables[index]._st != EMPTY)
			{
				if (_tables[index]._kv.first == key && _tables[index]._st == EXITS)
				{
					return &_tables[index];
				}
				i++;
				index = start + i;
				index %= _tables.size();
			}
			return nullptr;
		}
		bool insert(const pair<K, V>& val)
		{
			Hash_Date<K, V>* ret = Find(val.first);
			if (ret != nullptr)
				return false;
			if (_n * 10 / _tables.size() >= 7)
			{
				size_t newsize = _tables.size() * 2;
				Hash_Table<K, V, Hashfunc> newHT;
				newHT._tables.resize(newsize);
				for (size_t i = 0; i < _tables.size(); i++)
				{
					if (_tables[i]._st == EXITS)
					{
						newHT.insert(_tables[i]._kv);
					}
				}
				_tables.swap(newHT._tables);
			}

			size_t start = hf(val.first) % _tables.size();
			size_t i = 0;
			size_t index = start;
			while (_tables[index]._st == EXITS)
			{
				i++;
				index = start + i * i;
				index %= _tables.size();
			}

			_tables[index]._kv = val;
			_tables[index]._st = EXITS;
			_n++;
			return true;
		}
		bool erase(const K& key)
		{
			Hash_Date<K, V>* ret = Find(key);
			if (ret == nullptr)
			{
				return false;
			}
			else
			{
				_n--;
				ret->_st = DELETE;
				return true;
			}
		}
	private:
		vector<Hash_Date<K, V>>_tables;
		size_t _n = 0;
	};
}

namespace Link_Hash
{

	template<class K, class V>
	struct Hash_Date
	{
		pair<K, V>_kv;
		Hash_Date<K,V>* next = nullptr;
	};

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
		int operator()(string& s)
		{
			int val = 0;
			for (int i = 0; i < s.size(); i++)
			{
				val = val * 131 + s[i];
			}
			return val;
		}
	};

	template<class K, class V, class Hashfunc = HashKey<K>>
	class Hash_Table
	{
		typedef Hash_Date<K, V> Node;
	public:
		Hashfunc hf;
		Hash_Table()
		{
			_tables.resize(10,nullptr);
		}
		Node* Find(const K& key)
		{
			size_t start = hf(key) % _tables.size();
			Node* cur = _tables[start];
			while (cur)
			{
				if (cur->_kv.first == key)
				{
					return cur;
				}
				cur = cur->next;
			}
			return nullptr;
		}
		bool insert(const pair<K, V>& val)
		{
			Node* ret = Find(val.first);
			if (ret != nullptr)
				return false;
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
							size_t val = hf(cur->_kv.first) % newsize;
							cur->next = newnode[val];
							newnode[val] = cur;
							cur = cur->next;
						}
						_tables[i] = nullptr;
					}
				}
				_tables.swap(newnode);
			}

			size_t start = hf(val.first) % _tables.size();
			Node* newnode = new Node;
			newnode->_kv = val;
			newnode->next = nullptr;

			Node*cur= _tables[start];
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
			return true;
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
				if (cur->_kv.first == key)
				{
					_tables[val] = cur->next;
					return true;
				}
				while (cur&&cur->next)
				{
					if (cur->next->_kv.first == key)
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
