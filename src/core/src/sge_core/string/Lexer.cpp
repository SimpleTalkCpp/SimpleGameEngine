#include "Lexer.h"
#include <sge_core/base/Error.h>
#include <sge_core/log/Log.h>
#include <sge_core/file/MemMapFile.h>

namespace sge {

void Lexer::Token::onFormat(fmt::format_context& ctx) const {
	fmt::format_to(ctx.out(), "({}, {})", type, str);
}

void Lexer::reset(ByteSpan source, StrView filename) {
	reset(StrView_make(source), filename);
}

void Lexer::reset(StrView source, StrView filename) {
	_filename = filename;
	_source = source;
	_cur = _source.data();
	_ch = 0;
	_col = 0;
	_line = 1;

	nextChar();
	nextToken();
}

bool Lexer::nextChar() {
	_ch = 0;
	if (!_cur) return false;
	if (_cur >= _source.end()) return false;
	_ch = *_cur;
	_cur++;
	_col++;

	if (_ch == '\n') {
		_line++;
		_col = 0;
	}

	return true;
}

bool Lexer::nextToken() {
	if (!_nextToken()) return false;
//	SGE_DUMP_VAR(_line, _token);
	return true;
}

void Lexer::trimSpaces() {
	for (;;) {
		if (_ch == ' '	|| _ch == '\t' || _ch == '\r') {
			nextChar();
		}else{
			break;
		}
	}
}

void Lexer::skipNewlineTokens() {
	while (_token.isNewline()) {
		nextToken();
	}
}

void Lexer::expectOperator(StrView s) {
	if (_token.isOperator(s)) {
		nextToken();
		return;
	}
	error("expected token {}", s);
}

void Lexer::errorUnexpectedChar() {
	error("Unexpected character [{}]", _ch);
}

void Lexer::errorUnexpectedToken() {
	error("Unexpected token [{}]", _token.str);
}

void Lexer::_error(StrView msg) {
	TempString tmp = msg;
	FmtTo(tmp, "\n{}\n", getLastFewLines(3));

	{
		TempString lastLine = getLastFewLines(1);
		size_t i = 0;
		for (auto& c : lastLine) {
			if (i >= _col) break;
			if (c == ' ' ) { tmp += '-';	continue; }
			if (c == '\n') { tmp += c;		continue; }
			if (c == '\t') { tmp += "----";	continue; }
			tmp += '-';
			i++;
		}
		tmp += "^^^\n";
	}

	FmtTo(tmp, "  token={}\n  file={}:{}:{}\n", _token, _filename, _line, _col);
	throw SGE_ERROR("{}", tmp);
}

bool Lexer::_nextToken() {
	_token.setNone();

	for (;;) {
		trimSpaces();
		if (!_ch) return false;

		if (_ch == '#') {
			_parseCommentSingleLine();
			continue;
		}

		if (_ch == '_' || isAlpha(_ch)) {
			return _parseIdentifier();
		}

		if (isDigit(_ch)) return _parseNumber();
		if (_ch == '\"') return _parseString();

		if (_ch == '\n') {
			_token.type = TokenType::Newline;
			_token.str += "<newline>";
			nextChar();
			return true;
		}

		if (_ch == '/') {
			nextChar();
			if (_ch == '*') { 
				_parseCommentBlock();
				continue;
			}

			if (_ch == '/') { 
				_parseCommentSingleLine();
				continue;
			}

			_token.type = TokenType::Operator;
			_token.str = '/';
			return true;
		}


		_token.type = TokenType::Operator;
		_token.str += _ch;
		nextChar();
		return true;
	}
}

bool Lexer::_parseIdentifier() {
	_token.type = TokenType::Identifier;

	_token.str += _ch;
	nextChar();

	while (_ch) {
		if (_ch == '_' || isAlpha(_ch) || isDigit(_ch)) {
			_token.str += _ch;
			nextChar();
		}else{
			break;
		}
	}
	return true;
}

bool Lexer::_parseNumber() {
	_token.type = TokenType::Number;
	_token.str += _ch;
	nextChar();

	bool hasDot = false;

	while(_ch) {
		if (_ch == '.') {
			if (hasDot) {
				errorUnexpectedChar();
			}
			hasDot = true;
			_token.str += _ch;
			nextChar();
		}else if (isDigit(_ch)) {
			_token.str += _ch;
			nextChar();
		}else{
			break;
		}
	}
	
	return true;
}

bool Lexer::_parseString() {
	_token.type = TokenType::String;

	for (;;) {
		nextChar();
		if (_ch == '\\') {
			nextChar();
			switch (_ch) {
				case '\\':
				case '/':
				case '"':
					_token.str += _ch;
					break;
				case 'b': _token.str += '\b'; break;
				case 'f': _token.str += '\f'; break;
				case 'n': _token.str += '\n'; break;
				case 'r': _token.str += '\r'; break;
				case 't': _token.str += '\t'; break;
				default:
					error("unknown escape character [{}]", _ch);
			}
		}else if (_ch == '\"') {
			nextChar();
			break;
		}else{
			_token.str += _ch;
		}
	}
	return true;
}

void Lexer::_parseCommentBlock() {
	nextChar();			
	for(;;) {
		if (!_ch) return;
		if (_ch == '*') {
			nextChar();
			if (_ch == '/') {
				nextChar();
				return;
			}
		}else{
			nextChar();
		}
	}
}

void Lexer::_parseCommentSingleLine() {
	nextChar();
	for(;;) {
		if (!_ch) return;
		if (_ch == '\n') {
			nextChar();
			return;
		}else{
			nextChar();
		}
	}
}

void Lexer::readString(String& s) {
	if (!_token.isString()) errorUnexpectedToken();
	s = _token.str;
	nextToken();
}

void Lexer::readIdentifier(String& s) {
	if (!_token.isIdentifier()) errorUnexpectedToken();
	s = _token.str;
	nextToken();
}

void Lexer::readBool(bool& v) {
	if (!_token.isIdentifier()) errorUnexpectedToken();
	if (_token.str == "true")
		v = true;
	else if (_token.str == "false")
		v = false;
	else
		errorUnexpectedToken();
	nextToken();
}

StrView Lexer::getLastFewLines(size_t lineCount) {
	if (!_cur) return StrView();

	auto n = lineCount;

	auto* start = _source.data();

	auto* p = _cur;
	for (;p >= start && n > 0; p--) {
		if (*p == '\n') {
			n--;
		}
	}
	p++;

	return StrView(p, _cur - p);
}

StrView Lexer::getRemainSource() const {
	if (!_cur) return StrView();
	auto* s = _cur - 1;
	return StrView(s, _source.end() - s);
}

}