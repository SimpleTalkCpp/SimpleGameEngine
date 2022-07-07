#pragma once

#include <sge_core/base/Error.h>
#include <sge_core/string/StringUtil.h>

namespace sge {

class Lexer : public NonCopyable {
public:
	enum class TokenType {
		None,
		Identifier,
		Number,
		String,
		Operator,
		Newline,
	};

	struct Token {
		TokenType	type = TokenType::None;
		String		str;

		bool isNone() const						{ return type == TokenType::None; }

		bool isIdentifier() const				{ return type == TokenType::Identifier; }
		bool isIdentifier(StrView s) const		{ return type == TokenType::Identifier && s == str; }

		bool isOperator() const					{ return type == TokenType::Operator; }
		bool isOperator(StrView s) const		{ return type == TokenType::Operator && s == str; }

		bool isString() const					{ return type == TokenType::String; }
		bool isString(StrView s) const			{ return type == TokenType::String && s == str; }

		bool isNumber() const					{ return type == TokenType::Number; }

		bool isNewline() const					{ return type == TokenType::Newline; }

		void setNone() { type = TokenType::None; str.clear(); }

		void onFormat(fmt::format_context& ctx) const;
	};

	void reset(ByteSpan source, StrView filename);
	void reset(StrView source, StrView filename);

	bool nextChar();
	bool nextToken();

	void trimSpaces();

	void skipNewlineTokens();

	bool isAlpha	(char c) { return isLowerCase(c) || isUpperCase(c); }
	bool isLowerCase(char c) { return c >= 'a' && c <= 'z'; }
	bool isUpperCase(char c) { return c >= 'A' && c <= 'Z'; }
	bool isDigit	(char c) { return c >= '0' && c <= '9'; }

	const Token& token() const { return _token; }

	template<class... Args>
	void error(const Args &... args) {
		auto msg = Fmt(args...);
		_error(msg);
	}

	void errorUnexpectedChar();
	void errorUnexpectedToken();

	void expectOperator(StrView s);

	void readString(String& s);
	void readIdentifier(String& s);

	void readBool(bool& v);

	template<class E> void readEnum(E& v);

	StrView getLastFewLines(size_t lineCount);

	const char* cur() const { return _cur; }
	StrView source() const	{ return _source; }
	size_t line() const { return _line; }
	size_t col() const { return _col; }

	StrView getRemainSource() const;

protected:

	void _error(StrView msg);

	bool _nextToken();

	bool _parseIdentifier();
	bool _parseNumber();
	bool _parseString();
	void _parseCommentBlock();
	void _parseCommentSingleLine();

	Token			_token;
	String			_filename; // for error message
	StrView			_source;
	const char*		_cur = nullptr;
	char			_ch = 0;
	size_t			_col = 0;
	size_t			_line = 0;
};

inline
const char* enumStr(Lexer::TokenType v) {
	using E = Lexer::TokenType;
	switch (v) {
		case E::None:			return "None";
		case E::Identifier:		return "Identifier";
		case E::Number:			return "Number";
		case E::String:			return "String";
		case E::Operator:		return "Operator";
		case E::Newline:		return "Newline";
		default: {
			SGE_ASSERT(false);
			return "";
		}
	}
}

SGE_FORMATTER_ENUM(Lexer::TokenType)
SGE_FORMATTER(Lexer::Token);


template<class E> inline
void Lexer::readEnum(E& v) {
	if (!token().isIdentifier()) {
		errorUnexpectedToken();
		return;
	}

	if (!enumTryParse(v, _token.str)) {
		error("read enum [{}]", _token.str);
		return;
	}
	nextToken();
}


}
