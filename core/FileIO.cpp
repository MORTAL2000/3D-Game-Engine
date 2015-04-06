#include "FileIO.h"

#include <algorithm>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef __WINDOWS_API__
	#include <windows.h>
	#include <Shlobj.h>

	#ifdef __CYGWIN__
		#define mkdir(file) mkdir(file, 0777)
	#else
		#define mkdir _mkdir
		#define rmdir _rmdir
	#endif

	#define S_IFDIR _S_IFDIR
	#define S_IFMT _S_IFMT
	#define S_IFREG _S_IFREG
	#define stat _stat
#endif

#ifdef __LINUX_API__
	#include <unistd.h>
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

std::string current_dir = "";

void updateFilesystem()
{
	if(!current_dir.empty())
	{
		setCurrentDirectory(current_dir);
	}
}

bool is_dir(const char* path)
{
	struct stat buf;
	stat(path, &buf);
	return S_ISDIR(buf.st_mode);
}

bool is_file(const char* path)
{
	struct stat buf;
	stat(path, &buf);
	return S_ISREG(buf.st_mode);
}

bool inDir(const std::string& path, std::string& dir)
{
	unsigned found = path.find_last_of("\\/");
	if(found == (unsigned)-1) return false;
	dir = path.substr(0, found);
	return true;
}

bool getTopDir(const std::string& path, std::string& dir)
{
	std::string temp = path;
	while(inDir(temp, dir))
	{
		temp = dir;
	}
	return dir.empty() == false;
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
	current_dir = directory;
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
		std::string tmp = (const char*)lpData;
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
		updateFilesystem();
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
		updateFilesystem();
		return filename;
	}
#endif
	return "";
}

std::string saveFile()
{
#ifdef WIN32
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	char filename[MAX_PATH] = "\0";
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = 0;
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "txt";

    if(GetSaveFileName(&ofn))
	{
		updateFilesystem();
		return filename;
	}
#endif
	return "";
}

}
