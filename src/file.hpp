#pragma once

#include <fstream>
#include <memory>
#include <vector>

#include "buffer.hpp"
#include "output.hpp"

class SegmentedFile : public OutputGenerator {

	private:

		std::vector<std::shared_ptr<ByteBuffer>> buffers;

	protected:

		std::shared_ptr<ByteBuffer> appendBuffer(int alignment);
		std::shared_ptr<ByteBuffer> getBuffer(int index);

	public:

		SegmentedFile(const std::string& path);
		void flush() override;

};
