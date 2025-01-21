#pragma once

#include <fstream>
#include <memory>
#include <vector>

#include "buffer.hpp"

class SegmentedFile {

	private:

		std::vector<std::shared_ptr<ByteBuffer>> buffers;

	protected:

		std::shared_ptr<ByteBuffer> appendBuffer(int alignment);
		std::shared_ptr<ByteBuffer> getBuffer(int index);

	public:

		void write(std::ofstream& stream);

};
