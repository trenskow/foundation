//
// exception.hpp
// shared-foundation-cpp
//
// Created by Kristian Trenskow on 2018/08/17.
// See license in LICENSE.
//

#ifndef shared_foundation_exception_hpp
#define shared_foundation_exception_hpp

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

namespace games::zerobit::shared::foundation::exceptions {

	class Exception {
	public:
		virtual ~Exception() = default;
		virtual const char* description() const = 0;
		virtual Exception* clone() const = 0;
	};

	class NotImplementedException: public Exception {

	public:
		NotImplementedException();

		virtual ~NotImplementedException() = default;

		virtual const char* description() const override;

		virtual NotImplementedException* clone() const override;

	};

	namespace serialization {

		class JSONMalformedException : public Exception {

		private:
			size_t _line;
			size_t _character;

		public:

			JSONMalformedException(
				size_t line,
				size_t character);

			virtual ~JSONMalformedException() = default;

			virtual const char* description() const override;

			virtual JSONMalformedException* clone() const override;

			size_t line() const;
			size_t character() const;

		};

		class JSONEncodingCircularReferenceException : public Exception {

		public:

			virtual ~JSONEncodingCircularReferenceException() = default;

			virtual const char* description() const override;

			virtual JSONEncodingCircularReferenceException* clone() const override;

		};

	}

	namespace memory {

		class AllocationException : public Exception {

		private:
			size_t _size;

		public:
			AllocationException(
				const size_t size);

			AllocationException(
				const AllocationException& other);

			virtual ~AllocationException() = default;

			virtual const char* description() const override;

			virtual AllocationException* clone() const override;

			size_t size() const;

		};

	}

	namespace types {

		class TypeConversionException : public Exception {

		public:

			TypeConversionException();

			virtual ~TypeConversionException() = default;

			virtual const char* description() const override;

			virtual TypeConversionException* clone() const override;

		};

		class RangeParserException : public Exception {

		public:

			RangeParserException();

			virtual ~RangeParserException() = default;

			virtual const char* description() const override;

			virtual RangeParserException* clone() const override;

		};

		class ISO8601Exception : public Exception {

		public:

			ISO8601Exception();

			virtual ~ISO8601Exception() = default;

			virtual const char* description() const override;

			virtual ISO8601Exception* clone() const override;

		};

		class DecoderException : public Exception {

		private:
			size_t _characterIndex;

		public:

			DecoderException(
				const size_t characterIndex);

			DecoderException(
				const DecoderException& other);

			virtual ~DecoderException() = default;

			virtual const char* description() const override;

			virtual DecoderException* clone() const override;

			size_t characterIndex() const;

		};

		class EncoderException: public Exception {

		private:
			size_t _characterIndex;

		public:

			EncoderException(
				const size_t characterIndex);

			virtual ~EncoderException() = default;

			virtual const char* description() const override;

			virtual EncoderException* clone() const override;

			size_t characterIndex() const;

		};

		class EncoderTypeException: public Exception {

		public:
			EncoderTypeException();

			virtual ~EncoderTypeException() = default;

			virtual const char* description() const override;

			virtual EncoderTypeException* clone() const override;

		};

		class OutOfBoundException : public Exception {

		private:
			size_t _index;

		public:

			OutOfBoundException(
				const size_t index);

			OutOfBoundException(
				const OutOfBoundException& other);

			virtual ~OutOfBoundException() = default;

			virtual const char* description() const override;

			virtual OutOfBoundException* clone() const override;

			size_t index() const;

		};

		class KeyNotFoundException : public Exception {

		public:

			KeyNotFoundException();

			virtual ~KeyNotFoundException() = default;

			virtual const char* description() const override;

			virtual KeyNotFoundException* clone() const override;

		};

		class NotFoundException : public Exception {

		public:

			NotFoundException();

			virtual ~NotFoundException() = default;

			virtual const char* description() const override;

			virtual NotFoundException* clone() const override;

		};

		class DurationParserException: public Exception {

		public:

			DurationParserException();

			virtual ~DurationParserException() = default;

			virtual const char* description() const override;

			virtual DurationParserException* clone() const override;

		};

		class UUIDMalformedException : public Exception {

		public:

			UUIDMalformedException();

			virtual ~UUIDMalformedException() = default;

			virtual const char* description() const override;

			virtual UUIDMalformedException* clone() const override;

		};

		class URLMalformedException : public Exception {

		public:

			URLMalformedException();

			virtual ~URLMalformedException() = default;

			virtual const char* description() const override;

			virtual URLMalformedException* clone() const override;

		};

		class UrlDecodingException : public Exception {

			uint8_t _character;

		public:

			UrlDecodingException(
				uint8_t chr);

			UrlDecodingException(
				const UrlDecodingException& other);

			virtual ~UrlDecodingException() = default;

			virtual const char* description() const override;

			virtual UrlDecodingException* clone() const override;

			uint8_t character() const;

		};

	}

	namespace tools {

		class InvalidPatternException : public Exception {

			public:

				InvalidPatternException();

				virtual ~InvalidPatternException() = default;

				virtual const char* description() const override;

				virtual InvalidPatternException* clone() const override;

		};

	}

}

#endif /* shared_foundation_exception_hpp */
