#ifndef STREAM_HPP
#define STREAM_HPP

#include "util/Result.hpp"
#include "util/Unit.hpp"

#include <stdint.h>

namespace Util {
namespace IO {

class ResizingBuffer;

class ReadStream
{
public:
	enum class Error {
		EndOfStream,
		StreamNotReady, // For non-blocking streams.
		Unknown
	};

	ReadStream();
	virtual ~ReadStream();
	virtual Result<Unit, Error> close();

	/**
	 * @brief read Reads a single byte into the passed in argument.
	 */
	virtual Result<Unit, Error> read(uint8_t&) = 0;

	/**
	 * @brief readFull Reads the entire contents of the stream into the passed in buffer.
	 * @param buffer
	 * @return
	 */
	virtual Result<Unit, Error> readFull(ResizingBuffer& buffer);

	/**
	 * @brief read Reads the contents of the stream until the first '\\n' into the buffer.
	 * This does not push the '\\n' into the buffer.
	 * @param buffer
	 * @return
	 */
	virtual Result<Unit, Error> readLine(ResizingBuffer& buffer);
};

}
}

#endif // STREAM_HPP
