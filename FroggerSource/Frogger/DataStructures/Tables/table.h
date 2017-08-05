// -----------------------------------------------------------------
// This is the header for table class template.
// -----------------------------------------------------------------
#pragma once

#include <vector>
using namespace std;

// ----------------------------------------------------------
// This structure represents an entry in a table.
//
// Version 3.0
// ----------------------------------------------------------
template <class T> struct Record
{
	T* rec;

	Record(T* r) { rec = r; }
	virtual bool equals(T*) =0;
	virtual bool matches(T*) =0;
	virtual bool isAddable() =0;
};

// ----------------------------------------------------------
// This class represents a collection of records.
//
// Version 4.2
// ----------------------------------------------------------
template <class T> class Table : protected vector<Record<T>*>
{
private:
	typedef vector<Record<T>*> super;
public:
	// ----------------------------------------------------------
	// This function adds record to the table.
	// @rec: The record to add.
	//
	// Version 4.2
	// ----------------------------------------------------------
	void add(Record<T>* r)
	{
		if (!r->isAddable())
			return;

		if (!isDefined(r->rec))
			push_back(r);
	}

	// ----------------------------------------------------------
	// This function determines if the record already exists in 
	// the table.
	// @rec: The record in question.
	//
	// Version 4.2
	// ----------------------------------------------------------
	bool isDefined(T* rec)
	{
		for (Record<T>* r : *this)
		{
			if (r->equals(rec))
				return true;
		}
		return false;
	}

	// ----------------------------------------------------------
	// This function determines if the record matches a defined
	// record
	// @rec: The record in question.
	//
	// Version 4.2
	// ----------------------------------------------------------
	bool matchExists(T* rec)
	{
		for (Record<T>* r : *this)
		{
			if (r->matches(rec))
				return true;
		}
		return false;
	}

	// ----------------------------------------------------------
	// This function returns the number of defined records that
	// match the desired record.
	// @rec: The record in question.
	//
	// Version 4.2
	// ----------------------------------------------------------
	int getNumberOfMatches(T* rec)
	{
		int matches = 0;
		for (Record<T>* r : *this)
		{
			if (r->matches(rec))
				matches++;
		}
		return matches;
	}

	// ----------------------------------------------------------
	// This function returns the first matching record in the 
	// table or NULL if no match found.
	// @rec: The record in question.
	//
	// Version 4.2
	// ----------------------------------------------------------
	T* getFirstMatch(T* rec)
	{
		for (Record<T>* r : *this)
		{
			if (r->matches(rec))
				return r->rec;
		}
		return NULL;
	}

	int size() { return ((super*)this)->size(); }
	T* operator[] (int x) { return at(x)->rec; }
};