#ifndef STREAM_HPP
#define STREAM_HPP

#include "util/exception/IOException.hpp"

#include <stdint.h>

namespace Util {
namespace IO {

class ResizingBuffer;

class ReadStream
{
public:
	enum Result {
		Ok,
		EndOfStream,
		Error,
		StreamNotReady // For non-blocking streams.
	};

	ReadStream();
	virtual ~ReadStream();
	virtual void close();

	/**
	 * @brief read Reads a single byte into the passed in argument.
	 */
	virtual Result read(uint8_t&) = 0;

	/**
	 * @brief readFull Reads the entire contents of the stream into the passed in buffer.
	 * @param buffer
	 * @return
	 */
	virtual Result readFull(ResizingBuffer& buffer);

	/**
	 * @brief read Reads the contents of the stream until the first '\n' into the buffer.
	 * This does not push the '\n' into the buffer.
	 * @param buffer
	 * @return
	 */
	virtual Result readLine(ResizingBuffer& buffer);
};

}
}

#endif // STREAM_HPP
