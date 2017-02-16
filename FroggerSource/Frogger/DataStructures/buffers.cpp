//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program provides a char buffer and a char stream
// -----------------------------------------------------------------
#include "buffers.h"
using namespace std;

// ----------------------------------------------------------
// This constructor sets the maximum length of the stream's
// history.
//
// Version 3.0
// ----------------------------------------------------------
BufferedStream::BufferedStream(int i_historyLength)
{
	buffer.reset();
	history.reset();
	streamIndex = 0;
	historyLength = i_historyLength;
}

// ----------------------------------------------------------
// This function returns the next char in the stream, moving 
// the file location ahead.
//
// Version 3.0
// ----------------------------------------------------------
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

// ----------------------------------------------------------
// This function returns the next char in the stream without 
// moving the file location ahead.
// Note: This function can trigger a history shift.
//
// Version 3.0
// ----------------------------------------------------------
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