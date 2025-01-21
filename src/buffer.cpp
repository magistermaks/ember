
#include "buffer.hpp"
#include <cstring>

/*
 * ByteBuffer
 */

ByteBuffer::ByteBuffer(std::shared_ptr<ByteBuffer>& previous, int alignment)
: alignment(alignment), previous(previous) {}

size_t ByteBuffer::getEndOffset() const {
	return getStartOffset() + bytes.size();
}

size_t ByteBuffer::getStartOffset() const {
	size_t start = previous ? previous->getEndOffset() : 0;

	while (start % alignment != 0) {
		start ++;
	}

	return start;
}

void ByteBuffer::appendBytes(const void* data, size_t size) {
	const size_t start = bytes.size();
	bytes.resize(start + size);
	std::memcpy(bytes.data() + start, data, size);
}

void ByteBuffer::addLink(const BufferLink& link) {
	links.emplace_back(link);
}

void ByteBuffer::link() {
	for (BufferLink& link : links) link(*this);
}

void ByteBuffer::write(std::ofstream& stream) {

	long cursor = stream.tellp();
	long start = getStartOffset();
	long padding = start - cursor;

	while (padding > 0) {
		stream.put(0);
		padding --;
	}

	stream.write((char*) bytes.data(), bytes.size());
}

ByteBuffer& ByteBuffer::resize(int size) {
	bytes.resize(size + bytes.size());
	return *this;
}

size_t ByteBuffer::size() {
	return bytes.size();
}

size_t ByteBuffer::getAlignment() const {
	return alignment;
}
