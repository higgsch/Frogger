#include "buffers.h"
using namespace std;

BufferedStream::BufferedStream(int i_historyLength)
{
	buffer.reset();
	history.reset();
	streamIndex = 0;
	historyLength = i_historyLength;
}

char BufferedStream::get()
{
	if (atEndOfBuffer())
	{
		archive();
		return EOF;
	}

	if (inHistory())
	{
		//increment bufferIndex and then negate (-1 -> 0, -2 -> 1)
		return history.charAt(-++streamIndex);
	}
	else
		return buffer.charAt(streamIndex++);
}

// ----------------------------------------------------------
// This function puts the previously read char back into the 
// to be read again.
// Note: History is limited to historyLimit chars beyond the 
// current buffer.
//
// Version 3.0
// ----------------------------------------------------------
void BufferedStream::unget()
{
	streamIndex--;
}

char BufferedStream::peek()
{
	char c = get();
	unget();
	return c;
}

// ----------------------------------------------------------
// This function fills the history to handle ungets beyond 
// main buffer boundaries.
//
// Version 3.0
// ----------------------------------------------------------
void BufferedStream::archive()
{
	//history is reverse order of buffer
	for (int i = 0; i < buffer.length(); i++)
		history.set(buffer.charAt(i) + history.value());

	//observe history length
	if (history.length() > historyLength)
		history.set(history.value().substr(0,historyLength));

	buffer.reset();
	streamIndex = 0;
}