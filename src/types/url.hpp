//
// url.hpp
// foundation
//
// Created by Kristian Trenskow on 2019/12/12.
// See license in LICENSE.
//

#ifndef foundation_url_hpp
#define foundation_url_hpp

#include "../memory/object.hpp"
#include "../memory/strong.hpp"
#include "../types/string.hpp"

namespace foundation::types {

	class URL : public Type {

	public:

		static String escapeDecode(
			const String& string
		) noexcept(false);

		static String escapeEncode(
			const String& string
		) noexcept(false);

		static Strong<URL> cwd();

		static Strong<URL> fromString(
			const String& urlString
		) noexcept(false);

		URL() = delete;

		URL(
			const URL& other);

		URL(
			const String& scheme,
			Strong<String> userInfo,
			Strong<String> host,
			uint16_t port,
			const String& path,
			Array<Array<String>> query,
			Strong<String> fragment);

		virtual Kind kind() const override;

		const String& scheme() const;

		const String* userInfo() const;

		const String* host() const;

		uint16_t port() const;

		String path() const;

		const Array<String> queryKeys() const;

		String valueForQueryKey(
			const String& key
		) const;

		const String& fragment() const;

		Strong<URL> appendingPath(
			const String& relativePath
		) const;

		Strong<URL> relativeTo(
			const URL& baseUrl
		) const;

		bool operator==(
			const Type& other
		) const = delete;

		virtual bool equals(
			const Type& other
		) const override;

	private:

		uint16_t _schemaDefaultPort() const;

		String _scheme;
		Strong<String> _userInfo;
		Strong<String> _host;
		uint16_t _port;
		Array<String> _path;
		Array<Array<String>> _query;
		Strong<String> _fragment;

	};

}

#endif /* foundation_url_hpp */
