#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <functional>
#include <fstream>

using BufferLink = std::function<void(class ByteBuffer&)>;

class ByteBuffer {

	private:

		int alignment = 1;
		std::shared_ptr<ByteBuffer> previous;
		std::vector<uint8_t> bytes;
		std::vector<BufferLink> links;

	public:

		ByteBuffer(std::shared_ptr<ByteBuffer>& previous, int alignment);

		size_t getEndOffset() const;
		size_t getStartOffset() const;
		size_t getAlignment() const;

		void appendBytes(const void* data, size_t size);
		void addLink(const BufferLink& link);

		size_t size();
		void link();
		void write(std::ofstream& stream);

		ByteBuffer& resize(int size);

	public:

		template <typename T>
		T& as(int index = 0) {
			int extend = index + 1;

			if (sizeof(T) * extend > bytes.size()) {
				throw std::runtime_error {
					"Invalid byte buffer size, expected at least " + std::to_string(sizeof(T)) + " bytes, but got " + std::to_string(bytes.size()) + "!"
				};
			}

			if (bytes.size() % sizeof(T) != 0) {
				throw std::runtime_error {
					"Invalid byte buffer alignment, expected multiple of " + std::to_string(sizeof(T)) + " bytes, but got " + std::to_string(bytes.size()) + "!"
				};
			}

			return reinterpret_cast<T*>(bytes.data())[index];
		}

		template <typename T>
		void appendObject(const T& value) {
			appendBytes(&value, sizeof(T));
		}

		template <typename T>
		void appendVector(const std::vector<T>& vector) {
			appendBytes((void*) vector.data(), sizeof(T) * vector.size());
		}

		void appendString(const std::string& string) {
			appendBytes((void*) string.c_str(), string.size() + 1);
		}

};
