#pragma once

namespace sge {

class NativeUIApp_Base : public NonCopyable {
public:
	class CreateDesc {
	};

			int  run(CreateDesc& desc);
			void quit(int returnCode);
	virtual void willQuit() {}

	void	setCurrentDir(StrView dir)	{ onSetCurrentDir(dir); }
	String	getCurrentDir()				{ return onGetCurrentDir(); }

	String	getExecutableFilename()		{ return onGetExecutableFilename(); }

protected:
	virtual String	onGetExecutableFilename() = 0;
	virtual String	onGetCurrentDir() = 0;
	virtual void	onSetCurrentDir(StrView dir) = 0;

	virtual void onCreate(CreateDesc& desc) {}
	virtual void onRun() {}
	virtual	void onQuit() {}

	int _exitCode = 0;
};

}