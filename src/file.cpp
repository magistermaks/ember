
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

SegmentedFile::SegmentedFile(const std::string& path)
: OutputGenerator(path) {}

void SegmentedFile::flush() {
	for (auto& buffer : buffers) {
		buffer->link();
		buffer->write(stream);
	}

	stream.flush();
}
