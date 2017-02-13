#pragma once

#include <string>
using namespace std;

class Buffer
{
private:
	string buffer;

public:
	Buffer() { buffer = ""; }

	void reset() { buffer = ""; }
	void append(char c) { buffer += c; }
	void set(string i_buffer) { buffer = i_buffer; }

	bool contentsEquals(string s) { return buffer == s; }
	bool isEmpty() { return buffer.length() == 0; }
	char charAt(int index) { return buffer[index]; }
	int length() { return buffer.length(); }
	string value() { return buffer; }
};

class BufferedStream
{
private:
	Buffer buffer; //a main buffer to build the stream
	Buffer history; //a historical buffer (used for successive ungets)
	int historyLength; // the length of history to keep
	int streamIndex;

	bool inHistory() { return streamIndex < 0; }

public:
	BufferedStream(int i_historyLength);

	void archive();
	void set(string s) { buffer.set(s); }

	char get();
	char peek();
	void unget();

	bool atEndOfBuffer() { return streamIndex == buffer.length(); }
};