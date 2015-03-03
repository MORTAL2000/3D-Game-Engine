#include "FileIO.h"

#include <algorithm>

#ifdef __WINDOWS_API__
	#include <dirent.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <windows.h>
	#include <Shlobj.h>
#endif

#ifdef __LINUX_API__
	#include <unistd.h>
#endif

#ifdef WIN32
	#ifdef __CYGWIN__
		#define mkdir(file) mkdir(file, 0777)
	#else
		#define mkdir _mkdir
		#define rmdir _rmdir
	#endif
#else
	#define mkdir(file) mkdir(file, 0777)
#endif

//
// #if defined(__MINGW32__) || defined(__CYGWIN__)
// #include <unistd.h>
// #else
// #include <direct.h>
// #endif

namespace FileIO
{

bool isFile(const std::string& filename)
{
	FILE* fp = fopen(filename.c_str(), "rb");
	if(!fp) return false;
	fclose(fp);
	return true;
}

std::vector<std::string> getFilesInDirectory(const std::string& path)
{
	std::vector<std::string> files;
	struct dirent* ent;
	DIR* dir = opendir(path.c_str());
	if(dir)
	{
		while((ent = readdir(dir)))
		{
			std::string curr(ent->d_name);
			if(curr == "." || curr == "..") continue;
			files.push_back(curr);
		}
	}
	closedir(dir);
	return files;
}

std::vector<std::string> getFilesInWorkingDirectory()
{
	auto files = getFilesInDirectory("./");
	return files;
}

void setCurrentDirectory(const std::string& directory)
{
	#ifdef __WINDOWS_API__
		SetCurrentDirectory(directory.c_str());
	#endif

	#ifdef __LINUX_API__
		chdir(directory.c_str());
	#endif
}

std::string getCurrentDirectory()
{
	static char currentDirectory[256];
	getcwd(currentDirectory, 256);
	std::string current = std::string(currentDirectory);
	std::replace(current.begin(), current.end(), '\\', '/');
	return current;
}

bool createDirectory(const std::string& path)
{
	if(mkdir(path.c_str()) != -1)
	{
		return true;
	}
	return false;
}

#ifdef WIN32
static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if(uMsg == BFFM_INITIALIZED)
	{
		std::string tmp = (const char *)lpData;
		SendMessage(hwnd, BFFM_SETSELECTION, true, lpData);
	}
	return 0;
}
#endif

std::string browseFolder()
{
#ifdef WIN32
	TCHAR path[MAX_PATH];
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = "Create project in folder";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)"./";
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if(pidl)
	{
		SHGetPathFromIDList(pidl, path);
		IMalloc * imalloc = 0;
		if(SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}
		return path;
	}
#endif
	return "";
}

//"[description] (*.ext)\0*.ext\0";
std::string browseFile(const char* filter, const char* extension)
{
#ifdef WIN32
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	char filename[MAX_PATH] = "\0";
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = 0;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = extension;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	if(GetOpenFileName(&ofn))
	{
		return filename;
	}
#endif
	return "";
}

}
