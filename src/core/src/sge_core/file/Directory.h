#pragma once

namespace sge {

struct Directory {
	Directory() = delete;

	static void setCurrent(StrView dir);
	static String getCurrent();

	static void create(StrView path);
	static bool exists(StrView path);

private:
	static void _create(StrView path);
};

}