
#include "file.hpp"

/*
 * SegmentedFile
 */

std::shared_ptr<ByteBuffer> SegmentedFile::appendBuffer(int alignment) {
	auto previous = buffers.empty() ? std::shared_ptr<ByteBuffer>(nullptr) : buffers.back();
	auto buffer = std::make_shared<ByteBuffer>(previous, alignment);

	buffers.emplace_back(buffer);
	return buffer;
}

std::shared_ptr<ByteBuffer> SegmentedFile::getBuffer(int index) {
	return buffers.at(index);
}

void SegmentedFile::write(std::ofstream& stream) {
	for (auto& buffer : buffers) {
		buffer->link();
		buffer->write(stream);
	}
}
